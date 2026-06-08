# Adding runtime `.ttf` support to CrossPoint

How to give upstream **CrossPoint** (`/Users/jmitch/GitHub/crosspoint-reader-main`)
the same "drop a `.ttf` on the device, pick any size, it renders" capability that
**Murphy M4 / MurphyOS** ships — by mirroring CrossPoint's *own* font
architecture. Murphy is a CrossPoint fork
([`murphy_reader_code_reuse.md`](murphy_reader_code_reuse.md)), so it built its
TTF path on exactly the classes described below; this is a back-port, not a
greenfield design.

- M4 behavior + binary evidence: [`murphy_reader_ttf_fonts.md`](murphy_reader_ttf_fonts.md)
- All `file:line` references below are into `crosspoint-reader-main`.

## TL;DR

CrossPoint already has every layer except an **on-device TrueType rasterizer**.
A `.ttf` backend is the runtime-rasterizing twin of the existing `SdCardFont`:
discover → load → register with the renderer as a `fontId`, and serve glyphs
through the same `glyphMissHandler` the renderer already calls. The only renderer
change needed is generalizing **one** function (`getGlyphBitmap`) so it isn't
hard-wired to `SdCardFont`. Recommended rasterizer: vendor `stb_truetype.h`
(public-domain; cmap + `glyf` incl. composites + antialiased raster + metrics),
which matches M4's `glyf`-only, CFF/TTC/WOFF-rejecting feature set.

---

## Part 1 — How CrossPoint's font system actually works

### Everything is a `fontId → EpdFontFamily` in the renderer

`GfxRenderer` holds two maps (`lib/GfxRenderer/GfxRenderer.h:53,58`):

```cpp
std::map<int, EpdFontFamily>  fontMap;          // fontId -> 4 styles (R/B/I/BI)
std::map<int, SdCardFont*>    sdCardFonts_;      // fontId -> SD backend (for the lazy path)
```

There are **three** font sources, all reduced to a `fontId`:

1. **Built-in** — `NOTOSERIF_*` / `NOTOSANS_*` at fixed sizes 12/14/16/18.
   These are DEFLATE-compressed: `EpdFontData.groups != nullptr`, decompressed on
   demand by `FontCacheManager` / `FontDecompressor`
   (`GfxRenderer::getGlyphBitmap`, `GfxRenderer.cpp:30-42`).
2. **SD `.cpfont`** — discovered by `SdCardFontRegistry`, loaded by
   `SdCardFontManager`, orchestrated by `SdCardFontSystem`. Glyphs load lazily
   (below). This is the model to copy.
3. **Runtime TTF** — *does not exist yet*. The gap is purely the rasterizer +
   its wiring.

### Draw-time: which font is active

`CrossPointSettings::getReaderFontId()` (`CrossPointSettings.cpp:347`) resolves
the active id every render:

```cpp
if (sdFontFamilyName[0] && sdFontIdResolver)        // SD font selected?
    if (int id = sdFontIdResolver(ctx, sdFontFamilyName, fontSize)) return id;
switch (fontFamily) { case NOTOSERIF: ... NOTOSERIF_14_FONT_ID ... }   // else built-in
```

The reader passes this id into every `GfxRenderer` measure/draw call
(`EpubReaderActivity.cpp:914,921,…`, `TxtReaderActivity.cpp:99`).

### Glyph fetch: the miss handler is the extension point

`EpdFont::getGlyph(cp)` (`EpdFont.cpp:154-185`):

1. binary-search the resident interval table;
2. on miss, call `data->glyphMissHandler(data->glyphMissCtx, cp)`;
3. on miss-of-miss, fall back to `REPLACEMENT_GLYPH`.

`EpdFontData` exposes the hook (`EpdFontData.h:139-143`):

```cpp
const EpdGlyph* (*glyphMissHandler)(void* ctx, uint32_t codepoint);
void* glyphMissCtx;
```

### Glyph bytes: `getGlyphBitmap`

`GfxRenderer::getGlyphBitmap` (`GfxRenderer.cpp:29-56`) returns the packed bitmap
for an `EpdGlyph`:

```cpp
if (fontData->groups) return decompressor->getBitmap(...);          // built-in compressed
if (fontData->glyphMissCtx) {                                       // <-- SD-SPECIFIC
    auto* sdFont = SdCardFont::fromMissCtx(fontData->glyphMissCtx);
    if (sdFont->isOverflowGlyph(glyph)) return sdFont->getOverflowBitmap(glyph);
}
return &fontData->bitmap[glyph->dataOffset];                        // resident bitmap
```

`renderCharImpl` then unpacks 1-bit (8 px/byte, MSB-first) or, when
`fontData->is2Bit`, 2-bit (4 px/byte, `0=white 1=light 2=dark 3=black`) and calls
`drawPixel` (`GfxRenderer.cpp` ~150-290). **The EPD is 4-level grayscale, so a
TTF backend should emit 2-bit.**

`EpdGlyph` is exactly 16 bytes (`SdCardFont.cpp:14` `static_assert`):
```c
uint8_t width, height;  uint16_t advanceX /*12.4 fp*/;  int16_t left, top;
uint16_t dataLength;    uint32_t dataOffset;
```

### The SD lazy path = the exact template for TTF

`SdCardFont::onGlyphMiss` (`SdCardFont.cpp:1253-1335`):

1. `ctx` is an `OverflowContext{ SdCardFont* self; uint8_t styleIdx; }`.
2. Check an 8-slot **ring buffer** (`OVERFLOW_CAPACITY = 8`, `SdCardFont.h:199-209`)
   for an already-loaded `(cp, style)`.
3. Else find the glyph index, read the `EpdGlyph` + its bitmap **from the file**
   into temporaries, commit to the next ring slot (evicting the oldest), return
   `&overflow_[slot].glyph`.

`getOverflowBitmap` / `isOverflowGlyph` / `fromMissCtx`
(`SdCardFont.cpp:1337-1353`) are what `getGlyphBitmap` uses to recover the bytes.
A TTF backend keeps this exact shape — it only swaps step 3's *"read EpdGlyph +
bitmap from `.cpfont`"* for *"rasterize one glyph from the TTF outline."*

### Lifecycle: registry → manager → system

- `SdCardFontManager::loadFamily` (`SdCardFontManager.cpp:33-86`): `new
  SdCardFont` → `load(path)` → `computeFontId(contentHash, name, pt)` →
  `renderer.registerSdCardFont(id, font)` → `renderer.insertFont(id,
  EpdFontFamily(4 styles))`.
- `SdCardFontSystem` (`src/SdCardFontSystem.{h,cpp}`): `begin()` discovers + loads
  the saved selection and installs the `sdFontIdResolver` lambda into
  `SETTINGS`; `ensureLoaded()` reloads on family/size change or after a web
  upload marks the registry dirty; `resolveFontId()` returns the loaded id.

### Per-page prewarm (where glyphs get pulled before drawing)

The reader does a **scan pass** then a **render pass** per page
(`EpubReaderActivity.cpp:911-921`). Between them it prewarms: SD fonts batch-read
the page's glyph metrics/bitmaps (`GfxRenderer::ensureSdCardFontReady` →
`SdCardFont::buildAdvanceTable` / `prewarm`). Glyphs still missing at draw time
fall through to `onGlyphMiss` (the overflow ring). A TTF backend hooks the same
two stages.

### Upload + size model

- Web upload: `POST /api/fonts/upload` → `handleFontUploadData`
  (`CrossPointWebServer.cpp:165, 1760-1885`): validates family name + `.cpfont`
  filename, checks magic `CPFONT\0\0`, streams to `/.fonts/<family>/`, then
  `sdFontSystem.markRegistryDirty()`. Roots are `/.fonts/` and `/fonts/`
  (`docs/sd-card-fonts.md`).
- Size is a fixed **enum** `SMALL/MEDIUM/LARGE/EXTRA_LARGE`
  (`CrossPointSettings.h:105`); the manager picks the closest prebuilt `.cpfont`
  size. **This is the core limitation TTF removes** — TTF renders at an arbitrary
  pixel size.

---

## Part 2 — The port, layer by layer

Each new piece is the twin of an existing one.

| CrossPoint today | New for TTF |
|---|---|
| `SdCardFont` (reads glyphs from `.cpfont`) | `TtfFont` (rasterizes glyphs) |
| `SdCardFont::onGlyphMiss` (file read) | `TtfFont::onGlyphMiss` (cmap→gid→rasterize) |
| `SdCardFontManager` | `TtfFontManager` |
| `SdCardFontSystem` | `TtfFontSystem` |
| `getReaderFontId()` SD branch | TTF branch (checked first) |
| `sdFontFamilyName` + `fontSize` enum | `ttfFontName` + `ttfFontSize` (px) |
| `.cpfont` upload (`CPFONT\0\0`) | `.ttf` upload (sfnt `0x00010000`/`true`) |
| `/.fonts/<fam>/*.cpfont` | `/.fonts/*.ttf` (flat) |

### 2.1 `TtfFont` (`lib/EpdFont/TtfFont.{h,cpp}`)

- **Load/validate** (mirror M4's parser): read the file (whole-file into PSRAM
  for latin faces; keep table-dir + `loca` resident and read `glyf` slices on
  demand for big CJK). Validate sfnt `0x00010000`/`'true'`; **reject** `OTTO`,
  `ttcf`, `wOFF`, `wOF2`. Require `head, maxp, loca, glyf, cmap, hhea, hmtx`
  (`stbtt_InitFont` covers most).
- **Metrics**: `scale = stbtt_ScaleForPixelHeight(ttfFontSize)`; `advanceY` /
  `ascender` / `descender` from `hhea` × scale; per-glyph `advanceX` →
  `round(adv*16)` (12.4 fp), `left/top` from `stbtt_GetGlyphBitmapBox`.
- **`onGlyphMiss`** — same ring-buffer skeleton as `SdCardFont`, body swapped:
  ```cpp
  int gid = stbtt_FindGlyphIndex(&font_, cp);            // cmap
  if (!gid) return nullptr;                              // -> replacement
  uint8_t* cov = stbtt_GetGlyphBitmap(&font_, 0, scale_, gid, &w,&h,&xoff,&yoff);
  uint8_t* packed = arena_.alloc(pack2bppSize(w,h));
  pack2bpp(packed, cov, w, h);                           // 8-bit coverage -> 0..3
  stbtt_FreeBitmap(cov, 0);
  EpdGlyph* g = ringAlloc(cp);                           // evict oldest
  g->width=w; g->height=h; g->left=xoff; g->top=-yoff;  // mind sign of yoff
  g->advanceX = round16(advancePx(gid));
  // bitmap recovered via getOverflowBitmap(g)
  return g;
  ```
  Set `is2Bit=true`, `glyphMissHandler=&TtfFont::onGlyphMiss`,
  `glyphMissCtx=&ctx_`, intervals empty (or seed ASCII). Use a **PSRAM bitmap
  arena** + a per-glyph size cap (M4: `Glyph %d glyf too large`); add a mutex if
  rasterizing off the render task.

### 2.2 Generalize `getGlyphBitmap` (the one required renderer change)

`getGlyphBitmap` (`GfxRenderer.cpp:48-51`) hard-casts `glyphMissCtx` to an
`SdCardFont`. A `TtfFont` ctx would be misread → crash. Make the overflow
recovery backend-agnostic. Minimal version — a tiny interface both backends
implement:

```cpp
struct OverflowSource {                       // in EpdFontData.h or a new header
  virtual bool isOverflowGlyph(const EpdGlyph*) const = 0;
  virtual const uint8_t* getOverflowBitmap(const EpdGlyph*) const = 0;
};
// EpdFontData gains: OverflowSource* overflowSource;  (set alongside glyphMissCtx)
```
```cpp
// getGlyphBitmap:
if (fontData->overflowSource && fontData->overflowSource->isOverflowGlyph(glyph))
    return fontData->overflowSource->getOverflowBitmap(glyph);
```
`SdCardFont` implements `OverflowSource` (it already has both methods); `TtfFont`
implements it too. `fromMissCtx` and the SD-specific cast go away. This is the
*only* change to shared renderer code.

### 2.3 Manager + system (twins)

- `TtfFontManager`: `load(path, pixelSize)` → `new TtfFont` → `computeFontId` →
  `renderer.registerSdCardFont`-equivalent (either reuse `sdCardFonts_` keyed by
  id, or add a parallel `ttfFonts_` map + `registerTtfFont`) → `insertFont(id,
  EpdFontFamily)`. TTF families are typically one style; pass the same `TtfFont`
  for R/B/I/BI or synthesize bold/italic later.
- `TtfFontSystem`: `begin()` / `ensureLoaded()` / `resolveFontId()` parallel to
  `SdCardFontSystem`, driven by `ttfFontName` + `ttfFontSize`.

### 2.4 Settings, resolution, UI

- `CrossPointSettings`: add `char ttfFontName[…]` and `uint16_t ttfFontSize`
  (px). M4 uses exactly these keys (`ttfFontName`/`ttfFontSize`), so add them to
  `JsonSettingsIO` too.
- `getReaderFontId()`: add a TTF branch **before** the SD branch:
  ```cpp
  if (ttfFontName[0] && ttfFontIdResolver)
      if (int id = ttfFontIdResolver(ttfCtx, ttfFontName, ttfFontSize)) return id;
  ```
- `FontSelectionActivity` (`src/activities/settings/FontSelectionActivity.cpp`):
  add a `Custom (TTF)` entry that lists `*.ttf` under `/.fonts/` and writes
  `ttfFontName`/`ttfFontSize`. (M4 labels: `Custom (TTF)`, `TTF Font`, `No TTF
  fonts found`, activity `TtfFontSelect`.) Expose a numeric size, not the 4-size
  enum.

### 2.5 Upload + discovery

- Web (optional, a nicety M4 doesn't even do — it relies on the file
  manager/USB): in `handleFontUploadData` accept `.ttf`, swap the `CPFONT\0\0`
  magic check for sfnt `0x00010000`/`'true'`, store to `/.fonts/`, then
  `markRegistryDirty()`.
- Discovery: scan `/.fonts/` and `/fonts/` for `*.ttf` (parallel to
  `SdCardFontRegistry::discover`). Keep an optional default name (`reader.ttf`)
  and CJK fallback, as M4 does.

### 2.6 Prewarm

Add a `ttfFontSystem.ensureLoaded(renderer)` call next to
`sdFontSystem.ensureLoaded` (`ReaderActivity.cpp:107`). Per-page glyphs flow
through `onGlyphMiss` automatically; optionally pre-rasterize the scan-pass text
in a `prewarm()` to keep the draw pass off the rasterizer.

---

## Scope / cut-lines (match M4, keep it small)

- **Support:** plain TrueType `glyf` outlines, quadratic Béziers, composite
  glyphs (stb handles these), `cmap` formats 4/6/12, optional `kern`.
- **Reject (like M4):** CFF/`OTTO`, `ttcf` collections, `WOFF`/`WOFF2`.
- **Skip:** hinting (unneeded at EPD sizes); `GPOS`/`GSUB` shaping — CrossPoint
  already does ligatures + combining-mark placement above the glyph layer
  (`EpdFont::applyLigatures`, `combiningMark` in `EpdFontData.h`), unchanged.
- **Grayscale:** rasterize antialiased, quantize to 2-bit (`is2Bit=true`).

## Validation

1. Unit: parse a known `.ttf`; assert table set, `unitsPerEm`, a few advances vs
   `fonttools`.
2. Golden glyph: rasterize `A`/`g` at 16 px; compare the 2-bit bitmap to an
   `stbtt`/freetype-py reference.
3. On-device: `reader.ttf` in `/.fonts/`, select `Custom (TTF)`, open an EPUB,
   check line height/advances and refresh timing.
4. Stress: a page of unique CJK to exercise arena growth, the size cap, and
   ring-buffer eviction without leaks.

## Concrete change list

| File | Change |
|---|---|
| `lib/EpdFont/TtfFont.{h,cpp}` | **new** — rasterizing font backend |
| `lib/EpdFont/stb_truetype.h` | **new** — vendored rasterizer |
| `lib/EpdFont/EpdFontData.h` | add `OverflowSource* overflowSource` |
| `lib/GfxRenderer/GfxRenderer.cpp` | generalize `getGlyphBitmap` overflow branch (2.2) |
| `src/TtfFontSystem.{h,cpp}` + `TtfFontManager.{h,cpp}` | **new** — twins of the SD versions |
| `src/CrossPointSettings.{h,cpp}` | `ttfFontName`/`ttfFontSize`, `getReaderFontId()` TTF branch, resolver hook |
| `src/JsonSettingsIO.cpp` | persist the two keys |
| `src/activities/settings/FontSelectionActivity.cpp` | `Custom (TTF)` picker |
| `src/activities/reader/ReaderActivity.cpp` | `ttfFontSystem.ensureLoaded()` |
| `src/network/CrossPointWebServer.cpp` | accept `.ttf` upload (optional) |
| `src/main.cpp` | instantiate `ttfFontSystem`, `begin()` |

## References

- M4 RE + function map: [`murphy_reader_ttf_fonts.md`](murphy_reader_ttf_fonts.md)
  (parser `entry 0x4203f520`; runtime manager `entry 0x4207d8d4`).
- CrossPoint: `lib/EpdFont/EpdFont.cpp:154` (getGlyph), `EpdFontData.h:75,139`
  (EpdGlyph, miss hook), `lib/EpdFont/SdCardFont.cpp:1253` (onGlyphMiss),
  `lib/GfxRenderer/GfxRenderer.cpp:29` (getGlyphBitmap),
  `lib/EpdFont/SdCardFontManager.cpp:33` (loadFamily/registration),
  `src/SdCardFontSystem.cpp` (orchestration),
  `src/CrossPointSettings.cpp:347` (getReaderFontId),
  `src/network/CrossPointWebServer.cpp:1760` (upload).
- Rasterizer: `stb_truetype.h` (nothings/stb), public domain.

# Porting runtime `.ttf` support to CrossPoint

How to give upstream **CrossPoint** the same "drop a `.ttf` on the device and
pick any size" capability that **Murphy M4 / MurphyOS** (`murphy-26-0526-1.2.16`)
ships. The M4 firmware is itself a CrossPoint fork (see
[`murphy_reader_code_reuse.md`](murphy_reader_code_reuse.md)), so this is a
feature back-port onto code CrossPoint already has — not a from-scratch design.

The reverse-engineered evidence for M4's implementation is in
[`murphy_reader_ttf_fonts.md`](murphy_reader_ttf_fonts.md). This document is the
"how to build the same thing in CrossPoint" companion.

## TL;DR

- CrossPoint already has everything except an **on-device TrueType rasterizer**:
  it has lazy glyph loading (`EpdFontData::glyphMissHandler`), an overflow ring
  buffer, a renderer that draws `EpdGlyph` bitmaps (1-bit and 2-bit/4-level),
  per-size font management, a settings/selection UI, and a web/USB upload path.
- Add a new `TtfFont` class that is the runtime-rasterizing sibling of
  `SdCardFont`: it parses a `.ttf` and, on each glyph miss, rasterizes one glyph
  into an `EpdGlyph` + packed bitmap in the overflow buffer.
- **No `GfxRenderer` changes are required.** The renderer already pulls glyphs
  through `glyphMissCtx`; it doesn't care whether the bytes came from a
  `.cpfont` on SD or a freshly rasterized outline.
- Recommended rasterizer: vendor **`stb_truetype.h`** (single-header,
  public-domain, does cmap + `glyf` incl. composites + antialiased raster +
  metrics). That matches M4's feature set (`glyf`-only; CFF/TTC/WOFF rejected)
  with far less custom code than M4's bespoke parser.

## What M4 does (target behavior to reproduce)

From the RE:

| Aspect | M4 behavior |
|--------|-------------|
| Accepted format | plain TrueType outlines (`glyf`); rejects `OTTO`/CFF, `ttcf` collections, `WOFF`/`WOFF2` |
| Tables used | `head`, `maxp`, `loca`, `glyf`, `cmap`, `hhea`, `hmtx`, optional `kern` |
| SFNT version check | `0x00010000` or `'true'` |
| Storage | `/.mofei/fonts/*.TTF`; default `reader.ttf`; CJK fallback `notosans_cjk_tc_fallback.ttf`; a separate UI face |
| Sizing | arbitrary ppem from the `ttfFontSize` setting (not fixed buckets) |
| Glyph loading | lazy per-glyph: reopen file, read the `glyf` slice via `loca`, rasterize, cache in growable arenas; per-glyph + runtime mutexes; per-glyph size cap |
| Rasterizer | bespoke, **not** FreeType |
| File intake | generic `/upload?path=/.mofei/fonts` or USB MSC — *not* the web Fonts page (that's `.epf` CJK packs only) |

## CrossPoint's integration seam (what you build against)

The renderer talks to fonts through three things, all already present:

1. **`EpdGlyph`** — `lib/EpdFont/EpdFontData.h`. Exactly 16 bytes
   (`static_assert` in `SdCardFont.cpp`):
   ```c
   uint8_t  width, height;   // bitmap dims in px (max 255)
   uint16_t advanceX;        // 12.4 fixed-point px
   int16_t  left, top;       // bitmap origin relative to pen
   uint16_t dataLength;      // bitmap byte length
   uint32_t dataOffset;      // offset into the font's bitmap buffer
   ```
2. **Bitmap packing** (`GfxRenderer.cpp`, `renderCharImpl`):
   - 1-bit: 8 px/byte, MSB first, row-major.
   - 2-bit (`EpdFontData::is2Bit`): 4 px/byte, MSB first, `0=white 1=light
     2=dark 3=black`. **Use this** — the EPD is 4-level grayscale, so antialiased
     output maps directly.
3. **The miss handler** (`EpdFont.cpp:177`):
   ```c
   const EpdGlyph* (*glyphMissHandler)(void* ctx, uint32_t codepoint);
   void* glyphMissCtx;
   ```
   `EpdFont::getGlyph(cp)` calls this when `cp` isn't in the resident interval
   table. `GfxRenderer::getGlyphBitmap()` recovers the backend via
   `fromMissCtx(glyphMissCtx)` and reads the bitmap from the overflow buffer.
   The returned `EpdGlyph*` only has to stay valid until the next miss eviction —
   exactly the ring-buffer contract `SdCardFont` already implements
   (`overflow_`, `overflowNext_`, `clearOverflow`).

`SdCardFont` (`lib/EpdFont/SdCardFont.{h,cpp}`) is the reference implementation
of a lazy, miss-handler-driven font. **Copy its skeleton.** The only thing that
changes is the *source* of each glyph's metrics+bitmap: SD `.cpfont` slice →
on-the-fly TrueType rasterization.

## Design: a `TtfFont` class

```
lib/EpdFont/TtfFont.{h,cpp}        // sibling of SdCardFont
lib/EpdFont/stb_truetype.h         // vendored rasterizer (or bespoke)
src/EpdFont/TtfFontManager.{h,cpp} // sibling of SdCardFontManager
```

### 1. Load / validate (mirrors M4's parser)

- Read the font into a buffer. For latin/Cyrillic/Greek faces (≤ ~1–2 MB) load
  the whole file into **PSRAM** and hand the pointer to `stbtt_InitFont`. For
  large CJK faces, keep M4's approach: hold only the table directory + `loca`
  resident and read each `glyf` slice from the file on demand (stb can be driven
  off a buffer you refill, or use a custom reader).
- Validate the SFNT version (`0x00010000`/`'true'`); **reject** `OTTO`, `ttcf`,
  `wOFF`, `wOF2` (return load-failure, matching M4).
- Confirm required tables exist: `head, maxp, loca, glyf, cmap, hhea, hmtx`
  (stb checks most of these in `stbtt_InitFont`).

### 2. Metrics

- `unitsPerEm` from `head`; `scale = ttfFontSize_px / unitsPerEm`
  (`stbtt_ScaleForPixelHeight`).
- `EpdFontData.advanceY` / `ascender` / `descender` from `hhea`
  (`stbtt_GetFontVMetrics` × scale).
- Per glyph: `advanceX` from `hmtx` → convert to 12.4 fixed-point
  (`round(adv_px * 16)`); `left`/`top` from the glyph bbox
  (`stbtt_GetGlyphBitmapBox`).

### 3. Glyph miss handler (the core)

```c
static const EpdGlyph* TtfFont::onGlyphMiss(void* ctx, uint32_t cp) {
  auto* self = static_cast<TtfFont*>(ctx);
  int gid = stbtt_FindGlyphIndex(&self->font_, cp);     // cmap
  if (!gid) return nullptr;                              // -> replacement glyph
  // rasterize to an 8-bit coverage buffer at self->scale_
  int w,h,xoff,yoff;
  uint8_t* cov = stbtt_GetGlyphBitmap(&self->font_, 0, self->scale_, gid, &w,&h,&xoff,&yoff);
  // quantize 8-bit coverage -> 2-bit (white/light/dark/black), pack 4px/byte MSB-first
  uint8_t* packed = self->arena_.alloc(packedSize(w,h));
  pack2bpp(packed, cov, w, h);                           // 0..255 -> 0..3
  stbtt_FreeBitmap(cov, 0);
  // fill EpdGlyph in the overflow ring (evict oldest if full)
  EpdGlyph* g = self->overflowAlloc(cp);
  g->width=w; g->height=h; g->left=xoff; g->top=yoff;    // note sign of yoff
  g->advanceX = round16(advancePx(gid));
  g->bitmap = packed;                                    // via overflow getOverflowBitmap()
  return g;
}
```
Set `is2Bit = true`, `glyphMissHandler = &TtfFont::onGlyphMiss`,
`glyphMissCtx = this`, and leave the resident interval table empty (or seed it
with ASCII for speed) so everything flows through the miss path — same as
`SdCardFont`'s overflow path.

### 4. Memory & concurrency (copy M4's shape)

- Bitmap **arena in PSRAM**; cap single-glyph size (M4: `Glyph %d glyf too
  large`) to reject pathological glyphs.
- Overflow **ring buffer** of recently rasterized glyphs (reuse
  `SdCardFont::MAX_PAGE_GLYPHS`-style cap). Rasterizing is slower than an SD read,
  but EPD page refresh is ~hundreds of ms, so per-page rasterization of a few
  hundred unique glyphs is acceptable; the ring amortizes repeats within a page.
- If glyphs are rasterized from a render task while layout runs elsewhere, add a
  mutex (M4 has both a per-glyph and a runtime mutex).

## Wiring it into the app

1. **Discovery / storage.** Scan CrossPoint's existing font roots `/.fonts/`
   and `/fonts/` (see `docs/sd-card-fonts.md`) for `*.ttf`. Keep M4's
   convention of a default name (`reader.ttf`) and an optional CJK fallback.
   (M4 uses `/.mofei/fonts/`; CrossPoint should use its own roots, not
   `/.mofei/`.)
2. **Settings.** Add `ttfFontName` + `ttfFontSize` equivalents to
   `CrossPointSettings` / `JsonSettingsIO` (M4 stores exactly these keys).
3. **Selection UI.** Add a `Custom (TTF)` entry to `FontSelectionActivity`
   (M4's labels: `Custom (TTF)`, `TTF Font`, `No TTF fonts found`,
   `TtfFontSelect`). Unlike the `.cpfont` size *enum* (SMALL..EXTRA_LARGE in
   `SdCardFontManager`), expose a numeric size since TTF is size-free.
4. **Renderer registration.** Add a `TtfFontManager` that heap-allocates a
   `TtfFont`, registers it with `GfxRenderer` the way `SdCardFontManager` does,
   and reloads on size/name change.
5. **Upload (optional).** To match "upload from the browser", extend the fonts
   upload handler + `FontsPage.html` to accept `.ttf` (content-type
   `application/x-font-ttf`) and store to `/.fonts/`. Note M4 itself does *not*
   put TTF on the web Fonts page — it relies on the generic file upload / USB —
   so this step is a CrossPoint nicety, not required for parity.

## Scope / cut-lines (match M4, keep it small)

- **Support:** plain TrueType `glyf` outlines, quadratic Béziers, composite
  glyphs (stb handles these), `cmap` formats 4/6/12, kerning via `kern`
  (optional).
- **Reject (like M4):** CFF/OpenType-`OTTO`, TrueType Collections `ttcf`,
  `WOFF`/`WOFF2`. No hinting (not needed at EPD sizes). No `GPOS`/`GSUB` shaping
  — CrossPoint already does its own ligatures/combining-mark handling above the
  glyph layer (`EpdFont::applyLigatures`, `combiningMark` helpers), which keeps
  working unchanged.
- **Grayscale:** rasterize antialiased and quantize to 2-bit. (Open question
  carried from the RE: confirm whether M4 antialiases or thresholds to 1-bit;
  2-bit is the better EPD result and the renderer supports it natively.)

## Validation plan

1. Unit: parse a known `.ttf`, assert table set + `unitsPerEm` + a few glyph
   advances against `fonttools`.
2. Golden glyph: rasterize `'A'`/`'g'` at 16 px, compare the 2-bit bitmap to a
   `stbtt`/freetype-py reference rendered the same way.
3. On-device: drop `reader.ttf` in `/.fonts/`, select `Custom (TTF)`, open an
   EPUB, verify layout metrics (line height, advances) and refresh timing.
4. Stress: a page of unique CJK codepoints to exercise arena growth, the size
   cap, and ring-buffer eviction without leaks.

## References

- M4 implementation evidence + function map:
  [`murphy_reader_ttf_fonts.md`](murphy_reader_ttf_fonts.md)
  (parser `entry 0x4203f520`, runtime manager `entry 0x4207d8d4`).
- CrossPoint seam: `lib/EpdFont/EpdFontData.h` (`EpdGlyph`, `glyphMissHandler`),
  `lib/EpdFont/SdCardFont.{h,cpp}` (lazy/overflow pattern to mirror),
  `lib/GfxRenderer/GfxRenderer.cpp` (`getGlyphBitmap`, 1-/2-bit `renderCharImpl`),
  `lib/EpdFont/SdCardFontManager.h`, `src/activities/settings/FontSelectionActivity.*`,
  `docs/sd-card-fonts.md`.
- Recommended rasterizer: `stb_truetype.h` (nothings/stb), public domain.

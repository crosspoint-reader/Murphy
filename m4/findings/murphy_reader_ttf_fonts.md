# Murphy Reader v1.2.16 native TTF font support

## Summary

Murphy Reader v1.2.16 does support `.ttf` fonts natively at runtime. This is
not just CrossPoint's converted SD-card font-pack support.

The firmware contains three distinct font paths:

1. CrossPoint-derived built-in compressed EPD fonts (`FontDecompressor`,
   `GfxRenderer`, `FDC` log tag).
2. Murphy-specific multilingual `.epf`/`EPF2` packs under `/.mofei/fonts/`,
   used for fixed Noto Sans CJK sizes and web-upload management.
3. A native runtime TrueType loader for user `.ttf` files, with settings
   fields `ttfFontName` and `ttfFontSize`, UI labels `Custom (TTF)` and
   `TTF Font`, and direct parsing of TrueType tables — `head`, `maxp`, `loca`,
   `glyf`, `cmap`, `hhea`, `hmtx` (and optional `kern`). See the code-level RE
   below for which tags are *fetched* vs merely *detected and rejected*.

The native TTF path appears to be a custom compact TrueType rasterizer/loader,
not FreeType. The binary has no obvious `FreeType`/`FT_` strings, but it has
many bespoke parse/read/allocation errors around TrueType table handling.

## Artifacts

Analyzed binary:

```text
m4/murphy-26-0526-1.2.16.bin
SHA-256: 3ee3d0a7207a17d49eb47fa60febff8cf4ac2f47bb52d74a1beceb40b8b124ea
```

Focused Ghidra/xref artifact:

```text
m4/findings/font_ttf_pointer_refs.md
```

An earlier raw Ghidra import at `0x42000000` recovered *no* xrefs for these
strings (see `font_ttf_pointer_refs.md`, "direct refs: none" throughout). That
import was wrong, not the binary: see the next section — once the real ESP32-S3
segment map is applied, every reference resolves.

## Code-level RE (segment-corrected)

The "no xrefs" result was an artifact of loading the flat `.bin` at a single
base. The image is a multi-segment ESP-IDF app; the strings live in **DROM**,
not IROM. Parsing the image header (`esptool image-info`, cross-checked with a
hand parser) gives:

| Seg | Type | Vaddr range | File off | Len |
|----:|------|-------------|---------:|----:|
| 0 | DROM | `0x3c1f0020`–`0x3c3a00e4` | `0x000018` | `0x1b00c4` |
| 1 | DRAM | `0x3fca1500`–`0x3fca7fac` | `0x1b00ec` | `0x06aac` |
| 2 | IRAM | `0x40378000`–`0x40381478` | `0x1b6ba0` | `0x09478` |
| 3 | IROM | `0x42000020`–`0x421e8d7c` | `0x1c0020` | `0x1e8d5c` |
| 4 | IRAM | `0x40381478`–`0x40391460` | `0x3a8d84` | `0x0ffe8` |

So e.g. `"Custom (TTF)"` at file `0x13b58` is **DROM `0x3c203b58`**, not
`0x42013b58`. The code references it through an `l32r` literal holding the
`0x3c20…` address; the raw import had the string at `0x4201…`, so nothing
matched. Recovering the references is then just: compute each string's true
DROM vaddr, scan the executable segments for word-aligned literals equal to it,
and disassemble the enclosing function (capstone 6 has an Xtensa backend).
Tooling: [`tools/esp32s3_xtensa_disasm.py`](../../tools/esp32s3_xtensa_disasm.py)
(literal-aware Xtensa disassembler that annotates `l32r` loads with the C string
they point to).

### Four functions, cleanly separated

The TTF references cluster into four distinct functions (windowed-ABI `entry`
prologues bound each one):

| Role | Function `entry` | Literal pool | What it touches |
|------|------------------|--------------|-----------------|
| **TrueType parser/loader** | `0x4203f520` | `0x42040b50`–`0x42040c3c` | every SFNT table tag, the SFNT-variant reject strings, the table-directory and arena errors |
| **TTF runtime manager** | `0x4207d8d4` (`entry a1, 0x110`) | `0x4207dfe4`–`0x4207e03c` | `reader.ttf`, CJK fallback, `Failed to load %s at %dpx`, runtime mutex, `ttfFontSize` |
| **Settings I/O** | JSON-settings region | ref at `0x4204189c` | `ttfFontName` / `ttfFontSize` persistence |
| **`TtfFontSelect` activity** | `0x420ba5f4`–`0x420bd748` | refs at `0x420ba620`, `0x420ba650` | the on-device font picker, default `reader.ttf` |

### The parser is one self-contained TrueType reader

The parser function's literal pool (read directly as data words — no
disassembly needed) names the exact set of tables it fetches:

```
0x42040bec  -> "glyf"      0x42040bfc -> "hhea"
0x42040bf0  -> "cmap"      0x42040c00 -> "hmtx"
0x42040bf4  -> "loca"      0x42040c04 -> "maxp"
0x42040bf8  -> "head"      0x42040c08 -> "kern"
```

Required core tables: **`head`, `maxp`, `loca`, `glyf`, `cmap`, `hhea`,
`hmtx`**; `kern` is optional (kerning). Notably **absent** from the fetch list:
`name`, `post`, `OS/2`, `CFF`, `GPOS`, `GSUB`. The `CFF`/`GPOS`/`OTTO`/`ttcf`/
`WOFF` strings in the binary are *detect-and-reject* labels, not tables it
parses. Combined with the error vocabulary, the parse path is:

1. Read the 12-byte SFNT header → `Failed to read TTF header in %s`.
2. Check the sfnt version (`0x00010000` / `'true'`); detect and **reject**
   `OTTO` (CFF), `ttcf` (collection), `WOFF`/`WOFF2` → `Not a TrueType font in %s`.
3. `malloc` the table directory, read `numTables × 16` records, with a cap →
   `malloc failed for table directory` / `Too many compact TTF tables`.
4. Locate the required tables → `Missing required TTF tables`, `No glyf table found`.
5. Pick a usable `cmap` subtable → `No suitable cmap subtable`.
6. Glyphs are loaded **lazily**: per glyph, the file is reopened and the `glyf`
   slice (located via `loca`) is read through a `"callback-read"` buffer, with a
   per-glyph size cap (`Glyph %d glyf too large`). Output goes into **growable
   arenas** (`Arena grow failed`, `Glyph storage grow failed`, `Interval storage
   grow failed`, `Failed to alloc %zu byte bitmap arena`), guarded by a
   per-glyph mutex plus a runtime mutex. There are no `FreeType`/`FT_` strings —
   this is a bespoke compact rasterizer, not FreeType.

This is functionally the same shape as CrossPoint's `SdCardFont` on-demand glyph
path (lazy reads + an overflow/arena buffer), but fed by a live TrueType parser
instead of a pre-baked `.cpfont`.

### How a `.ttf` actually gets accepted

This is the part the string list alone didn't make obvious. There is **no TTF
upload control on the web Fonts page** — `m4/binwalk_extracted/FontsPage.html`
only accepts `.epf` CJK packs (`accept=".epf"`, `POST /upload?path=/.mofei/fonts`).
`.ttf`/`.otf`/`.woff`/`.woff2`/`.sfnt` live only in the **generic file-upload
content-type table** (`0x000e8a0f`). So a user gets a `.ttf` onto the device by:

- the **generic file manager** upload (`/upload?path=…` into `/.mofei/fonts/`), or
- **USB MSC** — the device mounts as `MOFEI Storage`; drop files into `/.mofei/fonts/`.

The reader then **discovers** `*.TTF` under `/.mofei/fonts/` (`No TTF fonts
found` when empty), lists them in `TtfFontSelect`, and persists the choice as
`ttfFontName`. Special names: `reader.ttf` (default custom reader face),
`notosans_cjk_tc_fallback.ttf` (CJK fallback subset), plus a separately-loaded
UI face (`Failed to load UI TTF %s at %dpx`). `ttfFontSize` sets the ppem the
parser instantiates the face at — i.e. true arbitrary-size rendering, not the
fixed size-enum buckets the `.epf`/`.cpfont` packs use.

## String Evidence

Offsets below are file offsets in `murphy-26-0526-1.2.16.bin`.

### User-facing TTF UI and settings

```text
0x0000db19  " (TTF)"
0x00010bf9  " (TTF)"
0x00013b58  "Custom (TTF)"
0x00013b65  "TTF Font"
0x00013b6e  "No TTF fonts found"
0x000354f1  "ttfFontName"
0x0003580f  "ttfFontSize"
0x000381a9  "TtfFontSelect"
```

This is stronger than MIME-type support: the setting names and activity name
show a reader-facing TTF selection flow.

### TrueType parser and table loader

```text
0x00033944  "cmap"
0x00033949  "glyf"
0x0003394e  "hhea"
0x00033953  "hmtx"
0x00033958  "kern"
0x0003395d  "GPOS"
0x00033962  "CFF "
0x00033967  "maxp"
0x00033b6d  "OpenType/CFF (OTTO) "
0x00033b93  "TrueType Collection (ttcf) "
0x00033bc0  "WOFF "
0x00033c08  "Failed to read TTF header in %s: %d/%u bytes"
0x00033c36  "Not a TrueType font in %s "
0x00033c7f  "malloc failed for table directory"
0x00033ca2  "Failed to read TTF table directory in %s: %d/%d bytes"
0x00033cd9  "Too many compact TTF tables"
0x00033cf6  "No glyf table found"
0x00033d67  "Missing required TTF tables"
0x00033d84  "No suitable cmap subtable"
```

The loader explicitly rejects unsupported SFNT variants:

- OpenType/CFF (`OTTO`) is detected but "not supported".
- TrueType Collection (`ttcf`) is detected but "not supported".
- WOFF is detected but "not supported".

So the supported native format is plain TrueType outlines with `glyf` data,
not CFF-based OTF, TTC collections, or webfont containers.

### Glyph loading and raster/cache behavior

```text
0x000339f3  "Failed to reopen %s for %s glyf read glyph %d"
0x00033a22  "Failed to reopen %s after remount for %s glyf read glyph %d"
0x00033a5f  "Failed to %s glyf for glyph %d in %s at offset %u: %d/%u"
0x00033a99  "Glyph %d glyf too large: %d bytes"
0x00033abc  "Failed to alloc %d byte glyf callback buffer"
0x00033af8  "Arena grow failed: %zu "
0x00033b1e  "Glyph storage grow failed: %d "
0x00033b44  "Interval storage grow failed: %d "
0x00033bd7  "Failed to create glyph mutex"
0x00033d9f  "Failed to alloc glyf buffer"
0x00033dbc  "Failed to alloc %zu byte bitmap arena"
0x00033de3  "Failed to alloc glyph storage"
```

This points to a lazy/on-demand glyph path:

- `glyf` data can be reopened and read per glyph.
- Glyphs have a size cap.
- There is a callback-read buffer.
- There are growable glyph, interval, and bitmap arenas.
- There is a glyph mutex, so access is shared across tasks or renderer paths.

### Runtime TTF registration/fallback

```text
0x000365eb  "Failed to create TTF runtime mutex"
0x0003660f  ".TTF"
0x00036614  "/.mofei/fonts/notosans_cjk_tc_fallback.ttf"
0x0003663f  "Failed to load CJK fallback subset TTF: %s"
0x0003666b  "reader.ttf"
0x00036676  "Failed to load %s at %dpx (tried "
0x0003669e  "Failed to load UI TTF %s at %dpx (tried "
```

The loader has a global/runtime mutex distinct from the per-glyph mutex. It
also has a hardcoded CJK fallback path and a default filename `reader.ttf`.

The current best interpretation is:

- User-supplied reader fonts are discovered or selected from `/.mofei/fonts/`.
- `reader.ttf` is a default/expected custom reader font name.
- `notosans_cjk_tc_fallback.ttf` is a Traditional Chinese fallback subset.
- `ttfFontSize` controls the runtime point/pixel size used to instantiate the
  TTF-backed font.

### Web upload MIME support

```text
0x000e8a0f  ".ttf"
0x000e8a1f  "application/x-font-ttf"
0x000e8a3f  ".otf"
0x000e8a4f  "application/x-font-opentype"
0x000e8a6f  ".woff"
0x000e8a7f  "application/font-woff"
0x000e8a9f  ".woff2"
0x000e8aaf  "application/font-woff2"
0x000e8aff  ".sfnt"
0x000e8b0f  "application/font-sfnt"
```

These MIME strings alone would not prove native rendering support. Combined
with the parser and settings strings above, they show the web server will
serve/upload font-looking files while the reader code itself supports plain
TTF.

## EPF2 is separate

The `.epf` font page is for multilingual packs, not the same as native TTF.
The embedded web UI says:

```text
Upload generated .epf files for Noto Sans CJK.
Files will be stored under /.mofei/fonts/.
Expected filenames: notosans_tc_8.epf ... notosans_tc_18.epf.
```

Relevant strings:

```text
0x00035bb7  "TCFONT"
0x00035be8  "Failed to read header of %s"
0x00035c05  "Invalid font pack header in %s: magic=%c%c%c%c v=%d"
0x00035c3a  "EPF header declares %llu bytes but file is only %llu bytes in %s "
0x00035e71  "/.mofei/fonts/notosans_tc_8.epf"
0x00035ea2  "/.mofei/fonts/notosans_tc_10.epf"
0x00035ed4  "/.mofei/fonts/notosans_tc_12.epf"
0x00035f06  "/.mofei/fonts/notosans_tc_14.epf"
0x00035f38  "/.mofei/fonts/notosans_tc_16.epf"
0x00035f6a  "/.mofei/fonts/notosans_tc_18.epf"
0x0003859e  "EPF2"
0x00038fc8  "Uploaded file is not a valid EPF2 font pack"
```

EPF2 appears to be a prebuilt bitmap/metrics pack for fixed CJK sizes. The
TTF path is a runtime TrueType parser/rasterizer.

## Raw BIN path is also separate

The firmware has a third storage-backed optimization path for a specific raw
Chinese font bitmap:

```text
0x0003600e  "RAWFONT"
0x00036016  "/.mofei/fonts/use_raw_bin_reader_font"
0x0003603c  "/.mofei/fonts/xiaomi_tc_45x60.bin"
0x0003605e  "Invalid BIN font size path=%s size=%u expected=%u"
0x00036091  "Failed to allocate raw BIN glyph cache bytes=%u heapMax=%u psramMax=%u"
```

This is likely a fast path for a fixed-size Traditional Chinese reader font,
guarded by a sentinel file. It does not explain general TTF support.

## What this implies for CrossPoint

CrossPoint mainline converts TTF/OTF **offline** (`fontconvert_sdcard.py` +
freetype-py) into fixed-size `.cpfont` bitmap packs; there is no on-device
rasterizer. Murphy v1.2.16 adds a **runtime** TrueType parser/rasterizer on top
of the same renderer/settings surface, so users drop a plain `.ttf` in and pick
any `ttfFontSize`.

The good news for porting: Murphy reuses CrossPoint's exact glyph plumbing — a
lazy `glyphMissHandler` feeding `EpdGlyph` bitmaps into an overflow/arena buffer,
drawn by the renderer that already understands `is2Bit` glyphs. A TTF backend can
therefore slot in as a sibling of `SdCardFont` with **no renderer changes**. The
full design is written up in
[`porting_ttf_to_crosspoint.md`](porting_ttf_to_crosspoint.md).

## RE status

The earlier "next steps" are now done:

- ✅ Segment-corrected reference recovery (DROM mapping above) — replaces the
  raw `0x42000000` import; all TTF strings now resolve to functions.
- ✅ TTF loader entrypoint identified via the table-tag literal pool
  (parser `entry 0x4203f520`; runtime manager `entry 0x4207d8d4`).
- ✅ Settings (`ttfFontName`/`ttfFontSize`) and the `TtfFontSelect` activity
  located.
- ✅ Compatibility model captured: storage path `/.mofei/fonts/`, `*.TTF`
  filter, accepted SFNT `0x00010000`/`'true'`, rejected `OTTO`/`ttcf`/`WOFF`,
  required tables `head/maxp/loca/glyf/cmap/hhea/hmtx`.

Remaining (optional, needs hardware or deeper decode):

1. Confirm the rasterizer's output bit depth (1-bit vs 2-bit/4-level) and
   whether it antialiases — the renderer supports both via `is2Bit`.
2. On-device check: place `/.mofei/fonts/reader.ttf` and confirm it appears
   under `TtfFontSelect`.

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
   `TTF Font`, and direct parsing of TrueType tables such as `cmap`, `glyf`,
   `hhea`, `hmtx`, `kern`, `GPOS`, and `maxp`.

The native TTF path appears to be a custom compact TrueType rasterizer/loader,
not FreeType. The binary has no obvious `FreeType`/`FT_` strings, but it has
many bespoke parse/read/allocation errors around TrueType table handling.

## Artifacts

Analyzed binary:

```text
murphyos/murphy-26-0526-1.2.16.bin
SHA-256: 3ee3d0a7207a17d49eb47fa60febff8cf4ac2f47bb52d74a1beceb40b8b124ea
```

Focused Ghidra/xref artifact:

```text
murphyos/font_ttf_pointer_refs.md
```

The raw import still does not recover usable references for these strings:
Ghidra reports them as strings but with no direct xrefs. This matches the
existing v1.2.16 import limitation documented in
`murphyos/murphy_reader_code_reuse.md`: the app was imported as a raw binary
at `0x42000000`, not as a parsed ESP32-S3 app image with segment mappings.

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

CrossPoint mainline converts TTF/OTF at build time or into `.cpfont` packs.
Murphy v1.2.16 goes further: it added a runtime TTF implementation on top of
the CrossPoint renderer/settings surface.

Practical porting implication:

- To match Murphy's "out of the box" custom-font UX, CrossPoint would need a
  native TTF loader/rasterizer or a compatible substitute.
- A faithful implementation probably needs only plain TrueType `glyf` support
  at first. CFF/OTF, TTC, WOFF, and WOFF2 can stay unsupported, matching
  Murphy.
- The loader should be designed around lazy glyph reads and per-page caches,
  not full-font rasterization into RAM.

## Next RE steps

1. Re-import `murphy-26-0526-1.2.16.bin` with ESP32-S3 segment parsing instead
   of raw `0x42000000`, then rerun string xrefs. Current raw import does not
   recover references for the TTF strings.
2. Recover the runtime structs around the TTF settings by tracking
   `ttfFontName` and `ttfFontSize` in `JsonSettingsIO`-like code.
3. Identify the TTF loader entrypoint by searching for the table tags
   `cmap`, `glyf`, `hhea`, `hmtx`, `maxp` as immediate constants and
   decompile nearby functions.
4. Build a small compatibility model: expected storage path(s), filename
   filtering, accepted SFNT header (`0x00010000` or `true`), and cmap subtable
   preference.
5. Test on-device, if available, by placing `/.mofei/fonts/reader.ttf` and
   observing whether it appears under `TTF Font` / `TtfFontSelect`.

# Murphy Reader v1.2.16 contains code derived from `crosspoint-reader-main`

## Related findings

- [Upstream MoFei firmware](upstream_mofei_firmware.md) — the corogoo Gitee project that ships the *other* firmware track Murphy M3 hardware actually runs (`touch/firmware.bin`). Murphy Reader at pandacat.ai is a separate fork that diverged from MoFei before incorporating crosspoint.
- [OEM touch v525 grayscale LUTs](oem_touch_v525_grayscale_luts.md) — LUT extraction from the MoFei `touch` build. Does **not** apply to Murphy Reader v1.2.16, whose display driver came from crosspoint.

## Summary

The current Murphy Reader firmware (`v1.2.16`, distributed at
`http://murphy.pandacat.ai/ota/latest`) contains string evidence that proves
it is built from — or has directly vendored substantial code from —
the `crosspoint-reader-main` codebase. The evidence is not limited to the public KOReader-Sync /
OPDS wire protocol strings that any independent implementation would
necessarily share; it includes the project name "CrossPoint" itself,
a class name `CrossPointWebServer` from the source tree, the legacy XOR
obfuscation key documented in `WifiCredentialStore.cpp`, and a number of
log messages whose exact prose exists only in the crosspoint repository.

The earlier `firmware.bin` analyzed from
`gitee.com/corogoo/3.7-inch-ink-screen-reader` (the original "MoFei" base)
is a separate, older firmware and does **not** contain any of these
strings. Murphy Reader is a downstream fork that swapped in
crosspoint-derived code at some point between the MoFei base and
v1.2.16.

## Artifacts analyzed

Both saved under `analysis/upstream_murphy_reader/`:

| File | Size | MD5 | SHA-256 |
|---|---|---|---|
| `murphy-26-0526-1.2.16.bin` | 3,903,024 | `05b8e2982420f319d94e7491dbbeb3d7` | `3ee3d0a7207a17d49eb47fa60febff8cf4ac2f47bb52d74a1beceb40b8b124ea` |
| `mofei-corogoo-EPD426-v1.bin` | 5,626,704 | `4821b423e69d72d09b43027a00ca34ba` | `e932fbc697339f3b1e17adcb6a77e11ba50a1ec2297c20a4fbf30a667759fa50` |

Source of `murphy-26-0526-1.2.16.bin`:
`http://murphy.pandacat.ai/firmware/murphy-26-0526-1.2.16.bin`, advertised
by the OTA endpoint `http://murphy.pandacat.ai/ota/latest`:

```
{
  "name": "Murphy Reader", "version": "1.2.16",
  "build_date": "2026-05-26", "chip_family": "ESP32-S3",
  "firmware_url": "http://murphy.pandacat.ai/firmware/murphy-26-0526-1.2.16.bin",
  "firmware_size": 3903024, "app_offset": 131072, "force_update": false
}
```

The PDF user-guide for this build (`Murphy Reader — First-time Use Guide`)
advertises "KOReader Sync: Synchronize reading progress", "OPDS online
library browsing", and "Calibre Wireless Sync" — features present in
`crosspoint-reader-main` and absent from the MoFei base.

App-image header data:

- Project name `arduino-lib-builder` (same as crosspoint and MoFei)
- ESP-IDF `v5.5.4`, build `Mar 31 2026 11:43:43`, commit prefix `14a0af9`
- ESP32-S3, app entry `0x403c98d0`-style image

## Smoking-gun strings

These three are not part of any public protocol. They originate in
`crosspoint-reader-main` and are present verbatim in the Murphy Reader
binary.

### 1. Class name `CrossPointWebServer` shipped in the activity list

At offset `0x00036f2c` of `murphy-26-0526-1.2.16.bin`, a list of activity
/ task names contains:

```
...HttpRequest.Boot.AutoSleep.WeatherClock.RecentBooks.FileBrowser.CrossPointWebServer.OpdsBookBrowser...
```

`CrossPointWebServer` is the network class from
`src/network/CrossPointWebServer.cpp` in the crosspoint repository. An
independent re-implementation of the same feature would not name the
class after a different project. No occurrence of this string exists in
the MoFei base binary.

### 2. The legacy XOR migration key

At offset `0x001645f4`:

```
CrossPoint/.mofei/wifi.bin.bak./.mofei/wifi.json./.mofei/wifi.bin
```

`WifiCredentialStore.cpp:21` in crosspoint:

```cpp
// Legacy obfuscation key - "CrossPoint" in ASCII (only used for binary migration)
```

Murphy Reader ships the string `"CrossPoint"` as a *literal byte-level
XOR key*, immediately adjacent to its own MoFei wifi-credential filenames.
The only reason to ship that key is to decrypt files originally written
by crosspoint firmware — i.e., to migrate ex-crosspoint user data into
MoFei storage. That migration code path was lifted from crosspoint
verbatim.

### 3. Distinctive log strings with crosspoint's tag conventions

At offset `0x000337d8`, contiguous in the binary:

```
Final XML parse error: %s..KOX.Failed to create XML parser../body/DocFragment[
.]/body./text()..KRS.Failed to save KOReader credentials during migration..KODoc...
```

The tags `KOX`, `KRS`, and `KODoc` are crosspoint's `LOG_ERR(...)` tag
literals:

- `lib/KOReaderSync/ChapterXPathResolver.cpp:104` —
  `LOG_ERR("KOX", "Final XML parse error: %s", ...)` (and lines 120, 224, 243, 353, 372)
- `lib/KOReaderSync/KOReaderCredentialStore.cpp:65` —
  `LOG_ERR("KRS", "Failed to save KOReader credentials during migration")`
- `lib/KOReaderSync/KOReaderDocumentId.cpp:47` —
  `LOG_DBG("KODoc", "Failed to open file: %s", ...)`

The exact prose `"Failed to save KOReader credentials during migration"`
exists at exactly one place in the world: that single source line.
The packing in the binary preserves the tag/format pairing produced by
crosspoint's `LOG_ERR` macro.

### 4. Storage filename scheme is identical (only the parent dir was rebranded)

| Crosspoint path | Murphy Reader path |
|---|---|
| `/.crosspoint/koreader.bin` | `/.mofei/koreader.bin` |
| `/.crosspoint/koreader.bin.bak` | `/.mofei/koreader.bin.bak` |
| `/.crosspoint/koreader.json` | `/.mofei/koreader.json` |
| `/.crosspoint/opds.json` | `/.mofei/opds.json` |
| `/.crosspoint/wifi.bin` | `/.mofei/wifi.bin` |
| `/.crosspoint/wifi.bin.bak` | `/.mofei/wifi.bin.bak` |
| `/.crosspoint/wifi.json` | `/.mofei/wifi.json` |

Three-file layouts (`name.bin` + `name.bin.bak` + `name.json`) for both
KO credentials and wifi credentials are a crosspoint-specific convention,
not a KOReader/OPDS protocol requirement.

## Full string-match table

190 distinct ≥8-character string literals were extracted from
`lib/KOReaderSync`, `lib/OpdsParser`, and `src/OpdsServerStore.cpp` in
crosspoint. 32 of them appear verbatim in `murphy-26-0526-1.2.16.bin`,
at the offsets shown:

| Murphy bin offset | String | Category |
|---|---|---|
| `0x00036f2c` | `CrossPoint` | crosspoint identity |
| `0x000337d8` | `Failed to save KOReader credentials during migration` | crosspoint prose |
| `0x00033794` | `Failed to create XML parser` | crosspoint prose |
| `0x0000c3a0` | `Couldn't allocate memory for parser` | crosspoint prose |
| `0x0000c3c5` | `Couldn't allocate memory for buffer` | crosspoint prose |
| `0x00033775` | `Final XML parse error: %s` | crosspoint prose |
| `0x0003377b` | `XML parse error: %s` | crosspoint prose |
| `0x000358e4` | `JSON parse error: %s` | crosspoint prose |
| `0x00034f3b` | `JSON parse failed: %s` | crosspoint prose |
| `0x00007985` | `Failed to open file: %s` | crosspoint prose |
| `0x0003193a` | `No credentials configured` | crosspoint prose |
| `0x000337b1` | `/body/DocFragment[` | crosspoint XPath pattern |
| `0x00014645` | `OPDS Server` | crosspoint UI label |
| `0x00056597` | `(default)` | crosspoint UI label |
| `0x000312be` | `Filename` | crosspoint field name |
| `0x0003586e` | `matchMethod` | crosspoint JSON field |
| `0x00035864` | `serverUrl` | crosspoint JSON field |
| `0x00035823` | `username` | crosspoint JSON field |
| `0x0003582c` | `password_obf` | crosspoint JSON field |
| `0x000084f1` | `document` | crosspoint JSON field |
| `0x00014ade` | `progress` | crosspoint JSON field |
| `0x000338ec` | `percentage` | crosspoint JSON field |
| `0x0000d79e` | `KOReader` | label |
| `0x000d1dc8` | `https://sync.koreader.rocks:443` | crosspoint default URL form |
| `0x000338d4` | `/syncs/progress` | KOReader-Sync protocol |
| `0x000338d4` | `/syncs/progress/` | KOReader-Sync protocol |
| `0x000338c8` | `/users/auth` | KOReader-Sync protocol |
| `0x000338aa` | `x-auth-user` | KOReader-Sync protocol |
| `0x000338b6` | `x-auth-key` | KOReader-Sync protocol |
| `0x00033889` | `application/vnd.koreader.v1+json` | KOReader-Sync MIME |
| `0x00033928` | `application/json` | standard MIME |
| `0x0003391b` | `Content-Type` | standard HTTP |

The "KOReader-Sync protocol" and "standard HTTP/MIME" rows could occur in
any independent implementation. Everything in the "crosspoint *"
categories — about 19 of the 32 — could not.

## What is *not* in the binary

Several crosspoint identifiers did not survive — most likely stripped by
a release build with logging compiled out, or renamed in a fork:

- Class/file names: `KOReaderSyncClient`, `OpdsParser`, `OpdsStream`,
  `ChapterXPathResolver`, `ProgressMapper`
- Format-string prose with structured fields: `"Authenticating: %s
  (heap: %u)"`, `"Auth response: %d (err: %d)"`, `"-> KO: spine=%d
  page=%d/%d %.2f%% %s"`, `"<- KO: %.2f%% %s -> spine=%d page=%d/%d"`,
  `"Added server: %s"`
- Path prefix: `/.crosspoint` (replaced by `/.mofei` while keeping the
  filenames)

This pattern — log strings stripped, identifier strings rebranded,
filenames and protocol/error strings preserved — is consistent with a
fork where the source was forked, the directory prefix renamed for
branding, and the build flags switched to a smaller release profile.
It is not consistent with an independent reimplementation, because an
independent reimplementation would not need a migration path that XORs
with the literal string `"CrossPoint"`.

## Reproduction

The matches can be regenerated with:

```python
import re, os
data = open('analysis/upstream_murphy_reader/murphy-26-0526-1.2.16.bin','rb').read()
files = []
for root in ['/path/to/crosspoint-reader-main/lib/KOReaderSync',
             '/path/to/crosspoint-reader-main/lib/OpdsParser']:
    for dp,_,fs in os.walk(root):
        for f in fs:
            if f.endswith(('.cpp','.h','.c')):
                files.append(os.path.join(dp,f))
files.append('/path/to/crosspoint-reader-main/src/OpdsServerStore.cpp')

strings = set()
str_re = re.compile(r'"((?:[^"\\]|\\.){8,})"')
for fp in files:
    txt = open(fp,'r',errors='ignore').read()
    for m in str_re.finditer(txt):
        strings.add(m.group(1).encode('utf-8','ignore').decode('unicode_escape'))

for s in sorted(strings):
    if s.encode('utf-8','ignore') in data:
        print(s)
```

## Wide net: matches across the entire crosspoint codebase

The KO/OPDS pass was narrow. Repeating the string match against **all**
`.cpp/.h/.c` files in `crosspoint-reader-main/lib` and `src` (excluding
vendored third-party trees like expat/zlib/freetype/mbedtls/lwip) yields:

- **7,465** distinct ≥8-character string literals in the crosspoint
  source tree (across 296 files).
- **535** of them appear verbatim in `murphy-26-0526-1.2.16.bin`.
- Distributed across **111 source files** in 60+ subsystems.

Per-file scorecard (files with ≥2 distinct hits):

| File | hits | total strings |
|---|---:|---:|
| `src/network/CrossPointWebServer.cpp` | 71 | 363 |
| `lib/Epub/Epub.cpp` | 48 | 121 |
| `src/network/WebDAVHandler.cpp` | 43 | 123 |
| `src/SettingsList.h` | 30 | 46 |
| `lib/ZipFile/ZipFile.cpp` | 20 | 22 |
| `lib/Epub/Epub/htmlEntities.cpp` | 20 | 22 |
| `src/JsonSettingsIO.cpp` | 19 | 63 |
| `lib/Epub/Epub/css/CssParser.cpp` | 18 | 42 |
| `lib/Epub/Epub/parsers/ChapterHtmlSlimParser.cpp` | 14 | 76 |
| `lib/Epub/Epub/parsers/ContentOpfParser.cpp` | 14 | 58 |
| `lib/KOReaderSync/KOReaderSyncClient.cpp` | 14 | 42 |
| `src/activities/ActivityManager.cpp` | 14 | 36 |
| `lib/PngToBmpConverter/PngToBmpConverter.cpp` | 14 | 24 |
| `lib/GfxRenderer/GfxRenderer.cpp` | 10 | 27 |
| `lib/Epub/Epub/converters/PngToFramebufferConverter.cpp` | 10 | 19 |
| `lib/Epub/Epub/BookMetadataCache.cpp` | 9 | 29 |
| `lib/Epub/Epub/Section.cpp` | 9 | 25 |
| `lib/Xtc/Xtc.cpp` | 8 | 27 |
| `lib/Epub/Epub/converters/JpegToFramebufferConverter.cpp` | 8 | 20 |
| `lib/Epub/Epub/blocks/ImageBlock.cpp` | 8 | 19 |
| `lib/Epub/Epub/parsers/ContainerParser.cpp` | 8 | 10 |
| `src/activities/boot_sleep/SleepActivity.cpp` | 7 | 33 |
| `src/activities/network/CrossPointWebServerActivity.cpp` | 6 | 48 |
| `lib/Txt/Txt.cpp` | 5 | 28 |
| `lib/hal/HalSystem.cpp` | 5 | 23 |
| `lib/KOReaderSync/KOReaderCredentialStore.cpp` | 5 | 22 |
| `src/network/OtaUpdater.cpp` | 5 | 21 |
| `src/network/HttpDownloader.cpp` | 5 | 19 |
| `src/components/themes/BaseTheme.cpp` | 5 | 12 |
| `lib/KOReaderSync/KOReaderJsonIO.cpp` | 5 | 12 |
| `lib/KOReaderSync/ChapterXPathResolver.cpp` | 4 | 28 |
| `lib/JpegToBmpConverter/JpegToBmpConverter.cpp` | 4 | 21 |
| `src/activities/settings/OpdsSettingsActivity.cpp` | 4 | 12 |
| `lib/Epub/Epub/Page.cpp` | 4 | 6 |
| `src/main.cpp` | 3 | 39 |
| `src/activities/network/WifiSelectionActivity.cpp` | 3 | 35 |
| `src/activities/reader/KOReaderSyncActivity.cpp` | 3 | 32 |
| `lib/OpdsParser/OpdsParser.cpp` | 3 | 26 |
| `src/activities/settings/ClearCacheActivity.cpp` | 3 | 19 |
| `src/CrossPointSettings.cpp` | 3 | 17 |
| `lib/EpdFont/FontDecompressor.cpp` | 3 | 14 |
| `src/util/ScreenshotUtil.cpp` | 3 | 11 |
| `src/activities/util/BmpViewerActivity.cpp` | 3 | 10 |
| `src/activities/reader/ReaderActivity.cpp` | 3 | 8 |
| `src/activities/network/CrossPointWebServerActivity.h` | 3 | 7 |
| `src/activities/network/NetworkModeSelectionActivity.h` | 3 | 6 |
| `src/activities/reader/EpubReaderActivity.cpp` | 2 | 60 |
| `src/activities/settings/FontDownloadActivity.cpp` | 2 | 51 |
| `src/activities/home/FileBrowserActivity.cpp` | 2 | 33 |
| `lib/Epub/Epub/parsers/TocNavParser.cpp` | 2 | 20 |
| `src/activities/reader/XtcReaderActivity.cpp` | 2 | 18 |
| `src/WifiCredentialStore.cpp` | 2 | 17 |
| `lib/Epub/Epub/parsers/TocNcxParser.cpp` | 2 | 14 |
| `lib/Serialization/ObfuscationUtils.cpp` | 2 | 10 |
| `src/activities/settings/OpdsServerListActivity.cpp` | 2 | 9 |
| `src/CrossPointState.cpp` | 2 | 9 |
| `src/activities/settings/FontSelectionActivity.cpp` | 2 | 7 |
| `lib/EpdFont/SdCardFontManager.cpp` | 2 | 7 |
| `src/activities/util/KeyboardEntryActivity.h` | 2 | 5 |
| `lib/Epub/Epub/converters/PixelCache.h` | 2 | 5 |
| `lib/Epub/Epub/converters/ImageToFramebufferDecoder.cpp` | 2 | 3 |

This is not localized to the KO/OPDS subsystems. It spans:

- **HTTP & web stack** (`CrossPointWebServer`, `WebDAVHandler`,
  `HttpDownloader`, `OtaUpdater`)
- **The full EPUB engine** (`Epub.cpp`, `Section`, `Page`,
  `BookMetadataCache`, `ChapterHtmlSlimParser`, `ContentOpfParser`,
  `ContainerParser`, `TocNavParser`, `TocNcxParser`, `htmlEntities`,
  `CssParser`, `ImageBlock`)
- **Image pipeline** (`PngToBmpConverter`,
  `PngToFramebufferConverter`, `JpegToBmpConverter`,
  `JpegToFramebufferConverter`, `ImageToFramebufferDecoder`,
  `PixelCache`)
- **Other crosspoint-specific readers/formats** (`Txt`, `Xtc`, `ZipFile`)
- **The activity framework** (`ActivityManager`, dozens of
  `*Activity.cpp` files), including `src/activities/reader/`,
  `src/activities/network/`, `src/activities/settings/`
- **Settings/state persistence** (`SettingsList.h`, `JsonSettingsIO`,
  `CrossPointSettings`, `CrossPointState`, `WifiCredentialStore`,
  `ObfuscationUtils`)
- **Rendering** (`GfxRenderer`, `BaseTheme`)
- **HAL** (`HalSystem`)
- **Font system** (`FontDecompressor`, `SdCardFontManager`)

## Storage scheme is identical 1:1

Murphy v1.2.16 uses the exact triple-file persistence scheme
(`name.bin` + `name.bin.bak` + `name.json`) for every persistent store
in crosspoint, with only the directory prefix changed from
`/.crosspoint` to `/.mofei`:

| Crosspoint path (from grep of source) | Murphy path (from strings of bin) |
|---|---|
| `/.crosspoint/` | `/.mofei/` |
| `/.crosspoint/koreader.bin{,.bak}` + `.json` | `/.mofei/koreader.bin{,.bak}` + `.json` |
| `/.crosspoint/opds.json` | `/.mofei/opds.json` |
| `/.crosspoint/recent.bin{,.bak}` + `.json` | `/.mofei/recent.bin{,.bak}` + `.json` |
| `/.crosspoint/settings.bin{,.bak}` + `.json` | `/.mofei/settings.bin{,.bak}` + `.json` |
| `/.crosspoint/state.bin{,.bak}` + `.json` | `/.mofei/state.bin{,.bak}` + `.json` |
| `/.crosspoint/wifi.bin{,.bak}` + `.json` | `/.mofei/wifi.bin{,.bak}` + `.json` |
| `/.crosspoint/language.bin{,.bak}` | `/.mofei/language.bin` |
| `/.crosspoint/epub_` | `/.mofei/epub_` |

Murphy added new feature paths on top (`arcade/`, `dashboard/`, `study/`,
`sleep/wordcloud.*`, `bookmarks`, `highlights`, `e2e`,
`reading/read_later`) — these correspond to the user-guide features that
weren't in crosspoint (flashcards, games, weather/clock dashboard, lock
screen wordcloud). The presence of `/.mofei/e2e` even preserves the
end-to-end-test directory hint from the crosspoint test setup.

## Log-tag conventions are crosspoint's

52 of the 76 distinct `LOG_xxx("TAG", ...)` tags used in
crosspoint-reader-main appear as isolated tokens in the v1.2.16 binary.
The distinctive ones — not generic three-letter words — are clearly
crosspoint's abbreviation choices:

- `KOX`, `KRS`, `KODoc`, `KOSync` (KOReader subsystem)
- `OPDS`, `DAV` (OPDS + WebDAV)
- `XTC`, `XTR`, `XTB` (crosspoint's XTC reader format)
- `EPB`, `EHP`, `EBP` (EPUB-internal)
- `BMC`, `CPS`, `WCS`, `CTR`, `OBF`, `PGE`, `FDC`, `RBS`, `RBA`,
  `SCT`, `TRS`, `TXB`, `WEBACT`, `CLEAR_CACHE`

Top tag frequencies in v1.2.16: `WiFi`×156, `OPDS`×50, `HTTP`×20,
`TOC`×12, `CSS`×11, `PNG`×10, `OTA`×9, `CTR`×9.

## Ghidra import

The v1.2.16 binary has been imported into the existing
`analysis/ghidra-project/MurphyM3` project with Xtensa LE 32 at base
`0x42000000` (raw-binary loader; the ESP32-S3 app-image segment header
was not parsed). Autoanalysis completed in ~32s.

```text
analysis/upstream_murphy_reader/
├── ghidra_logs/import.log
├── ghidra_logs/xref.log
├── fn_string_refs.tsv           (preliminary, see note)
└── (binaries)
```

Note: the raw-binary loader auto-defined only ~17 strings as `Data`
objects, so `fn_string_refs.tsv` is currently sparse. For a thorough
function-level XREF dump, the next step is to run a strings analyzer
(`StringsAnalyzer`) over the program to define more `Data` objects,
then re-export. That would let us attribute specific byte offsets
(e.g., the contiguous `KOX`/`KRS`/`KODoc` block at `0x000337d8`) to
specific Xtensa functions, and from there to a specific crosspoint
function — but the string-level evidence already establishes the
provenance unambiguously.

## Conclusion

Murphy Reader v1.2.16 is not "inspired by" or "compatible with"
crosspoint-reader-main. It is a fork that vendored the crosspoint
source tree, swapped the storage directory prefix (`/.crosspoint` →
`/.mofei`), kept literally the byte string `"CrossPoint"` as the XOR
migration key so it can read ex-crosspoint user data, retained the
crosspoint class name `CrossPointWebServer` as a task/activity
identifier, and built additional features (flashcards, games,
dashboard, lock-screen wordcloud) on top of the crosspoint foundation.
The vendored scope spans at least 111 source files across the entire
codebase — not a narrow library reuse.

## 2026-05-27 live OTA verification and extraction

The live OTA endpoint still advertises the same build on 2026-05-27:

```json
{
  "name": "Murphy Reader",
  "version": "1.2.16",
  "build_date": "2026-05-26",
  "chip_family": "ESP32-S3",
  "firmware_url": "http://murphy.pandacat.ai/firmware/murphy-26-0526-1.2.16.bin",
  "firmware_size": 3903024,
  "app_offset": 131072,
  "force_update": false
}
```

The locally saved file is byte-identical to the live firmware:

```text
3ee3d0a7207a17d49eb47fa60febff8cf4ac2f47bb52d74a1beceb40b8b124ea  /tmp/murphy-26-0526-1.2.16.bin
3ee3d0a7207a17d49eb47fa60febff8cf4ac2f47bb52d74a1beceb40b8b124ea  analysis/upstream_murphy_reader/murphy-26-0526-1.2.16.bin
```

A reproducible whole-tree source-string matcher was added at
`tools/crosspoint_firmware_string_match.py`. Running it against
`/Users/jmitch/GitHub/crosspoint-reader-main` and the verified Murphy
firmware produced
`analysis/upstream_murphy_reader/crosspoint_string_matches.tsv`:

- 684 exact firmware hits.
- 488 unique first-party crosspoint strings.
- 105 crosspoint source files represented.
- Top hit groups: `src/network` 216, `lib/Epub` 131,
  `src/activities` 104, `lib/KOReaderSync` 39.

Top source files by exact hit count in the current run:

| hits | source file |
|---:|---|
| 103 | `src/network/CrossPointWebServer.cpp` |
| 93 | `src/network/WebDAVHandler.cpp` |
| 33 | `src/SettingsList.h` |
| 28 | `lib/KOReaderSync/KOReaderSyncClient.cpp` |
| 25 | `src/JsonSettingsIO.cpp` |
| 24 | `lib/ZipFile/ZipFile.cpp` |
| 18 | `lib/Epub/Epub/css/CssParser.cpp` |
| 18 | `lib/Epub/Epub/parsers/ContentOpfParser.cpp` |
| 14 | `lib/PngToBmpConverter/PngToBmpConverter.cpp` |
| 14 | `lib/Xtc/Xtc/XtcTypes.h` |
| 13 | `lib/Epub/Epub/converters/PngToFramebufferConverter.cpp` |
| 13 | `src/activities/ActivityManager.cpp` |
| 12 | `lib/Epub/Epub/parsers/ChapterHtmlSlimParser.cpp` |
| 12 | `src/network/HttpDownloader.cpp` |

`binwalk -e` found five embedded gzip payloads. Named copies were saved
under `analysis/upstream_murphy_reader/binwalk_extract/`:

| firmware offset | recovered asset | notes |
|---:|---|---|
| `0x17626c` | `jszip.min.js` | JSZip 3.10.1 |
| `0x17d147` | `HomePage.html` | Murphy-branded web UI |
| `0x17d792` | `FilesPage.html` | Murphy-branded web UI; 2744/3106 long crosspoint `FilesPage.html` line fragments still match |
| `0x187f77` | `SettingsPage.html` | Murphy-branded web UI; 260/402 long crosspoint `SettingsPage.html` line fragments still match |
| `0x189229` | `FontsPage.html` | Murphy-branded web UI |

Ghidra headless analysis was rerun for the verified image and exported
`analysis/upstream_murphy_reader/ghidra_inventory_v1.2.16.txt`. The raw
import is useful for RE navigation, but it is not a source-code dump:
the current raw-loader import still yields sparse function/string XREFs
(`fn_string_refs_v1.2.16.tsv` contains only 16 XREF rows), so the
string-literal and embedded-asset evidence remains the stronger
reproducible result.

## Open questions / next steps

1. **License at incorporation time** — if crosspoint-reader-main is
   under a copyleft or attribution-required license, distribution of
   the Murphy Reader binary at `murphy.pandacat.ai` without
   corresponding source or credit may be a violation. The fact that
   the binary ships the crosspoint XOR migration key suggests they
   expected users to migrate from a crosspoint-based firmware,
   implying they were aware of the project at the time.
2. **Function-level binary diff** — compile crosspoint translation
   units (e.g., `CrossPointWebServer.cpp`, `Epub.cpp`,
   `KOReaderSyncClient.cpp`) with ESP-IDF v5.5.4 / Arduino-ESP32 and
   compare the resulting Xtensa machine code against the v1.2.16
   regions reachable from the matched string XREFs. A match at the
   instruction-encoding level (not just constant pools) is the
   stronger evidence if needed.
3. **Tracking which crosspoint commit was forked** — string deltas
   between this build and earlier crosspoint commits could date the
   fork point.

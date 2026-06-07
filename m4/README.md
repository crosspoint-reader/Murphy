# Murphy M4 (MurphyOS / Murphy Reader)

Reverse-engineering work on **Murphy M4**, whose firmware is **MurphyOS** —
the "Murphy Reader" build distributed at `http://murphy.pandacat.ai`. Unlike
the M3's OEM MoFei/corogoo firmware, MurphyOS is a downstream fork that
vendored substantial code from the `crosspoint-reader-main` codebase (OPDS,
KOReader Sync, Calibre wireless sync, native TTF fonts).

The analyzed image is the OTA app image `murphy-26-0526-1.2.16.bin`
(`v1.2.16`, ESP32-S3, build `Mar 31 2026`). No raw flash dump has been
captured yet — analysis so far is of the OTA binary.

## Start Here

- **CrossPoint code-reuse evidence:** [findings/murphy_reader_code_reuse.md](findings/murphy_reader_code_reuse.md)
- **Native TTF font support (RE):** [findings/murphy_reader_ttf_fonts.md](findings/murphy_reader_ttf_fonts.md)
- **Porting TTF support to CrossPoint:** [findings/porting_ttf_to_crosspoint.md](findings/porting_ttf_to_crosspoint.md)

## Layout (`m4/`)

- `murphy-26-0526-1.2.16.bin` — OTA app image (the MurphyOS / Murphy Reader v1.2.16 build).
- `findings/` — human-readable reverse-engineering notes:
  - `murphy_reader_code_reuse.md` — string-level proof the firmware derives from `crosspoint-reader-main`.
  - `murphy_reader_ttf_fonts.md` — native runtime `.ttf` font support (string + code-level RE).
  - `porting_ttf_to_crosspoint.md` — guide for adding runtime TTF rendering to upstream CrossPoint.
  - `font_ttf_pointer_refs.md` — early Ghidra/xref artifact (superseded by the segment-corrected analysis in `murphy_reader_ttf_fonts.md`).
- `analysis/` — Ghidra exports and string-match tables:
  - `crosspoint_string_matches.tsv` — whole-tree crosspoint↔firmware string matches.
  - `fn_string_refs_v1.2.16.tsv` (and the identical `fn_string_refs.tsv`) — function/string XREF export (sparse; raw-binary loader limitation).
  - `ghidra_inventory_v1.2.16.txt` — Ghidra program inventory.
  - `ghidra-logs/` — headless import/xref logs.
- `binwalk_extracted/` — embedded web-UI assets (`HomePage.html`, `FilesPage.html`, `SettingsPage.html`, `FontsPage.html`, `jszip.min.js`) and carved gzip payloads.

As an ESP32-S3 image, future work mirrors the M3 layout when a flash dump is
captured (`extracted/`, `ghidra-project/`, `probes/`).

## Relationship to M3

Murphy M3 runs the **OEM MoFei/corogoo** firmware (its own thing — see
[`../m3/`](../m3/README.md) and `../m3/oem_firmware/`). Murphy M4 / MurphyOS is
the separate CrossPoint-derived fork. The two share the on-device `/.mofei/`
storage directory scheme but have distinct code lineages. Shared tooling and
vendor reference live in [`../tools/`](../tools/) and [`../vendor/`](../vendor/).

# Ghidra Analysis

## Xtensa Support

Ghidra installed: `/opt/homebrew/Cellar/ghidra/12.1/libexec`

The GUI is not required for the current workflow. Use the headless runner:

`/opt/homebrew/opt/ghidra/libexec/support/analyzeHeadless`

The Homebrew Ghidra 12.1 installation did not include an Xtensa processor module. I cloned Ebiroll's `ghidra-xtensa` into:

`tools/ghidra-xtensa`

The repo's Gradle wrapper failed because it uses Gradle 7.3.3, while Ghidra 12.1 requires Gradle 8.5+. The SLEIGH processor compiled successfully when run directly without the conflict-reporting `-l` flag:

`/opt/homebrew/Cellar/ghidra/12.1/libexec/support/sleigh -u -n -t -e -f data/languages/xtensa.slaspec data/languages/xtensa.sla`

Installed processor path:

`/opt/homebrew/Cellar/ghidra/12.1/libexec/Ghidra/Processors/Xtensa`

Packaged extension zip:

`tools/ghidra-xtensa/dist/ghidra_12.1_PUBLIC_20260525_xtensa.zip`

## Project

Ghidra project:

`m3/ghidra-project/MurphyM3`

Language:

`Xtensa:LE:32:default`

The app image was imported as raw binary at base `0x0`, then `tools/ghidra_scripts/LoadEsp32S3Segments.java` mapped ESP32-S3 segments at their runtime addresses before analysis.

## Runtime Memory Blocks

From `m3/analysis/ghidra_inventory.md`:

| Block | Start | End | Access |
| --- | ---: | ---: | --- |
| raw app blob | `0x00000000` | `0x006cffff` | RWX |
| DROM | `0x3c190020` | `0x3c2a7a77` | R |
| DRAM | `0x3fc9a6b0` | `0x3fca1c5f` | RW |
| IRAM | `0x40374000` | `0x40374fdf` | RX |
| IRAM | `0x40374fe0` | `0x4038a6a3` | RX |
| IROM | `0x42000020` | `0x4218739b` | RX |
| RTC data | `0x50000000` | `0x50001c63` | RW |
| RTC DRAM/IRAM | `0x600fe000` | `0x600fe02b` | RW |

Entry label:

`esp32s3_entry = 0x40377b70`

Ghidra analysis completed in about 80 seconds. There are decompiler warnings from incomplete MMU/peripheral mappings and raw-blob duplicate analysis, but the Xtensa language loaded and the segment map is usable for manual work.

## Segment Artifacts

Segment files are under `m3/analysis/segments/`:

| Segment | Load address | Size |
| ---: | ---: | ---: |
| 0 | `0x3c190020` | `0x117a58` |
| 1 | `0x3fc9a6b0` | `0x075b0` |
| 2 | `0x40374000` | `0x00fe0` |
| 3 | `0x42000020` | `0x18737c` |
| 4 | `0x40374fe0` | `0x156c4` |
| 5 | `0x50000000` | `0x01c64` |
| 6 | `0x600fe000` | `0x0002c` |

## Export Scripts

Custom scripts in `tools/ghidra_scripts/`:

| Script | Purpose |
| --- | --- |
| `LoadEsp32S3Segments.java` | Map ESP32-S3 app-image segments at runtime addresses. |
| `DumpProgramInventory.java` | Export memory blocks, functions, symbols, and strings. |
| `FindStringRefs.java` | Find string matches and direct references. |
| `FindPointerRefs.java` | Find string matches, literal/pointer locations, and references to those pointers. Useful for Xtensa literal pools. |
| `DumpMemoryAt.java` | Dump bytes around selected Ghidra addresses for inspecting pointer tables and adjacent data. |
| `DecompileAt.java` | Decompile functions containing selected addresses. |
| `FindCallsiteContexts.java` | Search hardware API names/strings and dump references, nearby instructions, and decompile context. |
| `FindHardwareConstants.java` | Broad hardware keyword mining helper for stripped OEM code. |

Touch-specific exports:

- `m3/analysis/touch_string_refs.md`
- `m3/analysis/touch_pointer_refs.md`
- `m3/analysis/touch_decompile.md`

Front-light-specific exports:

- `m3/analysis/frontlight_string_refs.md`
- `m3/analysis/frontlight_pointer_refs.md`
- `m3/analysis/frontlight_table_memory.md`

Button/input-specific exports:

- `m3/analysis/button_combo_string_refs.md`
- `m3/analysis/button_combo_pointer_refs.md`
- `m3/analysis/button_combo_disassembly.md`
- `m3/analysis/button_combo_objdump_42000020.md`
- `m3/analysis/button_combo_objdump_keymap.md`

OEM hardware-mining exports:

- `m3/analysis/oem_hardware_calls.md`
- `m3/analysis/oem_symbol_table_disasm.md`
- `m3/analysis/oem_hardware_keyword_mining.md`

See `m3/findings/oem_headless_ghidra.md` for the exact headless commands and current limits of the static OEM hardware recovery.

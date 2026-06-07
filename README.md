# Murphy Reverse Engineering

Firmware dumps, extracted artifacts, reverse-engineering notes, and porting plans for the Murphy family of e-paper readers.

Work is organized per hardware generation. Each generation is self-contained, including the firmware it runs: **Murphy M3** runs the OEM MoFei/corogoo firmware, while **Murphy M4** is **MurphyOS** — the CrossPoint-derived "Murphy Reader" build.

## Repository Layout

- [`m3/`](m3/README.md) — **Murphy M3** (ESP32-S3). 16 MiB flash dump, carved partitions, segment/Ghidra analysis, per-subsystem findings, and Arduino hardware probes. Display, SD, buttons, and front light are ported and booting under CrossPoint. Includes the OEM MoFei/corogoo reference firmware and extracted display LUTs in [`m3/oem_firmware/`](m3/oem_firmware/).
- [`m4/`](m4/README.md) — **Murphy M4 / MurphyOS**: the "Murphy Reader" OTA build (`murphy-26-0526-1.2.16.bin`), a downstream fork that vendored `crosspoint-reader-main` code — CrossPoint code-reuse evidence, native TTF-font findings, and Ghidra/string-match exports.
- [`vendor/`](vendor/) — vendor reference material (git submodules): Elecrow CrowPanel ESP32 3.7" E-paper HMI, Corogoo 3.7" ink-screen reader, and Good Display panel datasheets.
- [`tools/`](tools/) — shared tooling: Ghidra loader/mining scripts and a local build of the Xtensa Ghidra processor plugin.

## Start Here

- **Murphy M3 overview:** [m3/README.md](m3/README.md)
- **M3 findings (per-subsystem):** [m3/findings/README.md](m3/findings/README.md)
- **M3 porting plan:** [m3/findings/porting_crosspoint.md](m3/findings/porting_crosspoint.md)
- **M3 OEM firmware identity (MoFei/corogoo):** [m3/findings/upstream_mofei_firmware.md](m3/findings/upstream_mofei_firmware.md)
- **Murphy M4 / MurphyOS overview:** [m4/README.md](m4/README.md)
- **MurphyOS ↔ CrossPoint code reuse:** [m4/findings/murphy_reader_code_reuse.md](m4/findings/murphy_reader_code_reuse.md)

## Hardware Snapshot

The M3 is an ESP32-S3-WROOM-1-N8R8-class board (8 MiB PSRAM, 16 MiB flash) with a UC8253-based Good Display `GDEY037T03-FT21` panel at 416x240, closely matching the Elecrow CrowPanel ESP32 3.7" E-paper HMI family. Full pin map, flash layout, and port status are in [m3/README.md](m3/README.md).

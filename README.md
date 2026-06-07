# Murphy Reverse Engineering

Firmware dumps, extracted artifacts, reverse-engineering notes, and porting plans for the Murphy family of e-paper readers and the shared **MurphyOS** reader firmware.

Work is organized per hardware generation (M3, M4) with cross-device firmware and reference material kept separate so each generation's bins and analysis stay self-contained.

## Repository Layout

- [`m3/`](m3/README.md) — **Murphy M3** (ESP32-S3). 16 MiB flash dump, carved partitions, segment/Ghidra analysis, per-subsystem findings, and Arduino hardware probes. Display, SD, buttons, and front light are ported and booting under CrossPoint.
- [`m4/`](m4/README.md) — **Murphy M4**. Placeholder; mirrors the M3 layout as artifacts are captured.
- [`murphyos/`](murphyos/) — **MurphyOS** reader firmware reverse engineering (cross-device): the upstream MoFei/Corogoo OTA images (`murphy-26-0526-1.2.16.bin` and friends), OEM display LUTs, CrossPoint code-reuse evidence, and TTF-font findings.
- [`vendor/`](vendor/) — vendor reference material (git submodules): Elecrow CrowPanel ESP32 3.7" E-paper HMI, Corogoo 3.7" ink-screen reader, and Good Display panel datasheets.
- [`tools/`](tools/) — shared tooling: Ghidra loader/mining scripts and a local build of the Xtensa Ghidra processor plugin.

## Start Here

- **Murphy M3 overview:** [m3/README.md](m3/README.md)
- **M3 findings (per-subsystem):** [m3/findings/README.md](m3/findings/README.md)
- **M3 porting plan:** [m3/findings/porting_crosspoint.md](m3/findings/porting_crosspoint.md)
- **MurphyOS firmware identity:** [murphyos/upstream_mofei_firmware.md](murphyos/upstream_mofei_firmware.md)
- **MurphyOS ↔ CrossPoint code reuse:** [murphyos/murphy_reader_code_reuse.md](murphyos/murphy_reader_code_reuse.md)

## Hardware Snapshot

The M3 is an ESP32-S3-WROOM-1-N8R8-class board (8 MiB PSRAM, 16 MiB flash) with a UC8253-based Good Display `GDEY037T03-FT21` panel at 416x240, closely matching the Elecrow CrowPanel ESP32 3.7" E-paper HMI family. Full pin map, flash layout, and port status are in [m3/README.md](m3/README.md).

# Murphy M4

Placeholder for Murphy M4 reverse-engineering work.

Drop M4 artifacts here as they are captured, mirroring the M3 layout:

- `m4_flash_dump.bin` — raw flash dump
- `extracted/` — carved partitions
- `binwalk_extracted/` — binwalk-style extraction
- `analysis/` — strings, segments, Ghidra logs, decompile notes
- `ghidra-project/` — Ghidra project for the app image
- `findings/` — human-readable per-subsystem notes
- `probes/` — standalone hardware probes

MurphyOS firmware (the reader OS, shared across devices) lives in [`../murphyos/`](../murphyos/),
and shared tooling/vendor reference live in [`../tools/`](../tools/) and [`../vendor/`](../vendor/).

#!/usr/bin/env python3
"""Match source string literals from crosspoint-reader-main against firmware.

This is a provenance helper, not a decompiler. It extracts C/C++ string
literals from first-party source files, folds adjacent literals, and records
byte offsets where each literal appears in a firmware blob.
"""

from __future__ import annotations

import argparse
import ast
import csv
import os
import re
from pathlib import Path


SOURCE_SUFFIXES = {".c", ".cc", ".cpp", ".cxx", ".h", ".hpp", ".ino"}
SKIP_PARTS = {
    ".git",
    ".pio",
    ".cache",
    "open-x4-sdk",
    "lib/expat",
    "lib/uzlib",
}


STRING_RE = re.compile(r'(?:"(?:\\.|[^"\\])*"\s*)+')


def should_skip(path: Path, root: Path) -> bool:
    rel = path.relative_to(root).as_posix()
    return any(part in rel.split("/") or rel.startswith(part + "/") for part in SKIP_PARTS)


def decode_string_group(group: str) -> str | None:
    parts = re.findall(r'"(?:\\.|[^"\\])*"', group, flags=re.S)
    decoded = []
    for part in parts:
        try:
            decoded.append(ast.literal_eval(part))
        except Exception:
            return None
    value = "".join(decoded)
    if "\x00" in value or len(value) < 8:
        return None
    return value


def iter_source_strings(root: Path):
    for dirpath, dirnames, filenames in os.walk(root):
        dirnames[:] = [
            d for d in dirnames
            if not should_skip(Path(dirpath) / d, root)
        ]
        for name in filenames:
            path = Path(dirpath) / name
            if path.suffix.lower() not in SOURCE_SUFFIXES or should_skip(path, root):
                continue
            try:
                text = path.read_text(errors="ignore")
            except OSError:
                continue
            for match in STRING_RE.finditer(text):
                value = decode_string_group(match.group(0))
                if value is None:
                    continue
                line = text.count("\n", 0, match.start()) + 1
                yield path.relative_to(root).as_posix(), line, value


def all_offsets(haystack: bytes, needle: bytes) -> list[int]:
    offsets = []
    start = 0
    while True:
        found = haystack.find(needle, start)
        if found < 0:
            return offsets
        offsets.append(found)
        start = found + 1


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--source-root", required=True, type=Path)
    parser.add_argument("--firmware", required=True, type=Path)
    parser.add_argument("--out", required=True, type=Path)
    parser.add_argument("--min-len", type=int, default=10)
    args = parser.parse_args()

    firmware = args.firmware.read_bytes()
    rows = []
    seen: set[tuple[str, str]] = set()

    for rel, line, value in iter_source_strings(args.source_root):
        if len(value) < args.min_len:
            continue
        key = (rel, value)
        if key in seen:
            continue
        seen.add(key)
        needle = value.encode("utf-8", errors="ignore")
        offsets = all_offsets(firmware, needle)
        for offset in offsets:
            rows.append({
                "firmware_offset": f"0x{offset:08x}",
                "source_file": rel,
                "source_line": line,
                "length": len(value),
                "string": value,
            })

    rows.sort(key=lambda r: (r["source_file"], int(r["source_line"]), r["string"]))
    args.out.parent.mkdir(parents=True, exist_ok=True)
    with args.out.open("w", newline="") as f:
        writer = csv.DictWriter(
            f,
            fieldnames=["firmware_offset", "source_file", "source_line", "length", "string"],
            dialect="excel-tab",
        )
        writer.writeheader()
        writer.writerows(rows)

    unique_strings = len({r["string"] for r in rows})
    unique_files = len({r["source_file"] for r in rows})
    print(f"wrote {len(rows)} matches ({unique_strings} unique strings from {unique_files} files) to {args.out}")


if __name__ == "__main__":
    main()

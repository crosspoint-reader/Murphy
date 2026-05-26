#!/usr/bin/env python3
"""Patch an ESP32-S3 app image inside a full flash dump and repair its footer.

This is intentionally small and conservative. It supports byte patches by app
file offset or loaded virtual address, plus same-length-or-shorter ASCII string
replacement for low-risk marker tests.
"""

from __future__ import annotations

import argparse
import hashlib
from pathlib import Path


IMAGE_MAGIC = 0xE9
INITIAL_CHECKSUM = 0xEF


class ImageLayout:
    def __init__(self, image: bytearray) -> None:
        if image[0] != IMAGE_MAGIC:
            raise ValueError(f"invalid ESP image magic 0x{image[0]:02x}")
        self.segment_count = image[1]
        self.segments: list[tuple[int, int, int]] = []
        off = 24
        for _ in range(self.segment_count):
            load_addr = int.from_bytes(image[off : off + 4], "little")
            size = int.from_bytes(image[off + 4 : off + 8], "little")
            data_off = off + 8
            self.segments.append((load_addr, data_off, size))
            off = data_off + size
        self.checksum_offset = off + ((15 - off) % 16)
        self.hash_offset = self.checksum_offset + 1

    def vaddr_to_file_offset(self, vaddr: int) -> int:
        for load_addr, data_off, size in self.segments:
            if load_addr <= vaddr < load_addr + size:
                return data_off + (vaddr - load_addr)
        raise ValueError(f"virtual address 0x{vaddr:x} is outside app segments")


def parse_hex_bytes(value: str) -> bytes:
    compact = value.replace(" ", "").replace(":", "")
    if len(compact) % 2:
        raise ValueError(f"odd number of hex digits in {value!r}")
    return bytes.fromhex(compact)


def apply_patch(image: bytearray, offset: int, data: bytes, label: str) -> None:
    if offset < 0 or offset + len(data) > len(image):
        raise ValueError(f"{label} patch at 0x{offset:x} is outside image")
    before = bytes(image[offset : offset + len(data)])
    image[offset : offset + len(data)] = data
    print(f"{label}: app+0x{offset:x} {before.hex()} -> {data.hex()}")


def replace_ascii(image: bytearray, old: bytes, new: bytes) -> None:
    if len(new) > len(old):
        raise ValueError("replacement string must not be longer than original")
    index = image.find(old)
    if index < 0:
        raise ValueError(f"ASCII string not found: {old!r}")
    if image.find(old, index + 1) >= 0:
        raise ValueError(f"ASCII string is not unique: {old!r}")
    replacement = new + (b"\x00" * (len(old) - len(new)))
    apply_patch(image, index, replacement, "replace-ascii")


def repair_footer(image: bytearray, layout: ImageLayout) -> None:
    checksum = INITIAL_CHECKSUM
    for _, data_off, size in layout.segments:
        for byte in image[data_off : data_off + size]:
            checksum ^= byte
    image[layout.checksum_offset] = checksum

    digest = hashlib.sha256(image[: layout.hash_offset]).digest()
    image[layout.hash_offset : layout.hash_offset + 32] = digest
    print(f"checksum @ app+0x{layout.checksum_offset:x}: 0x{checksum:02x}")
    print(f"sha256   @ app+0x{layout.hash_offset:x}: {digest.hex()}")


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--input", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--app-offset", default="0x10000")
    parser.add_argument(
        "--patch-app-offset",
        action="append",
        default=[],
        metavar="OFFSET:HEXBYTES",
    )
    parser.add_argument(
        "--patch-vaddr",
        action="append",
        default=[],
        metavar="VADDR:HEXBYTES",
    )
    parser.add_argument(
        "--replace-ascii",
        action="append",
        default=[],
        metavar="OLD=NEW",
    )
    args = parser.parse_args()

    full = bytearray(args.input.read_bytes())
    app_offset = int(args.app_offset, 0)
    app = bytearray(full[app_offset:])
    layout = ImageLayout(app)

    for spec in args.patch_app_offset:
        off_text, data_text = spec.split(":", 1)
        apply_patch(app, int(off_text, 0), parse_hex_bytes(data_text), "patch-app-offset")

    for spec in args.patch_vaddr:
        addr_text, data_text = spec.split(":", 1)
        app_file_off = layout.vaddr_to_file_offset(int(addr_text, 0))
        apply_patch(app, app_file_off, parse_hex_bytes(data_text), "patch-vaddr")

    for spec in args.replace_ascii:
        old_text, new_text = spec.split("=", 1)
        replace_ascii(app, old_text.encode(), new_text.encode())

    repair_footer(app, layout)
    full[app_offset : app_offset + len(app)] = app
    args.output.write_bytes(full)
    print(f"wrote {args.output}")


if __name__ == "__main__":
    main()

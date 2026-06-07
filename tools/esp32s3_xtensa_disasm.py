#!/usr/bin/env python3
"""Literal-aware Xtensa disassembler for ESP32-S3 app images.

Parses the .bin segment map, then disassembles a vaddr range with capstone's
Xtensa backend. Resolves `l32r` literal loads to their pointed-to value and, if
that value lands in the DROM string segment, annotates the referenced C string.
Marks `entry` prologues as function starts.

Usage: esp32s3_xtensa_disasm.py <image.bin> <start_vaddr_hex> <len_hex>
"""
import struct, sys
import capstone as cap

def load_segments(data):
    assert data[0] == 0xE9, "not an ESP app image"
    n = data[1]
    off = 24
    segs = []
    for i in range(n):
        load, length = struct.unpack_from("<II", data, off)
        off += 8
        segs.append((i, load, length, off))
        off += length
    return segs

def make_maps(segs):
    def v2f(v):
        for _, load, length, fo in segs:
            if load <= v < load + length:
                return fo + (v - load)
        return None
    return v2f

def cstr(data, v2f, v, maxlen=80):
    f = v2f(v)
    if f is None: return None
    end = data.find(b"\0", f, f + maxlen)
    if end < 0: return None
    try:
        s = data[f:end].decode("utf-8")
    except UnicodeDecodeError:
        return None
    if not s or not all(32 <= ord(c) < 127 or c in "\t" for c in s):
        return None
    return s

def disasm(image, start, length, annotate_strings=True):
    data = open(image, "rb").read()
    segs = load_segments(data)
    v2f = make_maps(segs)
    fo = v2f(start)
    if fo is None:
        print(f"// {start:#x} not in any segment"); return
    code = data[fo:fo + length]
    md = cap.Cs(cap.CS_ARCH_XTENSA, cap.CS_MODE_LITTLE_ENDIAN)
    md.detail = True
    for insn in md.disasm(code, start):
        line = f"{insn.address:#010x}:  {insn.mnemonic:<8} {insn.op_str}"
        ann = ""
        if insn.mnemonic == "entry":
            ann = "   ; ---- FUNCTION START ----"
        if insn.mnemonic == "l32r":
            # operand is the literal address; read the 4-byte literal
            parts = insn.op_str.split(",")
            if len(parts) == 2:
                tgt = parts[1].strip()
                try:
                    litaddr = int(tgt, 16) if tgt.startswith("0x") else int(tgt)
                    lf = v2f(litaddr)
                    if lf is not None:
                        val = struct.unpack_from("<I", data, lf)[0]
                        s = cstr(data, v2f, val) if annotate_strings else None
                        ann = f"   ; ={val:#x}" + (f'  "{s}"' if s else "")
                except ValueError:
                    pass
        print(line + ann)

if __name__ == "__main__":
    img, start, length = sys.argv[1], int(sys.argv[2], 16), int(sys.argv[3], 16)
    disasm(img, start, length)

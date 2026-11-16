#!/usr/bin/env python3
"""1080 Snowboarding asset extractor (PC port, M0 "extractor v0").

Reads the user's own ROM (baserom.z64, big-endian .z64 order) and writes an asset tree.
Standard library only. Nothing it writes may be committed (the default output dir is gitignored).

Format reference: docs/USO_FORMAT.md sections 2-7. Summary of what this tool relies on:
  - The kernel device table (k_gFsDevices, link 0x80009E60 = ROM 0xAE60) lists the ROM devices as
    0x14-byte entries [name ptr (runtime +0x400 address), romStart, romEnd, vram, 0], NULL-terminated.
  - A device is a stream of records: [u32 type, u32 size, u32 flag] + payload.
  - Each file starts with an Info record (type 0, size 0x2C): [0x12345678, s32 module id, 1, name[0x20]].
    A file's records follow up to End (type 11); the device ends with EndOfFile (type 10).
  - Bss (type 8) has no payload; its size is the Bss byte count.
  - flag & 0x10000: the payload is Yay0. `size` is then the COMPRESSED record length (the next record
    starts at payload + size, possibly byte-misaligned); the uncompressed size is in the Yay0 header.
  - BinaryDataSection (type 12) records hold the rom/*.bin files.

Output layout (under --out, default port/assets/extracted):
  fs/<device>/<file>            the file's raw record stream, exactly as on the ROM (Info .. End)
  uso/<file>/NN_<Type>.bin      one file per section, Yay0 decompressed, big-endian, unswapped
  bin/<file>                    the payload of a .bin file (its BinaryDataSection), as-is
  manifest.json                 every file and section: name, device, offsets, sizes, compression,
                                module id, SHA-1; plus the self-check results

Usage: extract.py [--rom baserom.z64] [--out DIR] [--no-write] [--skip-sha1]
Exit status 0 only when every self-check passes.
"""
import argparse
import hashlib
import json
import os
import struct
import sys

EXPECTED_SHA1 = "79cd1166c365e5809dec9b62e6d40d6032d5db3a"  # tenshoe.yaml (US 1.0)
DEVICE_TABLE_ROM = 0xAE60       # k_gFsDevices, link 0x80009E60
KERNEL_RUNTIME_TO_ROM = 0x7FFFF400  # runtime ptr (link + 0x400) -> ROM offset
INFO_MAGIC = 0x12345678

SECTION_NAMES = {
    0: "Info", 1: "Sym", 2: "TextReloc", 3: "DataReloc", 4: "RoDataReloc", 5: "Text",
    6: "Data", 7: "RoData", 8: "Bss", 9: "EntrySym", 10: "EndOfFile", 11: "End",
    12: "BinaryDataSection", 13: "DiskInfo",
}
RELOC_TARGET = {2: 5, 3: 6, 4: 7}   # reloc section type -> section it patches
FLAG_YAY0 = 0x10000


class ExtractError(Exception):
    pass


def u32(buf, off):
    return struct.unpack_from(">I", buf, off)[0]


def s32(buf, off):
    return struct.unpack_from(">i", buf, off)[0]


def yay0_decode(buf, off):
    """Decode the Yay0 block at buf[off]. Returns (data, compressed bytes consumed)."""
    if buf[off:off + 4] != b"Yay0":
        raise ExtractError(f"no Yay0 magic at 0x{off:X}")
    size, link_off, chunk_off = struct.unpack_from(">III", buf, off + 4)
    mask_p, link_p, chunk_p = off + 16, off + link_off, off + chunk_off
    out = bytearray(size)
    o = 0
    bits = 0
    mask = 0
    while o < size:
        if bits == 0:
            mask = u32(buf, mask_p)
            mask_p += 4
            bits = 32
        if mask & 0x80000000:
            out[o] = buf[chunk_p]
            chunk_p += 1
            o += 1
        else:
            v = (buf[link_p] << 8) | buf[link_p + 1]
            link_p += 2
            src = o - (v & 0xFFF) - 1
            n = v >> 12
            if n == 0:
                n = buf[chunk_p] + 0x12
                chunk_p += 1
            else:
                n += 2
            if src < 0 or o + n > size:
                raise ExtractError(f"corrupt Yay0 back-reference at 0x{off:X}")
            for _ in range(n):  # overlapping copies are byte-serial
                out[o] = out[src]
                o += 1
                src += 1
        mask = (mask << 1) & 0xFFFFFFFF
        bits -= 1
    return bytes(out), max(mask_p, link_p, chunk_p) - off


def read_devices(rom):
    devs = []
    p = DEVICE_TABLE_ROM
    while True:
        name_ptr, start, end, vram, extra = struct.unpack_from(">IIIII", rom, p)
        if name_ptr == 0:
            break
        no = name_ptr - KERNEL_RUNTIME_TO_ROM
        name = rom[no:no + 32].split(b"\0")[0].decode("ascii")
        devs.append({"name": name, "rom_start": start, "rom_end": end, "vram": vram})
        p += 0x14
    if [d["name"] for d in devs] != ["mainuso", "rom"]:
        raise ExtractError(f"unexpected device table: {devs}")
    return devs


def walk_device(rom, dev, problems):
    """Split one device into files. Each file: Info fields + list of records."""
    files = []
    o, end = dev["rom_start"], dev["rom_end"]
    cur = None
    while True:
        if o + 12 > end:
            raise ExtractError(f"{dev['name']}: ran off the device at 0x{o:X} without EndOfFile")
        rtype, size, flag = struct.unpack_from(">III", rom, o)
        if rtype not in SECTION_NAMES:
            raise ExtractError(f"{dev['name']}: bad record type {rtype} at 0x{o:X}")
        payload = o + 12
        if rtype == 10:  # EndOfFile
            tail = rom[payload:end]
            if cur is not None:
                problems.append(f"{dev['name']}: EndOfFile inside file {cur['name']}")
            if any(tail):
                problems.append(f"{dev['name']}: non-zero bytes after EndOfFile")
            return files, payload
        if rtype == 0:
            if size != 0x2C or u32(rom, payload) != INFO_MAGIC:
                raise ExtractError(f"{dev['name']}: bad Info record at 0x{o:X}")
            name = rom[payload + 12:payload + 12 + 0x20].split(b"\0")[0].decode("ascii")
            cur = {"name": name, "device": dev["name"], "module_id": s32(rom, payload + 4),
                   "info_word2": u32(rom, payload + 8), "rom_offset": o,
                   "device_offset": o - dev["rom_start"], "info_flag": flag, "records": []}
            files.append(cur)
            o = payload + size
            continue
        if cur is None:
            raise ExtractError(f"{dev['name']}: record before any Info at 0x{o:X}")
        rec = {"type": rtype, "name": SECTION_NAMES[rtype], "size": size, "flag": flag,
               "rom_offset": o, "compressed": bool(flag & FLAG_YAY0)}
        if rtype == 8:  # Bss: no payload
            nxt = payload
        else:
            nxt = payload + size
            if rec["compressed"]:
                data, used = yay0_decode(rom, payload)
                rec["yay0_header_size"] = u32(rom, payload + 4)
                rec["compressed_used"] = used
                if used > size:
                    problems.append(f"{cur['name']} {rec['name']}: Yay0 stream ({used:#x}) "
                                    f"overruns its record ({size:#x})")
                if len(data) != rec["yay0_header_size"]:
                    problems.append(f"{cur['name']} {rec['name']}: decoded {len(data):#x} != "
                                    f"Yay0 header {rec['yay0_header_size']:#x}")
            else:
                data = rom[payload:payload + size]
            rec["data"] = data
        cur["records"].append(rec)
        o = nxt
        if rtype == 11:  # End of this file
            cur["rom_end"] = o
            cur["size"] = o - cur["rom_offset"]
            cur = None


def classify(f):
    types = {r["type"] for r in f["records"]}
    if f["module_id"] < 0:
        return "null"
    if 12 in types:
        return "bin"
    if 5 in types:
        return "uso-code"
    return "uso-data"


def check_uso(f, problems):
    """Cross-check Sym and Reloc tables against the section sizes."""
    secs = {}
    for r in f["records"]:
        if r["type"] in secs and r["type"] not in (11,):
            problems.append(f"{f['name']}: duplicate {r['name']} section")
        secs[r["type"]] = r
    sizes = {t: (r["size"] if t == 8 else len(r["data"])) for t, r in secs.items() if t != 11}
    if 1 not in secs:
        problems.append(f"{f['name']}: no Sym section")
        return 0, 0
    sym = secs[1]["data"]
    if len(sym) % 12:
        problems.append(f"{f['name']}: Sym size {len(sym):#x} not a multiple of 12")
    nsym = len(sym) // 12
    for i in range(nsym):
        info = u32(sym, i * 12) >> 16
        kind, tag = info & 7, info >> 4
        val = u32(sym, i * 12 + 4)
        if kind in (2, 4):
            if tag not in sizes:
                problems.append(f"{f['name']}: sym {i} in missing section type {tag}")
            elif val > sizes[tag]:
                problems.append(f"{f['name']}: sym {i} value {val:#x} past section {tag}")
    nrel = 0
    for rt, target in RELOC_TARGET.items():
        if rt not in secs:
            continue
        rel = secs[rt]["data"]
        if target not in sizes:
            problems.append(f"{f['name']}: {SECTION_NAMES[rt]} without its target section")
            continue
        for i in range(len(rel) // 12):
            info, off = u32(rel, i * 12), u32(rel, i * 12 + 4)
            nrel += 1
            if (info & 7) not in (1, 2, 3, 4):
                problems.append(f"{f['name']}: reloc type {info & 7}")
            if off + 4 > sizes[target]:
                problems.append(f"{f['name']}: {SECTION_NAMES[rt]} offset {off:#x} past "
                                f"{SECTION_NAMES[target]} size {sizes[target]:#x}")
            if (info >> 4) >= nsym:
                problems.append(f"{f['name']}: reloc sym {info >> 4} >= {nsym}")
    return nsym, nrel


def sha1(b):
    return hashlib.sha1(b).hexdigest()


def write(path, data):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "wb") as fp:
        fp.write(data)


def extract(rom, out_dir, do_write=True):
    problems = []
    devices = read_devices(rom)
    manifest = {"rom_sha1": sha1(rom), "devices": [], "files": []}
    for dev in devices:
        files, eof_end = walk_device(rom, dev, problems)
        manifest["devices"].append({**dev, "file_count": sum(1 for f in files if f["module_id"] >= 0),
                                    "eof_end": eof_end})
        for f in files:
            kind = classify(f)
            if kind == "null":
                continue
            raw = rom[f["rom_offset"]:f["rom_end"]]
            entry = {"name": f["name"], "device": f["device"], "path": f"{f['device']}/{f['name']}",
                     "kind": kind, "module_id": f["module_id"], "rom_offset": f["rom_offset"],
                     "device_offset": f["device_offset"], "size": f["size"],
                     "compressed": any(r["compressed"] for r in f["records"]),
                     "sha1": sha1(raw), "sections": []}
            if do_write:
                write(os.path.join(out_dir, "fs", f["device"], f["name"]), raw)
            for n, r in enumerate(f["records"]):
                if r["type"] == 11:
                    continue
                sec = {"type": r["type"], "name": r["name"], "rom_offset": r["rom_offset"],
                       "record_size": r["size"], "flag": f"0x{r['flag']:08X}",
                       "compressed": r["compressed"]}
                if r["type"] == 8:
                    sec["bss_size"] = r["size"]
                else:
                    d = r["data"]
                    sec["size"] = len(d)
                    sec["sha1"] = sha1(d)
                    if kind == "bin":
                        sec["file"] = f"bin/{f['name']}"
                    else:
                        sec["file"] = f"uso/{f['name']}/{n:02d}_{r['name']}.bin"
                    if do_write:
                        write(os.path.join(out_dir, sec["file"]), d)
                entry["sections"].append(sec)
            if kind != "bin":
                entry["sym_count"], entry["reloc_count"] = check_uso(f, problems)
            manifest["files"].append(entry)

    ids = [e["module_id"] for e in manifest["files"]]
    dup = sorted({i for i in ids if ids.count(i) > 1})
    if dup:
        problems.append(f"duplicate module ids {dup}")
    missing_core = [i for i in range(47) if i not in ids]
    if missing_core:
        problems.append(f"module ids missing from 0-46: {missing_core}")
    gaps = [i for i in range(max(ids) + 1) if i not in ids]
    manifest["module_ids"] = {"count": len(ids), "max": max(ids), "gaps": gaps}
    manifest["problems"] = problems
    if do_write:
        os.makedirs(out_dir, exist_ok=True)
        with open(os.path.join(out_dir, "manifest.json"), "w") as fp:
            json.dump(manifest, fp, indent=1)
    return manifest


def summary(m):
    files = m["files"]
    by_kind = {}
    for e in files:
        by_kind[e["kind"]] = by_kind.get(e["kind"], 0) + 1
    comp = [s for e in files for s in e["sections"] if s["compressed"]]
    lines = [
        f"ROM SHA-1 {m['rom_sha1']}",
        "devices: " + ", ".join(f"{d['name']} ({d['file_count']} files)" for d in m["devices"]),
        f"files: {len(files)}  " + "  ".join(f"{k}={v}" for k, v in sorted(by_kind.items())),
        f"module ids: {m['module_ids']['count']} (max {m['module_ids']['max']}, "
        f"gaps {m['module_ids']['gaps']})",
        f"Yay0 sections: {len(comp)} ({sum(s['record_size'] for s in comp):#x} -> "
        f"{sum(s['size'] for s in comp):#x} bytes), all decoded to their header size: "
        f"{not any('Yay0' in p or 'decoded' in p for p in m['problems'])}",
        f"self-check problems: {len(m['problems'])}",
    ]
    lines += ["  " + p for p in m["problems"]]
    return "\n".join(lines)


def main(argv=None):
    here = os.path.dirname(os.path.abspath(__file__))
    root = os.path.normpath(os.path.join(here, "..", "..", ".."))
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--rom", default=os.path.join(root, "baserom.z64"))
    ap.add_argument("--out", default=os.path.join(root, "port", "assets", "extracted"))
    ap.add_argument("--no-write", action="store_true", help="only run the walk and self-checks")
    ap.add_argument("--skip-sha1", action="store_true", help="accept a ROM with a different SHA-1")
    a = ap.parse_args(argv)
    with open(a.rom, "rb") as fp:
        rom = fp.read()
    if rom[:4] != b"\x80\x37\x12\x40":
        print("error: not a big-endian .z64 ROM (byte-swapped .v64/.n64 are not supported yet)")
        return 2
    h = sha1(rom)
    if h != EXPECTED_SHA1 and not a.skip_sha1:
        print(f"error: ROM SHA-1 {h} != expected {EXPECTED_SHA1} (US 1.0)")
        return 2
    try:
        m = extract(rom, a.out, not a.no_write)
    except ExtractError as e:
        print(f"error: {e}")
        return 1
    print(summary(m))
    if not a.no_write:
        print(f"wrote {a.out}")
    return 0 if not m["problems"] else 1


if __name__ == "__main__":
    sys.exit(main())

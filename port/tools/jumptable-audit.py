#!/usr/bin/env python3
"""Audit the case labels of donor switches whose jump table is pinned to the ROM's table.

A donor spliced with REPLACE_FUNC_BODY has its .rodata relocations renamed to <FUNC>_rodata and
pinned (undefined_syms_auto.txt) to the table the USO ships. The matching build therefore links
the ROM's table, and only the code bytes are checked: a C case label that names the wrong value
still matches (same bounds, same body order). The PC port compiles the C switch with gcc, which
builds its table from the labels, so a wrong label is a wrong dispatch there (plan 15.7: the
AudioThread_ProcessGlobalCmd labels were off by one, and no sequence ever started).

For each game_libs donor: the donor .o's own .rodata tables (R_MIPS_32 against .text, REL addend
in the word = the C labels' bodies) against the ROM words at bootup RoData + pin + the same offset
(USO text addresses, game_libs offset + 0x1466C). Entries are compared relative to the function.

Usage: port/tools/jumptable-audit.py [--verbose]   (from the project root, after make)
"""
import re
import struct
import subprocess
import sys

ROM = "baserom.z64"
RODATA_ROM = 0xD9FE44      # bootup.uso RoData (game_libs rodata)
GL_TO_USO = 0x1466C        # game_libs splat offset -> bootup.uso text address
READELF = "mips-linux-gnu-readelf"


def makefile_donors():
    text = open("Makefile").read()
    var = dict(re.findall(r"^(\w+_DONOR)\s*:=\s*(\S+)", text, re.M))
    out = {}
    for func, v in re.findall(r"REPLACE_FUNC_BODY\s*\+=\s*(\w+)=\$\((\w+)\)", text):
        if v in var:
            out[func] = var[v]
    return out


def pins():
    out = {}
    for m in re.finditer(r"^(\w+)_rodata\s*=\s*0x([0-9A-Fa-f]+);", open("undefined_syms_auto.txt").read(), re.M):
        out[m.group(1)] = int(m.group(2), 16)
    return out


def func_rom(func):
    try:
        s = subprocess.run(["grep", "-rl", "--include=*.s", "glabel %s$" % func, "asm/nonmatchings/game_libs"],
                           capture_output=True, text=True).stdout.split()
    except Exception:
        return None
    if not s:
        return None
    m = re.search(r"/\* ([0-9A-F]+) ([0-9A-F]{8}) ", open(s[0]).read())
    return (int(m.group(1), 16), int(m.group(2), 16)) if m else None


def donor_tables(obj, func):
    """[(rodata offset, donor text offset relative to func)] from the donor's .rodata relocs."""
    relocs = subprocess.run([READELF, "-rW", obj], capture_output=True, text=True).stdout
    syms = subprocess.run([READELF, "-sW", obj], capture_output=True, text=True).stdout
    fstart = None
    for line in syms.splitlines():
        f = line.split()
        if len(f) >= 8 and f[7] == func:
            fstart = int(f[1], 16)
    if fstart is None:
        return None
    sec = None
    entries = []
    for line in relocs.splitlines():
        m = re.match(r"Relocation section '\.rel\.(\S+)'", line)
        if m:
            sec = m.group(1)
            continue
        f = line.split()
        if sec == "rodata" and len(f) >= 5 and f[2] == "R_MIPS_32" and f[4] == ".text":
            entries.append(int(f[0], 16))
    data = subprocess.run(["mips-linux-gnu-objcopy", "-O", "binary", "--only-section=.rodata", obj, "/dev/stdout"],
                          capture_output=True).stdout
    return [(k, struct.unpack(">I", data[k:k + 4])[0] - fstart) for k in entries]


def main():
    verbose = "--verbose" in sys.argv
    rom = open(ROM, "rb").read()
    donors = makefile_donors()
    bad = 0
    checked = 0
    for func, pin in sorted(pins().items()):
        obj = donors.get(func)
        if obj is None or "/game_libs/" not in obj:
            continue
        fr = func_rom(func)
        if fr is None:
            print("skip %s: no game_libs asm" % func)
            continue
        uso = fr[1] + GL_TO_USO
        tab = donor_tables(obj, func)
        if not tab:
            print("skip %s: no .rodata text relocs in %s" % (func, obj))
            continue
        checked += 1
        diffs = []
        for k, rel in tab:
            romw = struct.unpack(">I", rom[RODATA_ROM + pin + k:RODATA_ROM + pin + k + 4])[0]
            if romw - uso != rel:
                diffs.append((k, rel, romw - uso))
        status = "OK" if not diffs else "MISMATCH (%d of %d entries)" % (len(diffs), len(tab))
        print("%-44s %-40s %s" % (func, obj.split("/")[-1], status))
        if diffs and verbose:
            for k, rel, r in diffs:
                print("    .rodata+0x%X: C label body +0x%X, ROM body +0x%X" % (k, rel, r))
        bad += bool(diffs)
    print("%d donors checked, %d with case labels that disagree with the ROM table" % (checked, bad))
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())

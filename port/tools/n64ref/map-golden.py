#!/usr/bin/env python3
"""The N64's course-map (minimap) marker inputs for port/host/match_race.input (plan 34), from an n64ref --watch run.

    n64ref --rom baserom.z64 --out DIR --input port/host/match_race.input --max-vi 9200 --watch-from 1480 \\
        --watch 0x80298384:0x24
    port/tools/n64ref/map-golden.py DIR/watch.bin port/tools/n64ref/map-golden.tsv [--every 8] [--first 1504] [--last 3960]

0x802981B8 is Match Race's CourseGadget (game_libs; the HUD course map, hud_addCourseGadget(self, 0x123, 0xE1, 0xD))
on a 4 MiB console with this script; the watch covers +0x1CC..+0x1EF: pct[2], marker[2][2] (the player's and the CPU
rider's x / z, CourseGadget_setVec4 from RaceEntry.posX / progress every step: mgr_DataManager_update), originX,
originZ, extent. The last --watch record of a step holds that step's results (the port's state.log line of the
step: marker[i] == r<i>= x / z). Writes `step p0x p0z p1x p1z pinx piny` (f32 bits, hex; pin = the CPU pin's map
offset in pixels as CourseGadget_display computes it: (int)(H * (v - origin) / extent), H = e[7] - e[3] = -94 on
Crystal Lake). modes-check.py's match check compares the port's r0= / r1= x / z bit for bit.
"""
import argparse, struct

N = 0x24


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('watch')
    ap.add_argument('out')
    ap.add_argument('--every', type=int, default=8)
    ap.add_argument('--first', type=int, default=1504)
    ap.add_argument('--last', type=int, default=3960)
    ap.add_argument('--height', type=int, default=-94)
    a = ap.parse_args()
    b = open(a.watch, 'rb').read()
    rec = 8 + N
    last = {}
    for o in range(0, len(b) - rec + 1, rec):
        _vi, step = struct.unpack('<II', b[o:o + 8])
        last[step] = b[o + 8:o + rec]
    with open(a.out, 'w') as fh:
        fh.write('# N64 (n64ref, plan 34) course-map marker inputs for port/host/match_race.input: step, player x z, '
                 'CPU x z (f32 bits), CPU pin x y (px); port/tools/n64ref/map-golden.py\n')
        for s in range(a.first, a.last + 1, a.every):
            if s not in last:
                continue
            w = last[s]
            p1x, p1z, ox, oz, ext = struct.unpack('>5f', w[16:36])
            pin = (int(a.height * ((p1x - ox) / ext)), int(a.height * ((p1z - oz) / ext)))
            fh.write('%d %s %d %d\n' % (s, ' '.join(w[i:i + 4].hex() for i in (8, 12, 16, 20)), pin[0], pin[1]))


if __name__ == '__main__':
    main()

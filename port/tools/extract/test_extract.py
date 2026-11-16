"""Tests for extract.py. Run: python3 -m unittest discover -s port/tools/extract

The ROM test runs only when baserom.z64 is present (it is never committed)."""
import os
import struct
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import extract  # noqa: E402

ROOT = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", ".."))
ROM = os.path.join(ROOT, "baserom.z64")


class Yay0Test(unittest.TestCase):
    def test_literals_and_overlapping_backref(self):
        # "abc" as literals, then a back-reference of 6 bytes at distance 3.
        blob = b"Yay0" + struct.pack(">III", 9, 20, 22) + struct.pack(">I", 0xE0000000)
        blob += struct.pack(">H", ((6 - 2) << 12) | (3 - 1)) + b"abc"
        data, used = extract.yay0_decode(blob, 0)
        self.assertEqual(data, b"abcabcabc")
        self.assertEqual(used, len(blob))

    def test_long_backref(self):
        # count nibble 0: length = next chunk byte + 0x12
        blob = b"Yay0" + struct.pack(">III", 1 + 0x20, 20, 22) + struct.pack(">I", 0x80000000)
        blob += struct.pack(">H", 0) + b"z" + bytes([0x20 - 0x12])
        data, _ = extract.yay0_decode(blob, 0)
        self.assertEqual(data, b"z" * 0x21)


@unittest.skipUnless(os.path.exists(ROM), "baserom.z64 not present")
class RomTest(unittest.TestCase):
    def test_extract_self_checks(self):
        with open(ROM, "rb") as fp:
            rom = fp.read()
        if extract.sha1(rom) != extract.EXPECTED_SHA1:
            self.skipTest("baserom.z64 is not the expected US 1.0 ROM")
        with tempfile.TemporaryDirectory() as out:
            m = extract.extract(rom, out, do_write=True)
            self.assertEqual(m["problems"], [])
            ids = {e["module_id"] for e in m["files"]}
            self.assertTrue(set(range(47)) <= ids)
            names = {e["path"] for e in m["files"]}
            self.assertIn("mainuso/bootup.uso", names)
            self.assertIn("rom/game.uso", names)
            self.assertIn("rom/anims.bin", names)
            self.assertTrue(os.path.exists(os.path.join(out, "manifest.json")))
            game = next(e for e in m["files"] if e["name"] == "game.uso")
            text = next(s for s in game["sections"] if s["name"] == "Text")
            self.assertTrue(text["compressed"])
            self.assertEqual(os.path.getsize(os.path.join(out, text["file"])), text["size"])


if __name__ == "__main__":
    unittest.main()

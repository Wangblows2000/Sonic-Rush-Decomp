#!/usr/bin/env python3
"""Cutscene sound banks (BYOR): one native bank per sys-SDAT sequence archive.

The DS loads one sys-SDAT group per cutscene (TalkDemo_LoadSound,
PlayerDemo_LoadSound, TransformDemo_LoadSound: NNS_SndArcLoadGroup(group)),
e.g. group 4 TALK (SSAR 4 ARC_TALK), 16 BOSS_TALK (SSAR 12), 15 BOSS_TRANS
(SSAR 11). The whole sys SDAT does not fit the AICA (4.8 MB of waves), so the
port builds the sys bank once per SSAR with every other SSAR emptied: the SSAR
indices, sequence ids, players and priorities stay those of the sys SDAT, so
Sound_PlaySeqArc(handle, arc, id) / Sound_PlayVoice(handle, arc, id) keep the
original arguments. Uses tools/dreamcast/audio/sfxbank.py (same .srb layout).

Output: <out>/sys_aNN.srb (NN = SSAR index) + sys_aNN.json.
Usage: csbank.py [--extract extract/eur] [--audio build/dreamcast/audio]
                 [--out build/dreamcast/cutscene/sound] [--arcs 4,8,...]
"""
import argparse
import os
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT / "tools/dreamcast/audio"))
import sdatlib  # noqa: E402
import sfxbank  # noqa: E402

# SSARs a cutscene plays from (TalkDemo data_0207747c, PlayerDemo data_02076544,
# TransformDemo 0xb, StaffRoll / ExtraEnding); 2 / 3 (ARC_ZONE_MAP_S / _B) whole for the
# sound test's voice entries (the sys bank keeps only the zone map's sequences of them)
DEFAULT_ARCS = [2, 3, 4, 8, 9, 10, 11, 12, 13, 14, 15]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--extract", default=str(ROOT / "extract/eur"))
    ap.add_argument("--audio", default=str(ROOT / "build/dreamcast/audio"))
    ap.add_argument("--out", default=str(ROOT / "build/dreamcast/cutscene/sound"))
    ap.add_argument("--arcs", default=",".join(map(str, DEFAULT_ARCS)))
    ap.add_argument("--budget", type=int, default=sfxbank.DEFAULT_BUDGET)
    a = ap.parse_args()
    a.only = ""
    b = sfxbank.Builder(a)
    orig = sdatlib.SDAT
    out = Path(a.out)
    out.mkdir(parents=True, exist_ok=True)
    worst = 0
    for arc in [int(x) for x in a.arcs.split(",") if x]:
        class OneArc(orig):
            def __init__(self, path, _arc=arc):
                super().__init__(path)
                self.info["seqarc"] = [s if i == _arc else None for i, s in enumerate(self.info["seqarc"])]
        sfxbank.sdatlib.SDAT = OneArc
        try:
            # select=None: every SSAR of the (one-SSAR) SDAT; the default select of sfxbank.SELECT["sys"]
            # keeps only the menu SSARs and left these banks empty
            r = b.build("sys", select=None)
        finally:
            sfxbank.sdatlib.SDAT = orig
        name = "sys_a%02d" % arc
        os.replace(out / "sys.srb", out / (name + ".srb"))
        os.replace(out / "sys.json", out / (name + ".json"))
        rt = out / "sys.rendered.txt"
        if rt.exists():
            rt.unlink()
        worst = max(worst, r["sound_ram_bytes"])
        print("%s sound RAM %8d  waves %3d  ids %3d%s" % (name, r["sound_ram_bytes"], r["waves"], len(r["ids"]),
                                                       "" if r["sound_ram_bytes"] <= a.budget else "  OVER BUDGET"))
    return 0 if worst <= a.budget else 1


if __name__ == "__main__":
    sys.exit(main())

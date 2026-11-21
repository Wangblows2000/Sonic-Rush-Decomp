#!/usr/bin/env python3
"""Second layout pass: organise src/game/ by gameplay system and zone.

One-shot migration, already applied (2026-11-21). Input layout was
src/game/<module>/<Prefix>/<symbol>.c; re-running it now changes nothing.
Output layout: docs/source-organization.md.

Evidence:
- zone numbers and titles: per-stage tables [zone 0..8][act 0..2]
  (gStageZoneSetupFuncTable, gStageZoneFileListTable, gZoneBg*/gZoneFx*Table,
  gStageZoneScriptTable) and each zone's zNN_act asset set
  (docs/research/0014-zones.md);
- per-directory zone ownership of ov000 objects/enemies/effects: assets found
  in only one zone's zNN_act narc, placement in zNN_eve .ev/.dc files, explicit
  zone-index checks (docs/research/0014-zones.md, table in this script).

Only paths change. delinks.txt order, sections and TU boundaries are untouched.
Usage: python3 tools/utilities/reorg_systems.py [--dry]
"""
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
CFG = ROOT / "config/eur/arm9"
DELINKS = [CFG / "delinks.txt", CFG / "itcm/delinks.txt",
           CFG / "overlays/ov000/delinks.txt", CFG / "overlays/ov001/delinks.txt"]

ZONES = {1: "Leaf_Storm", 2: "Water_Palace", 3: "Mirage_Road", 4: "Night_Carnival",
         5: "Huge_Crisis", 6: "Altitude_Limit", 7: "Dead_Line", "F": "Final_Zone",
         "X": "Extra_Zone"}


def zone(z, sub):
    return f"Zone/{ZONES[z]}/{sub}"


# --- per-directory destinations (directory = symbol prefix) -----------------
G = {}


def put(dest, *dirs):
    for d in dirs:
        G[d] = dest


# Core game system
put("GameSystem", "Main", "Game", "System", "Task", "Heap", "Mem", "File", "FileRequest",
    "Overlay", "Compress", "Math", "Rand", "Pad", "Touch", "Mic", "StoryProgress",
    "SceneChange", "DisableSwapBuffersTask", "Misc")
# Graphics engine
put("Graphics/Sprite", "AnimatorSprite", "AnimatorSpriteDS", "BAC", "SpritePair", "Oam",
    "OamFreezeTask", "OamUnfreezeTask")
put("Graphics/Model", "Asset3D", "Model", "ModelPalette", "KeyframeAnim")
put("Graphics/Palette", "Palette", "PaletteAnim", "PaletteTransfer")
put("Graphics/Vram", "Vram", "VramPltt", "VramTex", "VRAMTransfer", "TransferJob",
    "ScreenTransfer")
put("Graphics/Background", "Background", "BackgroundDS", "Bg")
put("Graphics/Screen", "BrightnessFade", "BrightnessLerp", "ScreenFlash", "ScreenShake",
    "Display")
put("Graphics/Font", "Font")
put("Sound", "Sound", "SysSound", "SysSoundBGMFade", "SysSoundDelayedSE", "StageJingle",
    "TensionSound")
put("SaveData", "SaveData", "SaveMsg", "SaveMsgRequest")
put("Wireless", "DownloadPlay", "NetPacket", "VsConnect", "Wireless", "WirelessError",
    "WirelessLobby", "WirelessLobbyChild", "WirelessLobbyDownloadPlay", "WirelessLobbyParent",
    "WirelessLobbyPreview", "WirelessMgr", "WirelessSession", "WirelessSessionEnd",
    "WirelessSessionStop", "StageNetEvent")
# Stage (acts): flow, loading, scripts, per-zone background/effect drivers
put("Stage", "Stage", "StageEntry", "StageExit", "StageLoader", "StageRestart",
    "StageShutdown", "StageStart", "StageEvent", "StageZoneScript", "StageClear",
    "PlayerStart")
put("Stage/ZoneBackground", "ZoneBg", "ZoneFx")
put("Stage/ObjectSystem", "MapObject", "StageObj", "ObjAction", "ObjAngle", "ObjCollisionMap",
    "ObjData", "ObjDraw", "ObjRect", "ObjShift", "ObjSpd", "ObjVram", "ObjVramRef",
    "MapCollision", "Gimmick")
put("Player", "Player", "AttackHitbox")
put("Camera", "Camera")
put("Boss", "Boss", "BossArena", "BossCamera", "BossEffect")
# Boss1 (Egg Hammer) is used by Leaf Storm and, upgraded, by Huge Crisis (z53 places Boss1)
put("Boss/Boss1", "Boss1", "Boss1Hammer", "Boss1Neck", "EffPebble")
put("UI/Hud", "Hud", "PlayerIcon", "ScorePopup", "CheckpointTimePopup", "MarkerTimeList",
    "EnemyRoomHud", "EffEnemyRoomFix", "BossHelp", "FixInformation")
put("UI/MessageBox", "MsgBox")
put("UI/Navi", "NaviB", "NaviBg", "NaviM", "NaviMPartA", "NaviMPartB", "NaviMsg", "NaviMsgFace")
put("UI/TitleCard", "StageTitleCard", "StageTitleCardBg", "StageTitleCardPart180",
    "StageTitleCardPart20", "StageTitleCardPart260", "StageTitleCardPart324",
    "StageTitleCardPart420", "StageTitleCardPartE0", "StageTitleCardPlayer",
    "StageTitleCardStrip", "StageTitleCardText3E8", "StageTitleCardText404")
put("UI/GameOver", "GameOver", "GameOverSeq")
put("VsMode", "EffVs", "EffVsConfusion", "EffVsSlowdown", "EffVsTensionDrain",
    "EffVsTensionMax", "FixVs", "FixVsHud", "FixVsPushIcon", "WirelessIcon")
# Menus
put("Menus/Pause", "PauseMenu")
put("Menus/SegaLogo", "SegaLogo")
put("Menus/TitleScreen", "TitleScreen", "TitleScreenFadeTask", "TitleDemo")
put("Menus/TitleMenu", "TitleMenu", "TitleMenuBgSlideTask", "TitleMenuFadeTask",
    "TitleMenuVsMode0Wait", "TitleMenuVsMode1Wait", "TitleMenuVsMode2Wait")
put("Menus/Options", "Options", "OptionsBgScroll", "OptionsFadeTask", "OptionsMenu",
    "OptionsWfsHost", "SoundTest")
put("Menus/Records", "Records", "RecordsFadeTask")
put("Menus/TimeAttack", "TimeAttackMenu", "TimeAttackMenuBgScroll", "TimeAttackMenuFadeTask")
put("Menus/VsMenu", "VsMenu", "VsMenuBrightnessFade")
put("Menus/TryAgain", "TryAgain", "TryAgainBgScroll", "TryAgainFadeTask")
put("Menus/ZoneMap", "ZoneMap", "ZoneMapFadeTask", "ZoneMapPause", "ZoneActSelect",
    "ZoneActSelectMode")
put("Menus/PlayerSelect", "PlayerSelectMenu")
put("Menus/TimeSelect", "TimeSelectMenu")
put("Menus/Results", "ResultsScreen")
# Cutscenes
put("Cutscenes/TalkDemo", "TalkDemo", "TalkDemoDeferredCall", "TalkDemoDim", "TalkDemoFreeRes",
    "TalkDemoRocks", "TalkDemoScene", "TalkDemoSceneBg2Window", "TalkDemoWindow",
    "TalkDemoWindowRaster")
put("Cutscenes/PlayerDemo", "PlayerDemo", "PlayerDemoBg", "PlayerDemoBgFreeRes",
    "PlayerDemoCameraShake", "PlayerDemoLight", "PlayerDemoModel", "PlayerDemoOp",
    "PlayerDemoRepeatSE", "PlayerDemoSkip")
put("Cutscenes/ClearDemo", "ClearDemo", "ClearDemoActText", "ClearDemoBars", "ClearDemoBg",
    "ClearDemoModel", "ClearDemoName", "ClearDemoScore", "ClearDemoTA")
put("Cutscenes/PolyDemo", "PolyDemo", "PolyDemoUnder2")
put("Cutscenes/TransformDemo", "TransformDemo", "TransformDemoEffect")
put("Cutscenes/DemoScript", "Demo", "DemoInput", "DemoPlay", "DemoScript")
put("Cutscenes/Ending", "Ending", "EndingBg0Hide", "EndingBg0Picture", "EndingBgLayersShow",
    "ExtraEnding", "StaffRoll", "StaffRollCredit", "PostStaffRoll", "PostStaffRollFade")
# Special Stage
put("SpecialStage", "SpecialStage", "SpecialStageActArchive", "SpecialStageActObj",
    "SpecialStageData", "SpecialStageIntroBg", "SpecialStageObj", "SpecialStageObjList",
    "SpecialStagePlayerArchive", "SpecialStageMap", "SpecialStageGimmick", "SpecialStageTrick",
    "SpecialStageTrickReward")
put("SpecialStage/Player", "SpecialStagePlayer")
put("SpecialStage/Camera", "SpecialStageCamera")
put("SpecialStage/Hud", "SpecialStageHud")
# Extra Zone (stage idx 8 act 2): boss fight and its clear scene
put(zone("X", "Boss"), "ExtraBoss", "ExtraBossObj", "ExtraBossPlayer", "ExtraBossPlayerManager")
put(zone("X", "Results"), "ExtraAct")
# Zone stage setup (main: Zone<N>Act<M>, BossStage<N>)
for n in range(1, 8):
    for a in (1, 2, 3):
        put(zone(n, "Stage"), f"Zone{n}Act{a}")
    put(zone(n, "Boss"), f"BossStage{n}")
put(zone("F", "Stage"), "Zone8Act3")
put(zone("F", "Boss"), "BossStageF")
put(zone("X", "Stage"), "Zone9Act3")

CSV_CATEGORY = {"Objects": "Objects", "Decorations": "Decorations", "Effects": "Effects",
                "Enemies": "Enemies", "Boss": "Boss", "UI": "UI"}


def load_ov000_classes():
    """Zone-specific ov000 directories from the classification table (0014)."""
    import csv
    path = ROOT / "docs/research/0014-ov000-classes.csv"
    out = {}
    for row in csv.DictReader(path.open()):
        d, cat, z = row["dir"], row["category"], row["zone"]
        if d in G or cat not in CSV_CATEGORY:
            continue
        if z in ("shared", "unknown"):
            out[d] = CSV_CATEGORY[cat] if cat != "Boss" else "Boss"
        else:
            out[d] = zone(int(z) if z.isdigit() else z, CSV_CATEGORY[cat])
    return out


# --- per-file zone rules (shared prefix directories with per-zone functions) ---
FILE_RULES = [
    (re.compile(r"^ZoneBg_Zone(\d)Boss_"), lambda m: zone(int(m[1]), "Background")),
    (re.compile(r"^ZoneBg_Zone(\d)_"), lambda m: zone(int(m[1]), "Background")),
    (re.compile(r"^ZoneFx_Zone(\d)Boss_"), lambda m: zone(int(m[1]), "Background")),
    (re.compile(r"^ZoneFx_Zone(\d)_"), lambda m: zone(int(m[1]), "Background")),
    (re.compile(r"^ZoneFx_ZoneFBoss_"), lambda m: zone("F", "Background")),
    (re.compile(r"^StageZoneScript_Zone(\d)Boss$"), lambda m: zone(int(m[1]), "Stage")),
    (re.compile(r"^StageZoneScript_ZoneFBoss$"), lambda m: zone("F", "Stage")),
    (re.compile(r"^StageLoader_Zone(\d)Boss_"), lambda m: zone(int(m[1]), "Stage")),
    (re.compile(r"^StageLoader_ZoneFBoss_"), lambda m: zone("F", "Stage")),
]


def dest_for(path):
    p = Path(path)
    parts = p.parts  # src game <module> <Prefix> file
    if len(parts) != 5 or parts[:2] != ("src", "game"):
        return None
    prefix, stem = parts[3], p.stem
    for rx, fn in FILE_RULES:
        m = rx.match(stem)
        if m:
            return f"src/game/{fn(m)}/{p.name}"
    d = G.get(prefix)
    if d is None:
        return "UNMAPPED"
    return f"src/game/{d}/{p.name}"


def tus():
    for path in DELINKS:
        for line in path.read_text().splitlines():
            if line.startswith("src/game/") and line.endswith(":"):
                yield line[:-1]


def main():
    G.update(load_ov000_classes())
    dry = "--dry" in sys.argv
    moves, unmapped = {}, []
    for tu in tus():
        new = dest_for(tu)
        if new == "UNMAPPED":
            unmapped.append(tu)
        elif new and new != tu:
            moves[tu] = new
    if unmapped:
        for u in unmapped:
            print("unmapped:", u)
        sys.exit(1)
    seen = {}
    for o, n in moves.items():
        if n.lower() in seen:
            sys.exit(f"collision {o} / {seen[n.lower()]} -> {n}")
        seen[n.lower()] = o
    dirs = {}
    for n in moves.values():
        d = str(Path(n).parent)
        if dirs.setdefault(d.lower(), d) != d:
            sys.exit(f"case collision {d}")
    print(f"{len(moves)} moves into {len(dirs)} directories")
    if dry:
        from collections import Counter
        for d, c in sorted(Counter(str(Path(n).parent) for n in moves.values()).items()):
            print(f"  {c:5d} {d}")
        return
    for o, n in moves.items():
        (ROOT / n).parent.mkdir(parents=True, exist_ok=True)
        subprocess.run(["git", "mv", o, n], cwd=ROOT, check=True)
    for path in DELINKS:
        lines = path.read_text().splitlines(keepends=True)
        for i, l in enumerate(lines):
            s = l.rstrip("\n")
            if s.endswith(":") and s[:-1] in moves:
                lines[i] = moves[s[:-1]] + ":\n"
        path.write_text("".join(lines))
    for d in sorted((ROOT / "src/game").glob("*/"), reverse=True):
        if d.name in ("main", "ov000", "ov001", "itcm"):
            for sub in sorted(d.rglob("*"), reverse=True):
                if sub.is_dir() and not any(sub.iterdir()):
                    sub.rmdir()
            if not any(d.iterdir()):
                d.rmdir()
    print("done")


if __name__ == "__main__":
    main()

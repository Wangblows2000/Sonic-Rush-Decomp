# 0014 — Zones: internal numbers, titles and zone-specific code (EUR)

How zones are numbered:
- `gGameState+0x1C` holds the zone and `+0x1D` holds the act. Both are **0-based**.
- All stage tables are laid out `[zone 0..8][act 0..2]`. That gives 27 stages, `stage = zone*3 + act`. Tables checked: `gStageZoneSetupFuncTable`, `gStageZoneFreeFuncTable`, `gStageZoneReleaseFuncTable`, `gStageZoneFileListTable` (main) and `gZoneBg*Table`, `gZoneFx*Table`, `gStageZoneScriptTable` (ov000).
- Function names `Zone<N>Act<M>` are **1-based** (N = zone + 1, M = act + 1).
- Act 3 (act index 2) is the 3D boss stage.

## (a) Zones 1-7

| N (1-based) | idx | Title | Evidence | Confidence |
|---|---|---|---|---|
| 1 | 0 | Leaf Storm | Stage files z11/z12/z13 (`gStageZoneFileListTable[0]`). z11_act holds rope_c, bend_stick, trampoline, egg pawn, dec_flower/vine/leaf/bird_nest. Boss stage loads bs1_base/neck/hammer/sky through `BossStage1_OnModelLoaded` (Egg Hammer). Sound `z1es`. Sonic story slot 0. | confirmed |
| 2 | 1 | Water Palace | z21_act holds water_jet, water_fall, fountain, fish, coral, algae, kuragen (jellyfish), egg_diver, bubble. `ZoneFx_Zone2_*` tints palettes (underwater). Boss loads bs2 lotus/water/shower/head/neck (Egg Turtle). | confirmed |
| 3 | 2 | Mirage Road | z31_act holds sand_slider, dec_sand, statue, relief, torch, hand, egg_hammer, egg_bishop. Boss loads bs3 ball/thorn/body (Egg Scarab). | confirmed |
| 4 | 3 | Night Carnival | z41_act holds neon_* decorations, swing_sky, neon_route, flapper_bat, egg_bunny. z41_eve holds z4pn01-16 palettes. Boss loads bs4 arm/rod/ele (Egg Libra). It is the first zone of Blaze's story order (`data_02074bd0` = 3,0,2,1,5,4,6,7,8). `Spring_OnHit` and `WallBreak_Create` have special cases for idx 3. | confirmed |
| 5 | 4 | Huge Crisis | z51_act holds pipe, piston, valve, steam, crane, jet_catapult, bomb_hawk, gun_hunter, rhino. Boss list = bs5_base, **bs1_neck, bs1_hammer**, bs5_sky. z53_act holds bs5_ham/bs5_neck palettes and bs1.nsbca. This matches Egg Hammer Fortress, an upgraded Egg Hammer. | confirmed |
| 6 | 5 | Altitude Limit | z61_act holds glider, parachute, saucer, cloud, propeller, jet, falco, flapper_cannon. `DecCloudSpawner_Create` spawns DecCloudW only when idx==5. Boss loads bs6 body/prop01/02/rcloud/tcloud/bult. The boss is **Egg Eagle** (fandom), not "Egg Bomber". | confirmed |
| 7 | 6 | Dead Line | z71_act holds rocket, cannon, duct, air, egg_solid, egg_sf, egg_flapper_laser, laser. Boss stage z73 loads only bs7_base/sky and has eff_aura_son/blz, eff_sb/bb_boost and strike effects. This fits the Sonic-vs-Blaze rival fight, which is Dead Line's boss. | confirmed |

Story order tables:
- Sonic `data_02074bc4` = 0..8.
- Blaze `data_02074bd0` = 3,0,2,1,5,4,6,7,8. Read as Night Carnival, Leaf Storm, Mirage Road, Water Palace, Altitude Limit, Huge Crisis, Dead Line, F, Extra. The NC-then-LS opening matches published lists. I could not check the rest of Blaze's order against a reachable source.
- `StoryProgress_GetZoneAt` clamps to 8.

## (b) Zones 8, 9, F, Extra, Special Stage

**F = internal zone idx 7 (1-based "zone 8").** Confirmed.
- Stage 23 (idx 7, act 2) loads zf3_map/eve/act, `snd/zfes`, and bsf_base/body/ball/shock/wave_sl/pas_dust01 through `BossStageF_OnModelLoaded`.
- The matching entries are `gStageZoneFreeFuncTable[23]` = `BossStageF_FreeModels`, `gStageZoneScriptTable[23]` = `StageZoneScript_ZoneFBoss`, `gZoneFxInit/Update[23]` = `ZoneFx_ZoneFBoss_*`, and `gStageBossMapLoaderTable[7]` = `StageLoader_ZoneFBoss_LoadMapGraphics`.
- It is story slot 7 for both characters. Other "F" assets: navi `msg_zonefinal`, `stdm_bgf`, talk_demo_boss_fmd_*.
- It is the final zone before Extra. Fandom gives Egg King as Sonic's final boss; that is tentative. Only one bsf boss data set exists.

**Zone 8 acts 1-2 (stages 21, 22) and zone 9 acts 1-2 (stages 24, 25).** These are not separate zones; they are variants that reuse other zones' assets:
- z81 = Leaf Storm assets. Its act narc is byte-identical to z11/z12, its eve narc contains z11.bbg, and it uses `Zone1Act1_*`, `ZoneBg_Zone1_*`.
- z82 = Mirage Road assets (eve contains z31.bbg; uses `Zone3Act1_*`, `ZoneBg_Zone3_*`).
- z91 = Night Carnival assets (z41.bbg plus z4pn; `Zone4Act1_*`, `ZoneBg/Fx_Zone4_*`).
- z92 = Huge Crisis assets (z51.bbg plus z5pn; `Zone5Act1_*`, `ZoneBg/Fx_Zone5_*`).
- Their maps are smaller than the originals.
- Code treats `zone in 7..8 && act < 2` specially: `Player_WrapLoopingArena` wraps x between 0x10000 and 0xB0000 (0x180000 for idx 8 act 1), `Marker_Create` uses PassCheck, and `Player_Action_ReachGoal`, `Player_Create`, `ZoneBg_Zone3_UpdateParallax/UpdateScreenScroll` change behaviour. `Spring_Create` gives idx 8 act 0 the idx 3 (Night Carnival) palette.
- Conclusion: looping race-arena variants, probably for VS. Tentative.
- I found no code path that selects them. The VS stage menu (`TimeSelectMenu_State_Select`) cycles slots 0..13 only (zones idx 0-6, acts 0-1), although it draws 18 slots. TCRF calls z82 unused. They may be unused, or reached only from the download-play child (mb/contest.srl's arm9 has the same zone file table). Status: unknown.

**Zone 9 act 3 (stage 26, idx 8 act 2) = Extra Zone.** Confirmed.
- `ExtraBoss_Start` (ov001) sets `zone=8, act=2`.
- `StageLoader_LoadZoneMapGraphics` skips idx 8 act 2.
- `gStageZoneFileListTable[26]` is NULL, because ExtraBoss loads its own `/narc/extra_act_lz7.narc`, `snd/ze`, and `/mod/bsex_md.nsbmd`.
- Player models are `e_son`/`e_blz` (Super Sonic / Burning Blaze); the boss is Egg Salamander per Wikipedia.
- The `Zone9Act3_*` functions are empty stubs.

**ExtraBoss (ov001)** is the Extra Zone boss fight. It is reached from `gDemoScriptTable` entries 0x40/0x41 (DemoScript mode 3) through `SceneChange_ToExtraBoss`.

**ExtraAct (ov001)** is the Extra Zone's post-boss clear/results scene, not a playable act. Evidence:
- `DemoScript_StartAfterBoss`: when zone idx == 8 it calls `DemoScript_Create(2)`, which is entry 0x44, `SceneChange_ToExtraAct`.
- Its symbols are score tally, time bonus (`gExtraActTimeBonusTable`) and rank stamp.
- It uses `extra_act_lz7.narc` with `snd/sys`.
- Confidence: strong.

**Special Stage (ov001 `SpecialStage*`).** Confirmed. It is not a zone slot.
- `SpecialStage_Start` calls `SpecialStage_Init(gGameState zone byte)`, which leads to `SpecialStage_LoadStageData(arg)` and `gSpecialStageFileTable[arg]` = `ss/ss_<idx+1>.sst`.
- So Special Stage k is chosen by the zone index (0-6) it was entered from. Assets: ss_act/ss_player narcs, ss_eme00-06.

## (c) Task 2 functions whose zone is not just their number

Every other function in task 2 matches its number. Checked through the per-stage tables above:
- all `ZoneN Act1/2/3_*` for N=1..7,
- `BossStage1..7`,
- `ZoneBg_Zone1..7_*`,
- `ZoneBg_Zone1Boss_Load` (slot 2 = z13),
- `ZoneFx_Zone2/4/5/6/7_*`,
- `ZoneFx_Zone3Boss_*` (slot 8),
- `StageZoneScript_Zone3Boss/Zone7Boss` (slots 8/20),
- `StageLoader_Zone2..7Boss_LoadMapGraphics` (table idx N-1).

No `Boss<N>*` function references another zone's bs files, apart from the exceptions listed here.

| Function(s) | Actual attribution | Evidence |
|---|---|---|
| `Zone8Act3_SetupResources`, `Zone8Act3_ReleaseResources` | **Zone F boss stage (zf3)**, stage 23 | Setup/Release table slot 23. In the same slot, Free = `BossStageF_FreeModels` and the file list = zf3 + bsf. "8" is just the 1-based number of idx 7, which is F. Suggest `ZoneFAct3_*` / `ZoneFBoss_*`. |
| `Zone9Act3_SetupResources`, `_ReleaseResources`, `_Free` | **Extra Zone**, stage 26 | Slot 26 has no file list. `ExtraBoss_Start` sets idx 8/2. |
| `Game_StartDemoScript3AtZone8Act2` | **Extra Zone (1-based Zone9 Act3)** | The name uses the 0-based values (8,2). DemoScript 3 resolves to `SceneChange_ToExtraBoss`. It is called from `PlayerSelectMenu_ExitToGame`, mode 0 sub 2. Suggest `Game_StartExtraBoss` or `...AtZone9Act3`. |
| `Zone1Act1_*`, `Zone3Act1_*`, `Zone4Act1_*`, `Zone5Act1_*` | Also used for stages 21 (z81), 22 (z82), 24 (z91) and 25 (z92) | Setup/Free/Release table slots 21/22/24/25. The functions are shared, so the names are not wrong. |
| `ZoneBg_Zone1_*`, `ZoneBg_Zone3_*`, `ZoneBg_Zone4_*`, `ZoneBg_Zone5_*`, `ZoneFx_Zone4_*`, `ZoneFx_Zone5_*` | Also run on z81/z82/z91/z92 | The `gZoneBg*` and `gZoneFx*` tables reuse them in slots 21/22/24/25. `ZoneBg_Zone3_UpdateParallax` and `_UpdateScreenScroll` have explicit idx 7-8 branches. Quirk: `gZoneFxReleaseTable[21]` (z81, Leaf Storm-based) = `ZoneFx_Zone4_Release`, while Init/Update[21] are NULL. |
| `Boss1_Create` and the `Boss1*` object code | **Shared by zone 1 (Egg Hammer) and zone 5 (Egg Hammer Fortress)** | `gMapObjectCreateTable` holds `Boss1_Create` at idx 63 and at idx 59. The run is 56=F, 57=7, 58=6, **59=5**, 60=4, 61=3, 62=2, 63=1. `BossStage5` loads bs1_neck/bs1_hammer. `Boss5PartD3` (missile, obj idx 211) is the only Boss5-specific object. |
| `BossStage5_OnModelLoaded` / `_FreeModels` | Zone 5, but they load zone 1's bs1_neck/bs1_hammer models | Stage 14 file list. |
| `BossF*`, `BossStageF_*`, `ZoneFx_ZoneFBoss_*`, `StageZoneScript_ZoneFBoss`, `StageLoader_ZoneFBoss_LoadMapGraphics` | F = internal zone idx 7 ("zone 8" when counted 1-based) | Stage 23 and boss-loader idx 7. |

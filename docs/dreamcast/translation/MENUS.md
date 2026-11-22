# Native front end and save data (Dreamcast port): translation notes

Status: the front end runs from boot to the stages and back. It covers the
SEGA logo, the title screen (with the attract-demo request), the title menu,
player select (Sonic / Blaze / extra), the zone map with the story
progression, the act select, the map pause and the options (difficulty, time
limit, sound test, delete save data). Save data is the original `gSaveData`
format, stored on the VMU. The code was checked as follows:

- Host test: the menus test passes (38/38 checks) and the save test passes (49/49).
- Build: all sources compile with `kos-cc -Werror`.
- KOS link test: it links (menus + save + `engine/render.c` + `engine/input.c` + a stub main).

It has **not** been re-run on SH-4 hardware or in an emulator since the fixes below. The integrator saw
an "Invalid texture V size" abort in Flycast, which is fixed by the power-of-two textures (section 3.1). The
single-screen compositions were checked with the host software compositor
(`test/mn_raster.h`), not on a PVR.

- Code:
  - `dreamcast/game/menus/`: `menus.h` (public API), `mn_internal.h`, `mn_*.c`, `tools/`, `test/`, `Makefile`.
  - `dreamcast/game/save/`: `save.h`, `save.c`, `save_vmu.c`, `test/`, `Makefile`.
- Converters (BYOR):
  - `dreamcast/game/menus/tools/menudata.py` writes ROM tables, menu backgrounds, `.ama` banks and the sound test list.
  - `menubg.py` is the menu `.bbg` converter.
- Source of truth:
  - `src/game/Menus/{SegaLogo,TitleScreen,TitleMenu,PlayerSelect,ZoneMap,Options}`, `src/game/SaveData`.
  - `src/game/GameSystem/StoryProgress_*`, `Game_StartStoryStage`, `SceneChange_*`.
  - `src/game/Cutscenes/ClearDemo/ClearDemo_GoToNextScene`, `src/game/Menus/Pause/PauseMenu_QuitToMap`.
  - `src/game/Cutscenes/Ending/PostStaffRoll_*` (unlock notices).
  - `src/game/Graphics/Model/KeyframeAnim_*`, `src/game/GameSystem/Pad_*`.
  - `src/asm_fallback/ZoneMap_ConfirmNode.c` and `ZoneMap_State_MoveToNode.c`, translated from the listings.
- Third-party code: none.

## 1. Files

| file | content |
|---|---|
| `menus.h` | API: `menus_init`, `menus_run_frame`, `menus_draw_tr`, `menus_stage_exit`, `MenuRequest`, `MenuHooks` |
| `mn_internal.h` | engine services named as on the DS (tasks, gPad, AnimatorSprite, Background, KeyframeAnim, brightness), `MnData` (ROM tables), `MnGameState` (gGameState fields) |
| `mn_core.c` | task list, `Pad_Reset`/`Pad_Update`, `.mnbg` / `frames.bin` / `.ama` loaders, AnimatorSprite subset, quads, DS compositing order, single-screen composition (`MnComp`), KeyframeAnim |
| `mn_draw.c` | projection of the DS quads to 640x480 with clipping (portable); KOS: power-of-two textures (BG images re-expanded when their palette changes) + TR-list quads + one full-frame brightness overlay |
| `mn_main.c` | boot, per-frame loop, `menus_stage_exit` (ClearDemo_GoToNextScene, PauseMenu_QuitToMap, game over, demo end) |
| `mn_story.c` | StoryProgress_*, Game_StartStoryStage, scene requests, sound hooks |
| `mn_title.c` | SegaLogo_*, TitleScreen_* (+ fade task, palette cycle), PostStaffRoll unlock notices |
| `mn_titlemenu.c` | TitleMenu_* (pages, button key frames, BG colour wipe, exits) |
| `mn_zonemap.c` | ZoneMap_* (node graph, paths, story steps, moves, scroll view, start stage) + ZoneMapPause_* |
| `mn_actsel.c` | ZoneActSelect_*, ZoneActSelectMode_* |
| `mn_psel.c` | PlayerSelectMenu_* (no 3D models) |
| `mn_options.c` | Options settings / sound test (layout rewritten) |
| `tools/menudata.py`, `tools/menubg.py` | converters (section 4) |
| `test/test_menus.c`, `test/mn_raster.h`, `test/link_main.c` | host test, host compositor, KOS link stub |
| `../save/save.c` | SaveData_* |
| `../save/save_vmu.c` | VMU backend (KOS) |
| `../save/test/test_save.c` | save host test |

## 2. Integration (main.c)

`main.c` and `build_disc.sh` are outside this area. The integrator makes these changes.

```c
#include "../menus/menus.h"
#include "../save/save.h"

/* once, after pvr_init / sound init */
save_vmu_install();                         /* VMU backend for SaveData_* */
menus_set_paths("/cd/menus", "/cd/sprites");
g_menu_hooks.bgm = ...;  g_menu_hooks.se = ...;  g_menu_hooks.voice = ...;
g_menu_hooks.stop_all = ...;  g_menu_hooks.sound_test = ...;   /* optional */
menus_init();                               /* menudata.bin, save_load() (boot), SegaLogo */

/* front-end loop (instead of the stage loop) */
InputState in;
input_poll(&in);
MenuRequest r = menus_run_frame(&in);
render_begin_frame();
render_begin_list(PVR_LIST_TR_POLY); menus_draw_tr(); render_end_list();
render_end_frame();
switch (r.kind) {
case MENU_REQ_STAGE:      start_stage(r); break;       /* below */
case MENU_REQ_DEMO:       start_demo(r.demo); break;   /* or: r = menus_stage_exit(MENU_EXIT_DEMO_END, 0,0,0,0) */
case MENU_REQ_UNAVAILABLE: printf("menus: %s not in the port\n", r.scene); break;  /* menus keep running */
default: break;
}
```

`start_stage(r)` does the following.

- Stage name: `"z%d%d", r.zone + 1, r.act + 1`.
  - `r.zone` is the DS zone id (`gGameState.unk1C`), which already went through the story order. Blaze starts at zone 3.
  - Acts are 0, 1 and 2 (boss).
  - Zones 7 and 8 (extra and final story) have no entries in `k_stages` yet.
- `g_chara = r.character`.
- `g_dcp_world.mode = r.mode`.
- `g_dcp_world.lives = r.lives`. These are the lives from the save (`StoryProgress_RestoreFromSave`), not a constant.
- `g_ui_game.save_lives[r.character] = r.lives`.
- `g_ui_game.save_flags = gSaveData.unk8` (bit 1: no time limit).
- `r.easy` is `gGameState.unk14`: 1 = normal, 0 = easy (the options value). Pass it to the systems that read difficulty (bosses).

When the UI module reports `ui_state() == UI_EXIT`, the integrator stops the stage and calls `menus_stage_exit`. The call follows the original exit code:

```c
uint32_t total = g_ui_game.total;           /* data_022c4590.unk0, what StoreStageResult saves */
MenuRequest r;
switch (ui_exit_kind()) {
case UI_EXIT_NEXT_ACT:  r = menus_stage_exit(MENU_EXIT_CLEARED,  total, g_dcp_world.lives, emeralds, g_dcp_world.frame); break;
case UI_EXIT_QUIT:      r = menus_stage_exit(MENU_EXIT_QUIT,     total, g_dcp_world.lives, emeralds, 0); break;
case UI_EXIT_GAME_OVER: r = menus_stage_exit(MENU_EXIT_GAME_OVER, 0, 0, emeralds, 0); break;
case UI_EXIT_TRY_AGAIN: r = menus_stage_exit(MENU_EXIT_CLEARED,  total, g_dcp_world.lives, emeralds, g_dcp_world.frame); break;
default:                r = menus_stage_exit(MENU_EXIT_GAME_OVER, 0, 0, emeralds, 0); break;
}
if (r.kind == MENU_REQ_STAGE) start_stage(r);   /* next act of a new stage, boss act */
else enter_front_end();                         /* zone map, act select, SEGA logo ... */
```

- `emeralds` is `gGameState.unk28`. Return the value from the last request until special stages exist.
- `UI_EXIT_NEXT_ACT` covers what the DS does in `ClearDemo_Create`, `ClearDemo_Destructor` and `ClearDemo_GoToNextScene`:
  - progress update and best score saved, then the VMU commit;
  - new stage: act 1 goes straight to act 2, act 2 goes to the boss, the boss goes to the zone map with the story step;
  - replayed stage: the act select (`ZoneMap_StartFromStage`).
- For a time-attack clear, the record (`SaveData_InsertTimeRecord`) is saved. TryAgain is not translated, so the request reports it as unavailable and the title menu opens.
- The stage loop no longer needs the "hold Y for Blaze" boot switch: `r.character` replaces it.

Draw order:

- `menus_draw_tr` uses z 10..90. The single full-frame brightness overlay is at z 90.
- When only menus are shown, nothing else is in the TR list.
- The OP list can stay empty: all menu quads are TR, because sprites and BGs have colour-0 transparency.

Build: add `$(patsubst %.c,%.o,$(wildcard menus/mn_*.c)) save/save.o save/save_vmu.o` to `OBJS`
in `dreamcast/game/Makefile`. KOS needs no extra library: `vmufs` and `vmu_pkg` are in libkallisti.

## 3. Single-screen composition (one 640x480 frame)

The DS shows two 256x192 screens. The port shows **one** continuous 4:3 640x480
frame. There is no stacked or side-by-side mode, no second view, no divider line
and no per-screen fade. The emulated DS engines still exist, because the
translated code draws to them: `Background_Show(engine, ...)`, the OAM priority
order and `BRIGHT_MAIN` / `BRIGHT_SUB`. They are only *sources* for the
composition.

Each scene builds an `MnComp` (`mn_internal.h`) when it sets up its display.
An `MnComp` is a list of up to 8 `MnRegion` entries, drawn back to front. Each region has:

- `engine` and a source rect in DS pixels (`sx0, sy0, sx1, sy1`);
- a destination origin `dx, dy` and one **uniform** `scale`, so the art is never stretched;
- `hide_bg` (a mask of BG layers left out of this region) and `hide_spr`;
- the DS compositing order (priority 3..0, BG before sprites, first OAM entry in front), applied inside the region.

Two fields belong to `MnComp` itself:

- `base`: the engine whose brightness fades the **whole frame**. The code does one full-frame overlay; the other engine's brightness is ignored.
- `fill`: an optional backdrop colour. It is used where a scene does not cover the frame.

The helpers are `mn_comp_reset(base)`, `mn_comp_add(...)` and `mn_comp_full(engine)` (0,0-256,192 at x2.5 = 640x480).
`mn_project` clips each quad to its region's source rect and transforms it.
`mn_sorted` walks the regions in order. The host compositor (`test/mn_raster.h`) and the
PVR path (`menus_draw_tr`) use the same projection.

| scene | base | composition | where the second screen's content goes |
|---|---|---|---|
| SegaLogo | main, white fill | main logo block (32,44-232,142) x1.5 at (16,166) and sub logo block (40,44-232,134) x1.5 at (336,172) | the two logo cards ("presented by SEGA", "created by SONIC TEAM") sit side by side on one white field |
| TitleScreen | main | main full (logo, copyright, x2.5) plus the sub's PRESS START plate (40,78-216,106, sprites only) x2.0 at (144,318) | PRESS START under the logo, between the logo and the copyright |
| unlock notice (PostStaffRoll) | main, black fill | main full | - |
| TitleMenu | main | main full (character art) plus the sub's button column (8,24-200,168, sprites only) x1.5 at (336,110) | the menu buttons and the B/back button sit on the right side of the art |
| PlayerSelect | sub | sub full (character art, scrolling name banner, "PLEASE SELECT A CHARACTER") plus the main's dial (0x36,0x08-0xca,0xc0: circle BG3, character dots, plate with L/R arrows; the white BG2 is left out) x1.0 at (478,72), and the main's B/back button x1.5 at (580,258) | the selector dial is a widget to the right of the art |
| ZoneMap, map pause, ActSelect | main | main full | the zone map, the pause box and the act list are on the touch screen; the sub (navigator panel) is not translated and is blank |
| Options (list, difficulty, time limit, erase, sound test) | sub | main BG3 full x2.5 (the zigzag backdrop, BG2/sprites left out), main BG2 Tails art (130,4-248,188) x2.0 at (392,56), sub panel (BG2 + sprites, BG3 left out) x1.5 at (4,96) | the options panel on the left of a single backdrop, with Tails on the right |

Region coordinates were tuned by looking at the host dumps (section 9). A scene
that changes its composition (the act select returning to the map) calls its
`*_Compose` function again.

### 3.1 PVR textures

The PVR accepts only power-of-two texture sizes (8..1024). `pvr_poly_compile` aborts
with "Invalid texture V size" otherwise. Two paths create textures:

- Sprite atlas pages (`res_tex`): the page is copied row by row into a
  `mn_tex_pow2(w) x mn_tex_pow2(h)` buffer (zero padding, stride = padded
  width) before `pvr_txr_load`.
- Menu BGs (`bg_tex`): these were already allocated at `tw x th`, the padded power-of-two size, with stride `tw`.

`tex_new` rejects non-power-of-two sizes. u/v are divided by the padded size, so the padding is never sampled.
The host build of `menus_draw_tr` asserts, for every texture it uses, that both
sizes are powers of two in 8..1024 and not smaller than the image.

## 4. Assets the disc needs (all built from the owner's ROM)

```
make -C dreamcast/game/menus data      # -> build/dreamcast/menus/  (about 1 s)
```

| disc path | from | size |
|---|---|---|
| `/cd/menus/menudata.bin` | `menudata.py`: ROM tables (section 6.3) + VMU icon from the DS banner | 1.8 KB |
| `/cd/menus/bg/*.mnbg` | `menubg.py`: 50 menu `.bbg` (`bg/sega_logo_*`, `title_logo_*`, `mnu_*`, `title_demo_image_*`, `bg2/zmap/*`, `bg2/caution/*`) | 2.6 MB |
| `/cd/menus/ama/*.ama` | `files/ama/` copied (mnu_button, mnu_wipe, mnu_opti, ps_act, zmap_map_icon, zone_sel) | 32 KB |
| `/cd/menus/sound_test.sld` | `files/st/sound_test.sld` copied (127 tracks) | 6 KB |
| `/cd/sprites/<id>/frames.bin` + `atlas0.argb1555` | `tools/dreamcast/sprites/bac2dc.py` output, ids below | |

Sprite ids: `act__title`, `act__title_<lang>`, `act__mnu_titl`, `act__mnu_titl_<lang>`, `act__mnu_psel`,
`act__mnu_psel_<lang>`, `act__mnu_opti_<lang>`, `act__font_opti_sound`, `act2__zmap__zmap_map_icon`,
`act2__zmap__zmap_scroll_arrow`, `act2__zmap__zmap_act`, `act2__zmap__font_zmap_act_num`,
`act2__zmap__zmap_pause`, `act2__zmap__zmap_pause_<lang>`. `<lang>` is jpn, eng, fra, deu, ita or spa
(`gGameLanguage`, default 1 = eng). English only needs about 2.9 MB of atlases.

Memory: one scene at a time.

- Largest scene: the zone map.
  - 648x240 BG: 155 KB of indices, plus a 1024x256 texture (512 KB of VRAM).
  - Sprites: about 0.4 MB of VRAM.
- Images are freed when the scene ends.

### 4.1 Menu BBG format and `.mnbg`

| offset | field |
|---|---|
| 0 | `"BBG\0"` |
| 4 | size |
| 8 | chr offset |
| 0xC | screen offset |
| 0x10 | palette offset |
| 0x14 | flags (bits 0-3 type) |
| 0x18 | w (tiles) |
| 0x1A | h (tiles) |
| 0x1C | u16 palette colour offset (`Background_Init`: `dest += unk1c * 2`) |
| 0x1E | u16 palette slot (`dest += unk1e * 512`) |

Types:

| type | format |
|---|---|
| 1 | 4 bpp, banks in bits 12-15 of the screen entries |
| 2 | 8 bpp ext palette |
| 5 | 8 bpp affine-ext (same screen entries). Only `zmap_map_bg`: 81x30 tiles |

Pixel value 0 is transparent.

`.mnbg` v1 layout:

| offset | content |
|---|---|
| 0 | `"MNBG"` |
| 4 | w, h (px) |
| 8 | type, pofs, pslot |
| 16 | the 256-colour palette as DS BGR555 |
| 528 | w*h u8 indices (4 bpp flattened to bank*16+value) |

The indices are kept, so the palette transfers of the menu code (title palette cycle, title-menu wipe colours, act-select zone colours) apply at run time.

Checked: previews in `build/dreamcast/menus/preview/` (SEGA logo, title logo, zone map, title-menu art, act-select BGs, player-select art, options art) and the composited frames in section 7.

## 5. Save data

`gSaveData` is 0x314 bytes. Layout:

| offset | content |
|---|---|
| 0 | CRC-32 |
| 4 | save counter |
| 8 | flags (1 easy, 2 no time limit, 0x100 player-select page, 0x200 sound test, 0x1000 extra cleared, 0x200000 unlock notice shown) |
| 0xC | Sonic: progress u16, lives, emeralds, best scores u32[7][3] |
| 0x64 | Blaze: the same fields |
| 0xBC | time-attack records u16[7][3][5] + Blaze bits |
| 0x1A4 | zones 7/8 records |
| 0x1D0 | VS totals |
| 0x1D4 | 10 VS opponents |

`save.h` asserts the struct offsets.

Kept as on the DS:

- The backup image: 12-byte signature `sonic_rush_` (read from the ROM into menudata.bin), then 10 slots of 0x320 bytes.
- 5 slot pairs, rotated by the counter (`SaveData_AdvanceSlot`), each pair written twice.
- Newest valid pair at boot (`SaveData_SelectLatestSlot`).
- Fallback to the second copy.
- Validation clamps.
- Defaults: 2 lives each, time records 0xffff.

DS card access, replaced:

- `CARDi_ReadBackup` and `CARDi_ProgramBackup` work on a RAM copy of the image.
- `save_flush` writes the image to the VMU at the end of the DS write sequences:
  - `SaveData_Save` (commit): after both slots;
  - `SaveData_Format`;
  - `SaveData_Startup`: one write after format + signature.
- VMU file: `SONICRUSH`, packaged by `vmu_pkg`. It holds 128 header bytes, the 512-byte icon and the 8012-byte image: 8652 bytes, 17 blocks.
- The icon is the DS banner icon (32x32, 16 colours), converted to ARGB4444.
- First VMU on the maple bus.
- A DS backup image is byte-compatible: little-endian on both machines.

Boot: `menus_init` calls `save_load`, which runs `SaveData_Startup` and then `SaveData_Load`.

| case | DS | here |
|---|---|---|
| no VMU | error message box, halt | flags 0x80: play with the defaults, no saving (`SAVE_DISABLED`) |
| VMU full / write fails | error message box, halt | the same: saving disabled, `save_last_error() == 1/2` |
| no file | format + signature (code 4) | the same (one VMU write) |
| both copies of the newest pair corrupt | "format?" message box (code 3) | defaults loaded; the next commit writes them (`save_last_error() == 3`) |

The `SaveMsg_*` message boxes are not drawn (no MsgBox port). The error code is available to the caller.

## 6. How the original is mapped

### 6.1 DS services

| DS | native |
|---|---|
| Task_Create / Destroy / DestroyRange / DestroyByTag / DestroyAll | fixed table, run by priority. A destructor runs at once; the work memory of a task destroyed while the list runs stays valid to the end of the frame. Tasks created during a frame start on the next frame |
| Pad_Reset / Pad_Update (trg, rep, delay 20 / interval 2) | translated. Keys from `InputState`: A->A, B->B, X->X, Y->Y, START->START, D-pad/analog->D-pad |
| gTouchState | not used: every touch action is mapped to a pad action (section 6.2) |
| Background_Init + Background_Draw (BBG, BGxCNT size/priority) | `Background_Show/SetPriority/SetSize`, scroll in `gDisplayMain/Sub[layer]` |
| PaletteTransfer to BG palettes | `mn_bg_set_colors` (texture re-expanded when a colour changed) |
| AnimatorSprite_Init / SetAnimation / ProcessAnimation / Draw / DrawRotScale | `MnSpr` on converted `frames.bin`: the first process after SetAnimation runs the first frame; durations in frames. Flags: 1 stopped, 2 ended, 4 loop, 0x80/0x100 flips, 0x60000 OAM priority, 0x80000 hidden |
| OAM / BG compositing | per engine, back to front: for priority 3..0, BG layers of that priority (higher BG number further back), then sprites in reverse call order (the first sprite drawn is in front) |
| master brightness (data_0208926c / data_02087d44) | `g_mn_bright[engine]` (both still written by the translated code); only `g_mn_bright[g_mn_comp.base]` is drawn, as one full-frame overlay |
| KeyframeAnim_* (.ama banks) | translated (`mn_core.c`). The `.ama` files are read unchanged |
| File_Load, gXxxFileTable[lang] | typed loaders; the table entries are written out as path strings with the table name in a comment |
| Sound_PlaySeq / SeqArc / Voice, SysSound_* | `g_menu_hooks` with the original ids |
| SceneChange_ToStage / ToDemoPlay / VS / ending | `MenuRequest` returned by `menus_run_frame` / `menus_stage_exit` |
| gGameState / data_022c4590 / data_022c45f8 | `MnGameState gGameState` (only the fields the front end uses) |

### 6.2 Touch -> controller

| screen | touch action (DS) | controller |
|---|---|---|
| Title screen | "touch screen" area | A or START |
| Title menu | item button / back button | UP/DOWN + A / B (the original's pad path; the 8-frame touch-only back flash is not translated) |
| Player select | OK button, left/right arrows, back button | A, LEFT/RIGHT, B |
| Zone map | tap a node (walk there along the graph), scroll arrows, corner button | D-pad walks node to node (the original pad path). A selects the node. X opens the free scroll (D-pad scrolls, B returns). START opens the pause menu |
| Act select | zone tab, act row, back button | LEFT/RIGHT, UP/DOWN + A, B |
| Map pause | item rows | UP/DOWN + A, B (and START closes page A) |
| Options | list, choice buttons, sound test arrows / play / stop, back button | UP/DOWN + A, LEFT/RIGHT + A, LEFT/RIGHT, A, B, B |

### 6.3 ROM tables in menudata.bin

| section | address | used by |
|---|---|---|
| `save_sig` | gSaveSignature 0x02076b3c | SaveData_CheckSignature / WriteSignature |
| `story_s`, `story_b` | data_02074bbc / data_02074bb4 | StoryProgress_* |
| `zone_ord_s`, `zone_ord_b` | data_02074bc4 / data_02074bd0 | StoryProgress_GetZoneAt / GetZoneIndex, SaveData_StoreStageResult |
| `tm_slide` | data_ov001_0231616c | title-menu wipe colours |
| `zm_adj` | data_ov001_0231649c | zone map node graph `[mode][node][dir]` |
| `zm_rects`, `zm_icons` | data_ov001_02316358 / 023162f8 | node rects (touch only, kept), zone name icons |
| `as_rank1`, `as_rank2` | data_ov001_0231c3f4 / 0231c404 | act-select ranks (100000/80000/50000, boss 50000/40000/20000) |
| `st_widths` | data_ov001_02316028 | sound-test glyph widths |
| `vmu_icon` | banner bitmap/palette | VMU icon |

## 7. Function mapping

Status: **as-is** = same statements and constants (types/helpers renamed); **modified** = same logic,
DS calls replaced; **rewritten** = behaviour kept, code substantially different; **new** = native
only; **not translated** = section 8.

| original | translated | status |
|---|---|---|
| SaveData_Init, InitCRCTable, CalcChecksum, VerifyChecksum, SetDefaults, Validate, Compare, ReadSlotPair, AdvanceSlot, Save, IsDirty, Load, Commit, FormatAll, Format, SelectLatestSlot, Startup, IsEnabled | same | as-is (heap buffers -> stack, CARD -> image; Format/Save flush the image) |
| SaveData_ReadSlot, WriteSlot, FormatSlot, CheckSignature, WriteSignature, IdentifyBackup | same | modified (RAM image / VMU) |
| SaveData_GetProgressFlags, SetProgressFlags, GetSonicEmeralds, SetSonicProgress, SetBlazeProgress, SetDifficulty, SetTimeLimit, SetGameCleared, UnlockPlayerSelectPage, UnlockSoundTest, SetLives, SetLivesBoth, RecordVsResult | same | as-is |
| SaveData_GetTimeRank, InsertTimeRecord, StoreLives, StoreStageResult | same | modified (gGameState / data_022c4590 fields passed as arguments) |
| SaveData_StartupOrShowError, LoadOrShowError, CommitOrShowError, FormatOrShowError | same | modified (error kept in `save_last_error`, no message box; card errors disable saving instead of halting) |
| SaveMsg_*, SaveMsgRequest_* | - | not translated (message boxes) |
| Pad_Reset, Pad_Update | same | as-is |
| KeyframeAnim_Init, Update, EvalPosition, EvalScaleRotation, LerpPosition, LerpScaleRotation, EaseCurve, IsFinished, PlayReverse, SetFrame, SetToLastFrame, SetAnim, Default* | same (Eval*/Lerp* as static helpers) | as-is |
| StoryProgress_CalcMapStep, GetZoneAt, GetZoneIndex, GetActIndex, GetActAt, IsStageNew, RestoreFromSave, Update | same | as-is |
| Game_StartStoryStage | same | modified (DemoScript_StartScript0 for act 2 -> stage request; NaviM_SaveStep dropped) |
| ClearDemo_GoToNextScene | static in `mn_main.c` | modified (scene changes -> requests; ending, TryAgain, DemoScript4 reported unavailable) |
| PauseMenu_QuitToMap | `menus_stage_exit(MENU_EXIT_QUIT)` | modified |
| SegaLogo_Create, Start, State_Init, State_FadeIn, State_Wait, State_FadeOut, Interpolate, LoadAssets, ShowBackgrounds, ReleaseAssets, ResetDisplay, Destructor | same | as-is / modified (display registers -> layers) |
| SegaLogo_State_End | same | modified (TitleDemo_Start -> TitleScreen_Start: the 3D intro is not translated) |
| SegaLogo_InitDisplay | same | modified |
| TitleScreen_Create, Start, StartFromDemo, State_Init, State_WaitForStart, State_StartPressed, CheckStartPressed, CreateFadeTask, FadeTask_Main, CreatePaletteCycle, PaletteCycle_Main, LoadAssets, InitBackgrounds, InitSprites, Destructor | same | modified (logic and constants as-is; touch area -> A) |
| TitleScreen_StartAttractDemo | in State_WaitForStart | modified (SceneChange_ToDemoPlay -> MENU_REQ_DEMO with the demo index) |
| TitleScreen_GoToTitleMenu, GoToUnlockNotice | inline | as-is |
| PostStaffRoll_ShowUnlockNotices, State_PlayerSelectUnlockNotice, State_SoundTestUnlockNotice, State_GoToSegaLogo | `PostStaffRoll_*` in mn_title.c | rewritten (no message box art: dimmed screen, same 0xf0-frame timing, A closes early; flags + commit as-is) |
| TitleDemo_* (3D intro, 52 functions) | - | not translated |
| TitleMenu_Create, Main, Start, StartAtPlayerSelectItem0/1, StartFromOptions, LoadAssets, InitSprites, InitDisplay, Destructor | same | modified |
| TitleMenu_OpenRootPage, OpenPlayerSelectPage, OpenVsPage, OpenVsSubPage, CreatePageTask, Unk_ov001_022d7ee8 / 022d8060 (`TitleMenu_InitPage`), Unk_022d7dbc, Unk_022d7e60, Select* | same | as-is |
| TitleMenu_State_WaitOpenAnims, State_Unk_022d8938 (`State_Select`), 022d8798 (`State_Confirm`), 022d85d0 (`State_Close`) | same | modified (touch path removed; VS message box -> declined) |
| TitleMenu_State_Unk_022d86b0 | - | not translated (touch-only back flash) |
| TitleMenu_DrawItemButton, DrawBackButton, Unk_022d7ac4 (`DrawPageTitle`) | same | as-is |
| TitleMenu_CreateFadeTask, StartFadeToBlack/White, FadeTask_Main, CreateBgSlideTask*, BgSlideTask_Main, ApplyBgSlideLayers | same | modified (palette transfer -> wipe image colour 1, one image per BG) |
| TitleMenu_ExitToTitleScreen, ExitToGameStart, ExitToPlayerSelectMode3/4, ExitToOptions | same | modified (scene changes -> starts / requests) |
| TitleMenu_StartVsHost, StartVsJoin, GoToVsLobby, SelectVsMode0-2, Vs*Wait_Main, Attach/DetachVsMenu, *MsgBox* | `TitleMenu_ExitToVs` | not translated (VS) |
| PlayerSelectMenu_StartMode0/3/4, Create, LoadAssets, InitBackgrounds, InitSprites, Main, Action_Opening/Intro/Select/ScrollOut/ScrollIn/Confirm/FadeOutBack, State_Intro/Select/ScrollOut/ScrollIn/Confirmed/FadeOutBack, SetBackgroundSelection, IsFullyUnlocked, EaseValue, ExitToGame, ExitBack, Set/Draw sprite helpers | `PlayerSelectMenu_StartMode` + statics | modified (models and camera orbit dropped; sprite helpers merged; touch -> pad) |
| PlayerSelectMenu models, lighting, palette anim, link icon, sync/partner (VS), QuitPrompt, StartMode1/2, MarkUnlockNoticeShown/ShouldShowUnlockNotice | - | not translated |
| ZoneMap_StartDefault, StartFromStage, StartFromStageWithFade, Start, Create, Main, LoadAssets, InitWork, InitDisplay, Destructor | same | modified (navi panel, 3D model, HDMA dropped) |
| ZoneMap_Unk_ov001_022e8090 (`ZoneMap_CreateState`), RevealNodesForStory, RevealSonicNodes, RevealBlazeNodes, SetPathDotsType, InitNodeLayer, InitSonicNodes, InitBlazeNodes, InitSonicPaths, InitBlazePaths, InitPath, ClearPath, BuildLinePath, BuildSplinePath, SplineSolve, SplineEval, Unk_022e432c, InitSprite, InitScrollArrows | same | as-is |
| ZoneMap_DrawNodeLayer, DrawNodeIcons, EaseInterp, CalcClampedScroll, GetPathIndex, GetDirectionAngle, GetNodeForStoryStep, GetNodeForCurrentZone, Get/IsSonic/BlazeMaxSelectable/ZoneSelectable, GetSonic/BlazeStoryStep, Set*ProgressFromStep, SaveProgress, SaveProgressOnPathArrival, Unk_022e21b8, Unk_022e26d0, Is*NaviAvailable | same | as-is |
| ZoneMap_RunSonicStoryStep, RunBlazeStoryStep, SelectNextState, Action_WaitTimer/ScrollToPathStart/ScrollToPos/MoveToNode/FadeOut/FadeOutExit/OpenPause, State_WaitTimer/ScrollToPathStart/MoveAlongPath/ScrollToPos/SelectNode/ScrollView/WaitPauseClosed/FadeOutExit, Unk_022e606c, Unk_022e5f80, Unk_022e5de0, State_Unk_022e7114 (`State_StartStage`), 022e7428 (`State_OpenActSelect`), 022e736c (`State_WaitActSelect`) | same | modified (touch paths, navi messages, char model calls dropped) |
| ZoneMap_ConfirmNode, ZoneMap_State_MoveToNode (asm_fallback) | same | modified (translated from the listings; model rotation not applied) |
| ZoneMap_CheckNewEmerald | same | modified (NaviM counters -> none) |
| ZoneMap_UpdateView, UpdateMapBg, AnimateScrollArrows, DrawScrollArrows, SetScrollArrowMode/Blink, DrawBackButton, DrawScrollButton, Restart*ButtonAnim | same / merged | modified (character model -> map icon sprite 31; column streaming -> BG scroll) |
| ZoneMap_GetNextNodeFromPad | same | as-is |
| ZoneMap_GetNextNodeFromTouch | `ZoneMap_GetNextNodeFromQueue` | modified (only the queued-move part, used by pass-through nodes) |
| ZoneMap_Action_PlayTalkDemo*, State_PlayTalkDemo, SelectTalkDemo | `ZoneMap_Action_PlayTalkDemo` / State | rewritten (TalkDemo not translated: 32-frame pause, BGM restart, same next step) |
| ZoneMap_Action_ShowMsgBox11/12, State_WaitMsgBox11/12 | same | modified (11 closes at once; 12 = accepted) |
| ZoneMap_SetNaviMessage, SetNodeNaviMessage, InitNavi, DelayedCall_Main, State_WaitNaviMessage | id recorded / 30-frame wait | not translated (navigator panel) |
| ZoneMap_DrawCharModel, AnimateCharModel, SetCharModel*, LoadPlayerArchive, ReleaseCharModel, SetupLighting, ApplyCameraLookAt, SetOrthoProjection, StartBg3XHdma, StopHdma, DrawAlphaGradient, EnableBlend, Unk_022e519c | - | not translated |
| ZoneMapFadeTask_Main, ZoneMap_CreateFadeTask | same | as-is |
| ZoneMapPause_Create, Destroy, LoadAssets, Action_OpenA/B, SelectA/B, ConfirmA/B, CloseA/B, State_Open/Select/ConfirmFlash/Close, DrawItem, Unk_022e8b5c, Finish, IsClosed | same (Actions merged) | modified (touch path removed) |
| ZoneMapPause_ExecuteChoice | - | not translated (not reached from the map) |
| ZoneActSelect_Create, LoadRecords, CalcRecordRanks, DrawActCursor, DrawActLabels, DrawRecordRanks, DrawRecordScores, DrawZoneTabs, DrawBackButton, Unk_022e08e0, UpdateTabOffsets, LoadZonePalette, State_FadeIn, State_Select, State_Unk_022e0f08, State_FadeOut, IsFinished, GetResult, GetSelectedZone | same | modified (palette numbers per sprite not applied; touch removed) |
| ZoneActSelectMode_Start, Main, StartStage, ExitToTitleMenu, Shutdown | same | modified |
| Options_StartFromTitleMenu, Options_LoadSettingsFromSave, OptionsMenu_SetDifficultyChoice, Options_SetTimeLimit, Options_EraseSaveData, Options_ExitToTitleMenu, Options_IsSoundTestUnlocked/IsExtraStoryCleared, SoundTest_BuildTrackNameText, OptionsMenu_DrawSoundTestTrackName, OptionsMenu_State_TimeLimit/SoundTest input rules | same | as-is / modified |
| Options page framework (OptionsMenu_OpenPage0-5, ClosePage*, DrawPage*, State_Unk_*, UpdateAnims, InitEnterItemAnims, OptionsBgScroll_Main ...) | `Options_State_Main` + `Options_Draw` | rewritten (same sprites and choices, direct layout) |
| OptionsWfsHost_*, DownloadPlay, SoundTest level meter/capture | - | not translated |
| Options PLAYER DATA pages, Options_ExitToRecords, Options_ExitToTimeAttackMenuMode2/3, ZoneMapPause_ExecuteChoice2/3 | mn_options.c, mn_timeattack.c | as-is (layout of the pages rewritten like the other pages) |
| - | `menus_init`, `menus_run_frame`, `menus_draw_tr`, `menus_stage_exit`, `save_load`, `save_write`, `mn_*` | new |

## 8. Gaps and assumptions

- **Not translated**:
  - TitleDemo: the 3D intro before the title screen. The SEGA logo goes straight to the title screen.
  - VS (all wireless menus, TimeSelect): see section 11.
  - Ending / staff roll and the extra / final story scripts (DemoScript*). Zone 7 and 8 clears report "Ending".
  - Talk demos (story dialogue on the map) unless main.c sets the talk_demo hooks.
  - Message boxes (MsgBox, SaveMsg).
  - Options DOWNLOAD DEMO (wireless).
- **No 3D**: the player-select character models and the zone-map character are not drawn. The map uses the zone-map icon sprite 31 as a marker. `dreamcast/game/model` could draw `son.dcm` / `blz.dcm` there later.
- **Message boxes decided without asking**:
  - Zone-map MsgBox 0x12 ("go to this zone?") counts as yes. The A press on the node is the answer.
  - Title-menu MsgBox 0 (wireless warning) counts as no.
  - The SaveMsg "format?" question counts as no: the defaults are loaded and saved by the next commit.
- **Palette numbers per sprite** (`AnimatorSprite.unk44`: act-select tab colours, highlighted digits) are not applied. The converted atlases carry the BAC's own palette (see SPRITES.md).
- **The zone map fog** (`data_ov001_0231639c`, HDMA alpha gradient over unexplored parts) is not drawn. The whole map is visible.
- Options:
  - The page/animation code was rewritten.
  - The item positions come from the sprite origins and the touch rectangles of the original. They were not measured from a DS frame.
  - PLAYER DATA is the first item (section 13); DOWNLOAD DEMO is not listed.
- **Sound**:
  - Only the original ids are passed to `g_menu_hooks`. Nothing is mapped to DC audio yet.
  - The sound test passes the `sound_test.sld` fields.
  - The delayed SE of TitleMenu (`SysSound_PlaySEDelayed`) plays at once.
- **Timing**:
  - Tasks created during a frame start on the next frame. The DS inserts them into the running list, so a few transitions may be 1 frame later than on the DS.
  - SegaLogo to TitleScreen takes 218 frames.
- **KOS draw**:
  - BG textures are re-expanded from indices when a palette changes. The title screen does this every frame for its 256x192 base: about 49k pixels plus a 128 KB upload per frame.
  - Fine for a menu, not measured on hardware.
- **VMU**:
  - The first memory card found is used.
  - Writing 17 blocks blocks the menu task for the duration of the write. The DS also shows its save message at these points.
  - Not tested with a real VMU.
- **`StoryProgress_IsStageNew`**:
  - For a zone index above 7 (not a story zone) it returns FALSE.
  - The DS reads past its table there.

## 9. Verification (what was run)

- `make -C dreamcast/game/menus data`:
  - menudata.bin has 13 sections (1784 bytes), with 50 backgrounds, 6 `.ama` files and sound_test.sld.
  - Spot checks:
    - signature `sonic_rush_`;
    - story tables 2,5,9,13,17,21,24,26 / 3,7,10,14,18,22,26,28;
    - Blaze zone order 3,0,2,1,5,4,6,7,8;
    - rank thresholds 100000/80000/50000;
    - sound test: 127 entries, "A NEW DAY (TITLE EDIT)" first.
  - BG previews inspected: SEGA logo, title logo, zone map 648x240.
- `make -C dreamcast/game/save host-test`: 49/49 checks.
  - CRC = reference CRC-32 of bytes 4..0x314.
  - Defaults.
  - Fresh VMU: format + one write, code 4.
  - No change: no write.
  - Commit writes pair 1 with counter 1, both copies equal.
  - Reboot restores the newest pair. The pairs rotate 2,3,4,0 with `counter % 5 == pair`.
  - First copy damaged: the second copy is used. Both damaged: code 3 and the defaults.
  - Validation clamps.
  - Sorted time-record insert with the Blaze bits.
  - Best-score keep and clamp.
  - VS move-to-front.
  - Lives clamps.
  - Write failure: error 2 and saving disabled.
  - No VMU: `SAVE_DISABLED`, defaults, commit returns 6.
  - The signature is read from menudata.bin.
- `make -C dreamcast/game/menus host-test DUMP=/tmp/menus/dump`: 38/38 checks, real assets. The run drives, in order:
  - Boot: SEGA logo, then the title screen at frame 218.
  - Attract demo request after 0x4b0 idle frames (demo 1), demo end, START, title menu.
  - Cursor SE: `SysSound_PlaySE(4)` reaches the hook as archive 1, sequence 5 (the SE index mapping in `mn_story.c`).
  - GAME START on a new save: stage zone 0 act 0, lives 2.
  - Progress 2, zone map:
    - the story step reveals the path to zone 2 and saves progress 3 after 388 frames;
    - RIGHT walks to node 2;
    - A starts zone index 1 act 0.
  - Clears:
    - act 1 clear: act 2 request, best score saved;
    - act 2 clear: boss request, progress 4;
    - boss clear: zone map, progress 5, lives 7.
  - Reboot from the "VMU" keeps the progress.
  - Story steps 6..11: house path, walk to node 15, zone 3 path. Progress 7 is saved.
  - Walk to node 1 (RIGHT, DOWN). A on a cleared zone opens the act select (RIGHT for zone 2, DOWN for act 2), which requests zone 1 act 1.
  - Player select: Blaze, then the Blaze zone map.
  - Options:
    - DIFFICULTY -> EASY, TIME LIMIT -> OFF;
    - sound test track 2 played via the hook;
    - B commits and returns to the title menu;
    - the settings survive a reload.
- Map pause: START on the zone map opens the pause box (dump `13b_zonemap_pause`), B closes it and the cursor stays on node 15.
- Visual check, single-screen 640x480 (host compositor, PPM -> PNG, each image inspected). No image shows a DS panel boundary or divider:
  - `02_segalogo`: both logo cards on one white field.
  - `03_title`: logo, PRESS START plate, copyright.
  - `06_titlemenu_root`: Sonic/Blaze art, MENU plate, GAMEPLAY / BATTLE PLAY / OPTIONS, back button.
  - `20_psel_sonic`, `21_psel_blaze`, `22_psel_confirm`: art, name ticker, "PLEASE SELECT A CHARACTER", the selector dial with its arrows and the back button.
  - `11_zonemap_revealed`, `23_zonemap_blaze`: map, path dots, ZONE labels, scroll button.
  - `13b_zonemap_pause`: pause box over the map.
  - `14_actselect`: tabs 1-2, ZONE 2, ACT-1 40000 C, ACT-2 30000 C, VS BOSS 20000 B, back button.
  - `31_options`, `32_options_difficulty`, `33_options_soundtest`: panel, values, NORMAL/EASY choice, track name, A:OK / B:BACK, Tails.
  - Known: on the difficulty page the "DIFFICULTY?" question sprite (anim 22, white) lies on the light stripes of `mnu_opti_bg02_b` and is hard to read. The same thing shows in the raw sub-engine dump, so it is not a composition artefact. It probably comes from the sprite or BG palette conversion (the sprite converter is outside this area) and is still open.
- Debug aid: `MN_DUMP_ENGINES=1` also writes each DS engine alone as `raw/<name>_e0/e1.ppm`. This is how the region coordinates were chosen.
- `make -C dreamcast/game/menus dc`: 10 menu sources + save.c + save_vmu.c compile with `kos-cc -std=gnu99 -O2 -Wall -Wextra -Werror`.
- `make -C dreamcast/game/menus link`: `menus_link.elf` (stub main + menus + save + render.c + input.c) links with no unresolved symbols.


- `make -C dreamcast/game/menus host-test` also runs `test_flow` (56 checks):
  - time attack: records shown, scroll, zone X wrap, stage request zone 0 act 1 mode 2; TryAgain retry / change act / quit; boss mode with Blaze;
  - Records: 6 opponents, page scroll 0 → 16 → 32 → 0, options round trips, TIME ATTACK RECORD → BOSS view only;
  - demo exits: DEMO_END → title screen, DEMO_TIMEOUT → SEGA logo;
  - ExtraAct: time 17000 → time bonus 38000, 120 rings → 12000, total 50000, rank 0; 1362 frames (white from 1301); A skips to the end in 256 frames;
  - navigator: NaviM counter and count start, intro fade (alpha 15 at 30 frames, 31 at 60), intro panel shown then hidden after 0xf0 frames, a map message for 180 frames, fade out, locked early in the story, everything gone after the scene.
- Frame dumps (640x480) looked at for TimeAttackMenu, TryAgain, Records and ExtraAct (letterbox, title, tally, rank). The navigator is checked by state only.
- `make -C dreamcast/game/menus dc` and `link` (kos-cc, -Werror): clean, `menus_link.elf` links (316 KB text).
- `make -C dreamcast/game/demo host-test`: 73/73 (all four demos replayed frame-exact against a reference Pad_Update, lead-frame case, end record, fade-in, label blink, frame-limit exit, skip exit). `make -C dreamcast/game/demo dc`: clean.

## 10. Attract demos (dreamcast/game/demo)

`DemoInput_*` / `DemoPlay_*` and `SceneChange_ToDemoPlay` are in `demo/demo.c`.
The DS plays a recorded pad stream (`/dat/key/keyNN.bin`: u32 random seed,
then `{u16 keys, u8 frames}` records, 0 frames = end) while a normal stage runs.
The demo table `data_02073bd0` (4 entries: zone, act, character, start point,
frame limit, key file) is copied by `demo/tools/demodata.py` into
`build/dreamcast/demo/demotab.bin` ("RDMO" v1), with the key files next to it.

Flow on the Dreamcast:

1. `menus_run_frame` returns `MENU_REQ_DEMO` (index 0..3, the original rotation).
2. main.c calls `demo_start(index, &info)`. This sets `g_dcp_world` (zone, act,
   mode 3, flags 4|8, start point, random seed) and loads the key stream.
3. main.c loads the stage at the start point, then calls `demo_after_stage_load()`
   (seed again, lead frames).
4. Every frame: `demo_input_frame(&real, &pad)` gives the recorded pad in place of
   the real one (`pad.in`: InputState for the player module), then the stage
   frame, then `demo_play_frame(&real)`.
5. `demo_play_frame` returns `DEMO_EXIT_TITLE` (A/B/START/X/Y pressed: the DS
   skip keys) or `DEMO_EXIT_SEGA_LOGO` (frame limit). main.c calls `demo_stop()`
   and `menus_stage_exit(MENU_EXIT_DEMO_END or MENU_EXIT_DEMO_TIMEOUT, ...)`.

The "DEMO PLAY" / "PRESS START" label (dm_demoplay_<lang>, blinking with
timer bit 0x40) and the stage changes for flag 8 (no title card, timer starts at
once, boss navigator on boss acts) are in the UI module (UI.md).

Frame exactness: the stream is replayed through the same `Pad_Update` logic as
the DS (held / pressed / released / repeat). On the DS, DemoInput also runs
during the stage loading frames, which depend on card timing; those frames are
`DEMO_LEAD_FRAMES` (default 0, `demo_set_lead_frames`). If a demo drifts on the
DC, this is the value to tune.

## 11. VS mode (not ported)

VS (`src/game/VsMode`, the Wireless menus, `TimeSelect`, OptionsWfsHost,
DownloadPlay) is the two-player race over DS wireless: two DS units, each with a
cartridge (or one of them by download play). The menus pair the two units,
pick the stage and the time (TimeSelect), and the stage runs on both with the
other player's position sent every frame. The Dreamcast has no second unit to
pair with and no wireless layer; a split-screen VS would be a new game mode, not
a translation. The title menu treats VS as declined (as before), and the VS
record page (Records, section 13) only shows what the save file holds.

## 12. Extra Zone results (ExtraAct)

`menus/mn_extraact.c` (37 functions of `src/game/Zone/Extra_Zone/Results`),
API in `menus/extraact.h`:

```c
extraact_set_model_dir("/cd/models");                 /* default */
if (extraact_start(time, rings, character) == 0) {    /* boss_extra_result() values */
    while (!extraact_run_frame(&in))                  /* 1 when the scene is over */
        extraact_draw();                              /* opens its own PVR lists */
    extraact_shutdown();
}
```

It runs after the Extra boss (CS_SCRIPT 2), before the extra ending. The scene:
fade from white, the finish model (eff_dmg_end01/02, 510-frame animation),
the letterbox bars, the title and caption, the time / ring bonus tally, the total
and the rank stamp, then a white fade. A/START skip like the DS. Tables come from
`build/dreamcast/menus/extraact.bin` (menudata.py). Composition: one 640x480
frame (the DS draws this scene on the main screen only).

## 13. Time attack, Try Again, Records (PLAYER DATA)

- `mn_timeattack.c` (TimeAttackMenu_*, 62 functions): mode 0 acts, mode 1
  bosses, modes 2/3 view only (from Options PLAYER DATA). Records come from the
  save file. Starting a stage returns `MENU_REQ_STAGE` with mode 2.
- `mn_tryagain.c` (TryAgain_*, 31 functions): after a time-attack clear
  (`UI_EXIT_TRY_AGAIN` → `menus_stage_exit(MENU_EXIT_CLEARED, ...)`): retry,
  change act, character select, quit.
- `mn_records.c` (Records_*, 46 functions): the VS record page (totals and the
  last 10 opponents with their names, drawn with font_ipl_8x8 through
  `Font_GetGlyphIndex`'s tables, `build/dreamcast/menus/font8.bin`).
  `menus_set_owner_name(utf16, len)` sets the owner's name (the DS reads it from
  the firmware).
- Options: PLAYER DATA is the first item; BATTLE PLAY RECORD → Records,
  TIME ATTACK RECORD → ACT / BOSS → TimeAttackMenu mode 2 / 3.

Compositions (MnComp, section 3): TimeAttackMenu and Records use the sub screen
(art and plate) as the 2.5x backdrop with the main-screen panel at 1.5x on the
left; TryAgain uses the main screen as the backdrop with the sub-screen choices
at 1.4x. The per-line BG scroll (HDMA) of these screens is drawn as bands
(`mn_emit_bg_part`).

## 14. Zone-map navigator (NaviBg / NaviMsg mode 0 / NaviM)

`menus/mn_navi.c`. On the DS the zone map is on one screen and the navigator
(Tails in Sonic's story, Cream in Blaze's) on the other: a face with idle,
blink and talking sequences, a speech balloon with story hints per map event
(ZoneMap_SetNaviMessage), and the NaviM panel with the lives counter, the story
step counter and the seven emerald slots (new emeralds grow in with a sparkle).
It is unlocked after the first steps of each story (`gSaveData.unkC >= 3` /
`unk64 >= 4`).

On the Dreamcast the map fills the frame and the navigator pieces are placed
over it (`mn_navi_compose`, scale 1.5): the balloon and message at the top
left, the face at the bottom right (both only while the DS shows them, with
the original 30/60-frame fades), and the NaviM panel at the top right. The
navigator frame background is left out.

Names: the UI module has the boss navigator (mode 1) under the same DS names,
so the menus copies are renamed by macro in mn_internal.h (`MnNaviMsg_*`,
`MnNaviM_*`), as with the sound calls.

Data (menudata.py `navi()`, from the owner's ROM): `bg/<c>_m_b.mnbg`,
`bg/m_bal_<c>.mnbg`, `bg/navi_<c>_<lang>_NN.mnbg` (27/28 message panels per
language, loaded one at a time when the panel changes) and
`navi/faces_<c>.bin` (NVFM v2: the face frames of the zone-map sequences 0..8,
RLE-packed, with the 9 distinct palettes and the 14 sequences; 360 KB / 405 KB
in RAM while the zone map is up, one frame expanded into the 16 KB face
texture when it changes). About 3 MB on the disc.

Wired: ZoneMap_Start → ZoneMap_InitNavi + NaviM_Create; story steps →
NaviMsg_StartIntro; ZoneMap_CheckNewEmerald → NaviM_CheckStepAdvanced /
NaviM_SetNewEmerald; ZoneMap_State_WaitNaviMessage → NaviM_StartCounter /
StartEmeraldAppear / IsCounting; ZoneMapPause → message 5;
Game_StartStoryStage → NaviM_SaveStep; act select →
ZoneActSelect_ShowIntroNaviMessage (create), ShowNaviMessage4 (act chosen) and,
at the end of its fade-in, NaviM_StartEmeraldAppear / StartCounter (flags 8 /
0x10 from ZoneMap_CreateState). The act select keeps the navigator pieces in
its composition.

## 15. Test-build switches and Flycast results (2026-11-22)

Files in `/cd/debug/` (only test discs have them; nothing happens without them):

- `menutrace.txt`: serial trace of the menus (scene changes, requests,
  navigator panel / face / region count).
- `menusave.txt` "flags sonic_progress blaze_progress": ORed / raised into the
  save after boot (for example `256 5 0` unlocks the time-attack page and puts
  Sonic's story at step 5, where the navigator is unlocked).
- `demolead.txt` "N": attract-demo lead frames, plus a serial trace of the player
  (position every 30 frames, lives).

Flycast (own display :96, own home so the VMU is private), serial traces only:

- Title idle → attract demo 1 → START skips it after 125 frames → title screen.
  Demos 1 and 2 run to their frame limit and exit to the SEGA logo.
- Zone map (story step 5): navigator tasks up, face fades in after the talk
  demo, map message panel 21 shown for 180 frames and hidden, the NaviM panel
  region present.
- Title menu → time attack → PlayerSelect → TimeAttackMenu → A: stage request
  zone 0 act 0 mode 2, stage loads. The stage could not be finished from a
  scripted keyboard, so TryAgain was not reached in Flycast (host test only).
- ExtraAct needs the whole extra story; host test only.

DEMO_LEAD_FRAMES: a sweep of 0, 8, 16 and 32 lead frames gives the same result
for demo 1 (Leaf Storm act 1 from x 17048): the player reaches x ≈ 18200 and
then rolls back and forth between x ≈ 17400 and 18200 until the frame limit,
while the recording holds right and boost from frame 815. Since the lead value
does not change this, the drift is in the stage (player physics or a missing
object near x 18200, y 3150-3250), not in the replay timing. DEMO_LEAD_FRAMES stays 0.

## 16. Whole-story flow test and fixes (2026-11-22, second pass)

- `make -C dreamcast/game/menus story-test` (`test/test_story.c`): the real menus, save and
  cutscene modules on the owner's data, driven by a model of main.c's scene flow (with the
  patches in `menus/patches/`). Every act is cleared at once; the zone map is walked by the
  test autopilot. It checks: Sonic from a new save through zones 0..6 in order, script 0
  before and script 1 after every boss, the nine map talks 0x15..0x1d, DemoScript 4, the
  final zone (zf3, no script 1), the ending, staff roll, PostStaffRoll notices (flags 0x300),
  then the SEGA logo; Blaze's order 3 0 2 1 5 4 6 7 and talks 0x28..0x2f to progress 28;
  the extra story (script 3, script 2, ExtraAct, extra ending, flag 0x1000); the zone-map
  BGM after every return from a stage, talk or script; game over (lives 2 saved), pause
  quit (lives kept), time attack without a clear (no record). 124/124.
- Fixes in this module: `SysSound_PlayBGM` restarts after `SysSound_Release` / a stage /
  `NNS_SndStopSoundAll` (the map was silent after every talk and stage);
  `menus_restart_bgm()`; PostStaffRoll unlock notices follow the original conditions and
  exits, `menus_post_staff_roll()`; `MENU_EXIT_TRY_AGAIN`; story game over saves the lives
  (Stage_ExitAfterFail). Test-only: the zone-map / player-select autopilot
  (`debug/automap.txt`, `g_mn_automap`), the 4th `menusave.txt` value (Sonic's emeralds),
  `menus trace boot save`, `menus trace exit` and `time records` lines.
- Flycast runs and the defect list: `docs/dreamcast/DEFECTS.md`.

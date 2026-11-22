# Native cutscenes (Dreamcast port): translation notes

Status: every story cutscene of Sonic Rush runs natively. That covers TalkDemo (the 40 dialogue
scripts in six languages, over the zone map, a stage or a boss), PlayerDemo with PolyDemoUnder2
(the 34 scripts, 54 3D model scenes, and their captions), TransformDemo (the 5 Super transformations),
DemoScript (the 69 story script entries that chain them), Ending (Sonic and Blaze), ExtraEnding,
StaffRoll (Sonic, Blaze, extra) and PostStaffRoll. The code was translated from
`src/game/Cutscenes/` and keeps the original state machines, task order, timings, camera, lights,
animations, dialogue, portraits, fades and sound cues. No cutscene function is an asm fallback:
every one has C source in `src/`.

The code was checked as follows (section 9 has the details):

- Host test: `make -C dreamcast/game/cutscene host-test` passes (2683/2683 checks). It runs
  every script headless and checks the command sequence, frames, page counts and results.
- Build: all sources compile with `kos-cc -O2 -fwrapv -Wall -Wextra -Werror` (KOS 2.3.0).
- KOS link test: `make link` links the module with the engine (render, input, audio) and the
  sound module into `cutscene_test.elf`.
- Flycast: that ELF was run on a test disc (`tools/build_test_disc.sh`): a zone-map talk,
  TransformDemo 0 and 4, PlayerDemo 1 and 21, the Blaze ending into its staff roll, the Blaze and
  extra staff rolls, and the ExtraEnding chain. Section 9 lists what was seen and what only the
  serial log shows.

Nothing was run on Dreamcast hardware.

- Code: `dreamcast/game/cutscene/`, which holds `cutscene.h` (the public API), `cs_internal.h`,
  `cs_*.c`, `tools/`, `test/` and the `Makefile`.
- Converters (BYOR, outputs never committed): `tools/csdata.py`, `tools/csbank.py`.
- Test disc: `tools/build_test_disc.sh` builds a separate disc around `cutscene_test.elf`.
- Source of truth:
  - `src/game/Cutscenes/{TalkDemo,PlayerDemo,PolyDemo,TransformDemo,DemoScript,Ending}`.
  - `src/game/Menus/ZoneMap/ZoneMap_State_PlayTalkDemo.c`, `ZoneMap_SelectTalkDemo.c`, `ZoneMap_Unk_ov001_022e1aac`.
  - `src/game/GameSystem/SceneChange_*`, `Game_StartStoryStage`, `Game_AdvanceActWithDemo`, `Game_StartDemoScript3AtZone8Act2`.
  - `src/game/Stage/Stage_OnBossCleared.c`, `src/game/Cutscenes/ClearDemo/ClearDemo_GoToNextScene.c`.
- Third-party code: none.

## 1. Files

| file | content |
|---|---|
| `cutscene.h` | API: `cutscene_start`, `cutscene_run_frame`, `cutscene_draw`, `cutscene_draw_list`, `cutscene_shutdown`, `CsKind`, `CsNext`, `CsStory`, `CsResult`, `cutscene_zonemap_talk_id` |
| `cs_internal.h` | DS services named as on the DS (gPad, AnimatorSprite, BrightnessFade, master brightness), images, textures, quads, 3D, sound, module entry points |
| `cs_core.c` | file loading, `csdata.bin` sections, ROM sin/cos table, CSI1/CSP1 images, textures with dirty-row uploads, DSPR sprites + AnimatorSprite, the quad list and DS→640x480 views, affine BG quads, BrightnessFade |
| `cs_draw.c` | KOS: PVR submission (OP backdrop, 3D lists, 2D quads in TR at z 100..120, brightness overlay); host: PVR size asserts |
| `cs_3d.c` | models (`dreamcast/game/model`), joint/visibility animations, DS camera (look-at + frustum from the DS sin/cos), DS lights |
| `cs_sound.c` | per-cutscene sound bank, sequence and STRM music, SE / voice handles, BGM fades |
| `cs_main.c` | API, task order, scene changes (`SceneChange_*`), zone-map talk ids, pad mapping |
| `cs_talk.c` | TalkDemo (scene, window band, dim, rocks, BG1 box, BG2 portrait, message pages, sounds, skip, MarkSeen) |
| `cs_player.c` | PlayerDemo (script ops, model scenes, lights, captions, camera shake, repeat SE, skip) + PolyDemoUnder2 |
| `cs_trans.c` | TransformDemo (models, effects, camera, light flash, fades, sounds) |
| `cs_script.c` | DemoScript (script table, op chain, delays, next scene, progress) |
| `cs_ending.c` | Ending, ExtraEnding, StaffRoll (+ picture states, credits, banner, closing card), PostStaffRoll |
| `tools/csdata.py` | ROM tables → `csdata.bin`, scripts, images, sprite/model/music lists (section 4) |
| `tools/csbank.py` | the sys SDAT split into one sound bank per SSAR (`sys_aNN.srb`) |
| `tools/build_test_disc.sh` | emulator test disc (section 9) |
| `test/test_cutscene.c` | host test; `test/cs_raster.h`: host compositor (PPM frame dumps) |
| `test/link_main.c` | KOS test main: plays a list of cutscenes (`/cd/cutscene/test.txt`) |

## 2. Integration (main.c, menus hooks, bosses)

`main.c`, the game `Makefile` and `build_disc.sh` belong to the integrator. These are the changes.

### 2.1 Build and boot

- Add the module's `cs_*.c` to `OBJS`: `$(patsubst %.c,%.o,$(wildcard cutscene/cs_*.c))`.
  `model/model.c` is already linked.
- `menus.h` gained two fields at the end of `MenuHooks` (section 2.4). Rebuild every object that includes it.

```c
#include "../cutscene/cutscene.h"
/* once, after audio_init / sound_init */
cutscene_set_paths("/cd/cutscene", "/cd/sprites", "/cd/models", "/cd/music");   /* the defaults */
```

### 2.2 Story state and the per-frame calls

Before every `cutscene_start`, fill `CsStory` from the save and game state:

```c
CsStory st = { .character = gGameState.unk0, .zone = gGameState.unk1C,
               .stage_new = StoryProgress_IsStageNew(), .save_flags = gSaveData.unk8,
               .sonic_progress = gSaveData.unkC, .blaze_progress = gSaveData.unk64,
               .language = menus_language(), .sonic_emeralds = gSaveData.unkF & 0x7f };
cutscene_set_story(&st);
cutscene_set_return_bank(stage_sdat_or_NULL);   /* bank to reload when a cutscene that loaded a sys bank ends */
if (cutscene_start(kind, id, character) != 0) { /* data missing: continue as if it had ended */ }
```

Run each cutscene as follows.

- **Full-screen cutscenes** are PlayerDemo, TransformDemo, DemoScript, Ending, StaffRoll and
  PostStaffRoll (`cutscene_is_fullscreen()` is 1). Use them in place of the stage or menu frame:
  ```c
  input_poll(&in);
  done = cutscene_run_frame(&in);
  sound_update();
  render_begin_frame();
  cutscene_draw();            /* opens and closes OP, PT, TR itself */
  render_end_frame();
  audio_update();
  ```
- **Overlays** are TalkDemo over the zone map, a stage or a boss (`cutscene_is_fullscreen()` is 0).
  - The caller keeps drawing its scene.
  - Freeze the world (no `boss_update` / player / objects) while `cutscene_active()`, as `System_Freeze` does on the DS.
  - Call `cutscene_run_frame(&in)` once per frame.
  - Call `cutscene_draw_list(PVR_LIST_TR_POLY)` inside the caller's TR list, after the scene and HUD. Its 2D is at z 100..120, above `menus_draw_tr` (z 10..90).
  - It may also be called for OP and PT; it adds nothing there for a talk.
- When `cutscene_run_frame` returns 1:
  - Read `cutscene_result()` and apply it (section 2.5).
  - Call `cutscene_shutdown()`. It frees textures, models and sound handles and reloads the return bank.

Pad mapping (`cs_main.c`):

| Dreamcast | DS |
|---|---|
| A | A |
| B | B and R (TalkDemo fast-forward) |
| START | START (skip PlayerDemo, Ending and StaffRoll, as on the DS) |
| X / Y / D-pad | X / Y / D-pad |

The touch-screen skip buttons are drawn but are not touch targets.

### 2.3 Triggers: every cutscene, where the original starts it, what main.c calls

| original call site | `cutscene_start(...)` | data used | music / sound | then |
|---|---|---|---|---|
| `ZoneMap_State_PlayTalkDemo` (story steps: Sonic 0,6,9,12,15,19,24,28,31 → talks 0x15..0x1d; Blaze 1,4,7,15,19,23,27,30 → 0x28..0x2f) | through the menus hook (2.4): `CS_TALK_MAP, cutscene_zonemap_talk_id(c, step), c` | `talk/<name>_<lang>.btk`, `talk/img.csp`, `talk/msg_<lang>.csp`, sprites `act__ac_gmk_rock`, prompt `ac_sf*` | script BGM (`sNN.wav`), `sys_a04.srb` (page SE / voices) | menus continue (map BGM 4/5) |
| `Game_StartStoryStage` / `Game_AdvanceActWithDemo` → `DemoScript_StartScript0` (boss act of zones 0..6; the menus currently request the boss stage directly) | `CS_SCRIPT, 0, c` (set `st.zone`, `st.stage_new`) | `csdata.bin` ds_table; TransformDemo / PlayerDemo / boss talks (ids 1..9: groups 16/17/18 = `sys_a12/13/14.srb`) | `sys_a11.srb` (transform), PlayerDemo SSARs | result `CS_NEXT_STAGE`: load the boss act |
| `Stage_OnBossCleared` → `DemoScript_StartAfterBoss` (story, zone ≠ 7) | `CS_SCRIPT, 1, c` (zone 8 = final: the code picks mode 2) | as above | as above | `CS_NEXT_CLEARDEMO`: show the results (UI_RESULTS), then `menus_stage_exit`; `CS_NEXT_EXTRA_ACT` (mode 2) / `CS_NEXT_ENDING` per the script table |
| `Boss7_Action_StartTalkDemo` (Dead Line rival, `boss7_set_talk_hook`) | `CS_TALK, 0xa` (Sonic) / `0xb` (Blaze) | talk scripts | none of its own (no sound group in the ROM table: the stage bank stays) | boss continues (freeze while active) |
| `BossFPartD8_State_WaitTalkDemo` (`g_boss_hooks.talk`) | `CS_TALK, 0xf` / `0x10` | talk scripts | none of its own (stage bank) | boss continues |
| `ClearDemo_GoToNextScene` zone 7 → `SceneChange_ToEnding` (menus request "Ending") | `CS_ENDING, 0, c` | `bg/ed_final_*`, `ed_base_*`, sprites `ac_sf*`, `ed_*` | `sNN.wav` of SEQ_FINAL_SONIC; then the `Final_Roll` STRM | chains Ending → StaffRoll → PostStaffRoll; result `CS_NEXT_UNLOCK_NOTICES` |
| `ClearDemo_GoToNextScene` zone 8 → `SceneChange_ToExtraEnding` (menus request "ExtraEnding") | `CS_EXTRA_ENDING, 0, c` | PlayerDemo 0x2a, `bg/extra_ed_*`, `ed_extra_end` | `extra_end.wav`, then the `Extra_Roll` STRM | chains → extra StaffRoll → PostStaffRoll |
| `ClearDemo_GoToNextScene` zone 6 boss → `DemoScript_StartScript4` (menus request "DemoScript4") | `CS_SCRIPT, 4, c` | script table | | `CS_NEXT_ZONEMAP` (the menus already switched to the map; run the script first, then the menus) |
| `PlayerSelectMenu_ExitToGame` → `Game_StartDemoScript3AtZone8Act2` (extra story; the menus start zone 8 act 2 directly) | `CS_SCRIPT, 3, c` with `st.zone = 8` | script table | | `CS_NEXT_EXTRA_BOSS`: load zone 8 act 2 |
| `PlayerDemoOp_StartTalkDemo` / DemoScript op 3 | internal | | | |
| `SceneChange_ToStaffRoll` / `ToExtraStaffRoll` / `ToPostStaffRoll` | internal (scene chain); `CS_STAFFROLL, 0/1/2` and `CS_POST_STAFFROLL` can also be started directly | | | |

When the menus report `MENU_REQ_UNAVAILABLE`, `r.scene` is `"Ending"`, `"ExtraEnding"` or `"DemoScript4"`.
Start the cutscene in the table above. The menus have already moved on (title screen, zone map),
so run the cutscene to its end before calling `menus_run_frame` again.

After `CS_NEXT_UNLOCK_NOTICES`, keep running the menus. `TitleScreen_GoToUnlockNotice` in `mn_title.c`
shows the PostStaffRoll unlock notices (player select, sound test) when the save says so.

### 2.4 Menus hook (the only menus/ edit)

`dreamcast/game/menus/menus.h` (`MenuHooks`), 2 fields:

```c
int (*talk_demo_start)(int character, int step);   /* TalkDemo_CreateForZoneMap; 0 = started */
int (*talk_demo_active)(void);                     /* TalkDemo_IsActive */
```

`dreamcast/game/menus/mn_zonemap.c`, `ZoneMap_State_PlayTalkDemo`: when both hooks are set, it follows
the original.

1. At frame 0xf: `SysSound_Release` (`stop_all` hook) and the character hidden.
2. At frame 0x10: start the talk for the map step (`unk4` Sonic / `unk8` Blaze).
3. Wait until the talk is inactive.
4. 15 frames later: the navi message (1), the map BGM (4, or 5 for the last steps), next state.

Without the hooks it keeps the old 32-frame skip. The menus host test passes either way (38/38). With
test hooks, the first map visits call `talk_demo_start` with steps 0, 6 and 9.

```c
static int menu_talk_start(int c, int step) {
    /* + cutscene_set_story(&st) as in 2.2 */
    return cutscene_start(CS_TALK_MAP, cutscene_zonemap_talk_id(c, step), c);
}
static int menu_talk_active(void) { return cutscene_active(); }
g_menu_hooks.talk_demo_start = menu_talk_start;
g_menu_hooks.talk_demo_active = menu_talk_active;
/* front-end frame: menus_run_frame(&in); if (cutscene_active()) cutscene_run_frame(&in);
 * TR list: menus_draw_tr(); cutscene_draw_list(PVR_LIST_TR_POLY); */
```

The original `unk19ac` (map BG fade) and `unk1998` are not part of the port's map composition.

### 2.5 Results → save

| `CsResult` field | original | apply with |
|---|---|---|
| `progress` (≥ 0) | `DemoScript_Finish` when the entry's flags & 1 | `SaveData_SetSonicProgress` / `SetBlazeProgress` (by character) + store lives |
| `set_save_flags` | `TalkDemo_MarkSeen` (0x20000 talk 0xa, 0x40000 0xb, 0x80000 0xf, 0x100000 0x10) | `SaveData_SetProgressFlags(SaveData_GetProgressFlags() \| f)` |
| `set_sonic_progress` / `set_blaze_progress` (≥ 0) | `StaffRoll_State_Finish` (0x1a / 0x1c) | `SaveData_SetSonicProgress` / `SetBlazeProgress` |
| `game_cleared` | `StaffRoll_State_Finish` mode 2 | `SaveData_SetGameCleared` |
| | (then the original commits) | `SaveData_CommitOrShowError` |
| `next` | the `SceneChange_*` of the original | table 2.3 |

## 3. Single-screen composition (one 640x480 frame)

There is one continuous view. Nothing is stacked or placed side by side, nothing is drawn twice,
and there is no divider. The DS main (top) screen is the frame at x2.5 (256x192 → 640x480). What
the bottom screen carried is composed into it at its original art, fonts and colours, never
stretched (uniform scale only).

| cutscene | DS screens | Dreamcast frame |
|---|---|---|
| TalkDemo | Main: window band (WIN0/WIN1), BG1 text box, BG2 affine portrait or full image, OBJ prompt / rocks, dim of the scene below. Sub: the caller's scene (map, stage). | One frame: the caller's scene, then the dim (BLDY 0→8/16), the band, the box and the portrait at x2.5 over it. Band rows come from the DS V-alarm limits; OBJ is clipped outside the band as WIN0/1 does. |
| PlayerDemo + PolyDemoUnder2 | Main: 3D scene (camera node, lights), BG3 caption strip 256x48 at y 144. Sub: logo panel + PRESS START skip button. | 3D at 4:3 full frame, captions at the bottom (x2.5). The skip button (sub (64,118)-(192,148)) is at x1.5 in the top-right corner while skippable. The decorative logo panel is not shown: it carries no information. |
| TransformDemo | One 3D scene rendered to both screens (capture of the two halves, `SetupDualScreenFrameA/B`). | One camera: eye (0,0,25) looking at (0, cam_y, 0), tracking the character's height, so the rising character stays in view. Effects, flashes and fades as the original. |
| Ending | Main: picture (BG1) + caption strip (BG0 y 144), BG2 slide / BG3 affine extras. Sub: logo panel, PRESS START button. | Main at x2.5. The skip button is top right. |
| StaffRoll | Main + sub: the credits column scrolls through both screens; running characters and logos on main; the closing SEGA / licence card on sub. | The main window of the column (credits enter from the bottom edge, where the sub screen was), characters and logos as on main. The closing card (sub (0,16)-(256,160)) is at x1.25 at (160,80) between the final logo and the banner. The skip button is top right. The sub screen's copy of the column (maps 7..9) is not loaded. |
| PostStaffRoll | TalkDemo 0x37 over black, then the menus' unlock notices. | Same. |

## 4. Data (all built from the owner's ROM; never committed)

```sh
make -C dreamcast/game/cutscene data     # = csdata.py + csbank.py (~20 s)
```

The converters need the outputs of the other converters first:

- the sprites in `build/dreamcast/sprites/bac` with `manifest.json` (bac2dc);
- the models in `build/dreamcast/models` with its manifest;
- the audio in `build/dreamcast/audio` (music manifest, STRMs) and `extract/eur`.

`csdata.py` checks that every referenced sprite and model exists (300 models, 0 missing).

| output | format |
|---|---|
| `csdata.bin` | "RCSD" v1: u32 magic, u16 version, u16 count, count × {char name[12], u32 off, u32 size}. Sections: `sincos` (ROM 0x0206E470), `td_*` (TalkDemo: names, message bases per language, portrait / frame tables, BGM, sounds, enter / exit motions, rocks, prompt sprites), `ds_table` (DemoScript 0x45 × 32 bytes), `pd_*` (PlayerDemo: script index, demos, scenes with model files and flags, lights, sounds, BGM, voices, talks, fades, captions), `tr_*` (TransformDemo ids, models, lights), `ed_*` (Ending pictures, sprites, table 0x02315EB4, timed captions), `sr_*` (StaffRoll BG lists, sprites, table 0x023160DC). Layouts are next to each `sec()` in `csdata.py`. |
| `talk/<name>_<lang>.btk` | #BTK TalkDemo scripts, unchanged (header 0x1c, pages of 13 s32) |
| `talk/img.csp` | CSP1 pack of the 92 images of `/bg2/talk/talk_m.bb` (portraits 80x80, frames 176x80, full images 256x56) |
| `talk/msg_<lang>.csp` | CSP1 pack of the message pages of `talk_m_<lang>.bb` (128x56). Read one entry at a time, as `TalkDemo_ReadMessage` does. |
| `pldm/<name>.bpd`, `pldm/poly_m.csp` | #BPD PlayerDemo scripts unchanged; caption / panel images |
| `bg/<name>.csi` | CSI1 indexed pictures of Ending / StaffRoll |
| `bg/<name>.csr` | CSR1: the three 256x9600 credit maps, row-compacted (only the non-empty rows; 0.87 MB together instead of 7.4 MB), read in one file read and streamed row by row into 256x256 ring textures |
| `sound/sys_aNN.srb` | sys SDAT split per SSAR (4, 8..15), same SSAR / sequence ids as the DS (`csbank.py`; the whole sys SDAT has 4.8 MB of waves and does not fit the AICA) |
| `sprites.txt`, `models.txt` | the sprite ids / model files the disc needs |
| `seqalias.txt`, `music.txt` | sys sequences that are byte-identical duplicates (15→11, 16→13, 22→8) play the converted copy |
| `preview/*.png` | previews (not for the disc) |

CSI1: "CSI1", u16 w, h, type, flags, npal, pofs, u16 pal[npal] (DS BGR555), u8 px[h][w].
CSR1: the same header with magic "CSR1" and the palette, zero-padded to 4 bytes, s32 rowoff[h]
(-1 = empty row), then the non-empty rows of w bytes.
4-bpp tiles are flattened to bank*16+value, and 0 = transparent. CSP1: u32 count, u32 off[count], CSI1 blobs.

### 4.1 Disc layout

`tools/build_test_disc.sh` is the reference. The game disc needs the same lines in `build_disc.sh`.

| disc path | from |
|---|---|
| `/cd/cutscene/csdata.bin`, `seqalias.txt`, `talk/`, `pldm/`, `bg/` | `build/dreamcast/cutscene/` (~36 MB, mostly `talk/`) |
| `/cd/sprites/<id>/frames.bin + atlas*.argb1555` | each id of `build/dreamcast/cutscene/sprites.txt` |
| `/cd/models/<file>` | each file of `build/dreamcast/cutscene/models.txt` |
| `/cd/sound/sys_aNN.srb` | `build/dreamcast/cutscene/sound/` |
| `/cd/music/sNN.wav` + `sys.txt` | the sys-SDAT music as `build_disc.sh` already copies it |
| `/cd/music/Final_Roll.wav`, `Extra_Roll.wav` | `build/dreamcast/audio/strm/sys/` |
| `/cd/music/extra_end.wav` | `build/dreamcast/audio/music/extra_end/SEQ_EXTRA_END.wav` |

## 5. Sound

- BGM: `Sound_PlaySeq(seq)` of the sys SDAT streams `/cd/music/sNN.wav` with the loop points of `sys.txt`.
  - The staff rolls use the STRMs `Final_Roll` / `Extra_Roll`; the extra ending uses `extra_end.wav`.
  - BGM fades (`NNS_SndPlayerStopSeq(.., fade)`, `NNS_SndArcStrmStop(.., 0x12c)`) stop the stream after the fade time. The stream does not ramp its volume.
- SE / voices: each cutscene loads its sys-SDAT group as on the DS (`TalkDemo_LoadSound`, `PlayerDemo_LoadSound`, `TransformDemo_LoadSound`).
  - Here that means the native bank of its SSAR: `sound_load_bank("sys_aNN")`.
  - `Sound_PlaySeqArc` / `Sound_PlayVoice` get the original (arc, id).
  - At the end, the return bank (`cutscene_set_return_bank`) is reloaded.
- TalkDemo page sounds are timed by scene phase, and PlayerDemo has a repeating SE (op 7). Both are as in the original.

## 6. How the original is mapped

- **Tasks**: one state machine per DS task, run in DS priority order each frame:
  DemoScript (0x100), BrightnessFade (0x3e00), PlayerDemo and its subtasks (0x3efe), TransformDemo
  (0x4000), Ending / ExtraEnding / StaffRoll (0x5000), TalkDemo (0xeffd..0xf00a). A task created
  during the frame first runs on the next frame, as on the DS. Every intermediate state of the
  original (`Begin*`, `*Motion`, `Skip*`) is kept, so frame counts match.
- **Display**: BG layers become textures. BG1/BG2/BG3 affine maps use the inverse of
  `G2_SetBGxAffine`. Windows become clipped quads, and master brightness is a full-frame overlay.
  Palette fades re-expand the texture rows that changed. All textures are power-of-two, non-twiddled ARGB1555.
- **3D**: `dreamcast/game/model` with DS-equivalent matrices: the camera from the `cam` / `int`
  joint nodes, `fovy 0x11c7`, 4:3, near 1, far 2048. Lights come from the ROM tables, turned by the view.
- **Sprites**: the DSPR sprites (bac2dc) through an AnimatorSprite port (frames, loops, flips, palettes, brightness).

## 7. Function mapping

Per directory: the total count, and how many are found by name in `cs_*.c` (as a definition, or as a
comment on the line that does their work):

| directory | functions | named in the port |
|---|---|---|
| TalkDemo | 145 | 121 |
| PlayerDemo | 107 | 82 |
| PolyDemo | 11 | 6 |
| TransformDemo | 47 | 29 |
| DemoScript | 25 | 12 |
| Ending (Ending, ExtraEnding, StaffRoll, PostStaffRoll) | 178 | 146 |
| ClearDemo | 78 | owned by the UI module (`ui_results.c`, 65 named there) and `menus/mn_main.c` (`ClearDemo_GoToNextScene`) |

The others, and where their work is:

| original | port |
|---|---|
| `*_GetCompressionType`, `*_GetUncompressedSize` (TalkDemo, PlayerDemo, PlayerDemoBg, PlayerDemoModel, PolyDemoUnder2, TransformDemo) | the converters decompress offline |
| `*_InitDisplay`, `*_RestoreDisplay`, `TalkDemo_InitDisplayZoneMap`, `TalkDemo_RestoreDisplayZoneMap`, `PostStaffRoll_InitDisplay` | DS VRAM / register setup; the compositions are in the `*_Draw` functions |
| `*_Destructor`, `*_ReleaseAssets`, `*FreeRes_Main`, `*_CreateFreeResTask`, `PlayerDemo_FreeData`, `PlayerDemo_ReleaseSkippable`, `TransformDemo_Release*`, `TalkDemoRocks_Release`, `TalkDemo_FreeScript`, `TalkDemo_DestructorDummy` | `*_Kill` / `*_Destructor` / `cutscene_shutdown` |
| `PlayerDemo_Create`, `PlayerDemo_Main`, `TransformDemo_Create`, `TransformDemo_Main`, `TransformDemo_RunFadeState`, `DemoScript_Main`, `DemoScript_Destructor` | `PlayerDemo_Start` / `_RunFrame`, `TransformDemo_Start` / `_RunFrame` (fade states dispatched there), `DemoScript_RunFrame` / `_Finish` |
| `DemoScript_StartScript0/3/4`, `StaffRoll_Start`, `StaffRoll_StartExtra` | `cutscene_start(CS_SCRIPT, 0/3/4)`, `StaffRoll_Create(mode)` |
| `PlayerDemoBg_LoadImage`, `_LoadPaletteRes`, `_SetBg3Position`, `PlayerDemo_GetScriptHeader` | `PlayerDemoBg_LoadImageRes`, `_LoadPalette`, caption placement in `PlayerDemo_Draw`, `PlayerDemo_GetCommand` |
| `PlayerDemoLight_GetVectors`, `_GetVectorsAlt`, `_Update` | `PlayerDemoLight_Apply` / `_ApplyAlt` + `cs_lights_from_ds` |
| `PlayerDemoModel_OnCameraNode`, `PlayerDemo_GetCameraShakeOffset`, `PlayerDemoCameraShake_Destructor` | `PlayerDemoModel_UpdateCamera` (camera from the node + shake), `PlayerDemo_Kill` |
| `PolyDemo_BuildModelPath`, `PolyDemo_GetModelFlags` | resolved by `csdata.py` (`pd_scene`: model files + flags) |
| `TalkDemoWindow_InitVAlarms`, `_CancelVAlarms`, `_OnVAlarm*` | the window band rows are computed each frame from the same V-count limits (no raster interrupt) |
| `TalkDemo_ClearMapRect`, `_ClearImage10x10`, `_MirrorMapRect` | the box / portrait are drawn as quads (mirror = flipped texture) |
| `TalkDemo_GetScript`, `_GetMessageIndex`, `_GetLanguageSuffix`, `_LoadMessageBgFailDummy`, `TalkDemoDeferredCall_Main`, `TalkDemoRocks_Draw` | `talk/*.btk`, `TalkDemoScene_InitMessageIndex` (`td_msg`), `cs_lang_suffix`, deferred call in `cs_talk.c`, `TalkDemoRocks_Main` |
| `TransformDemo_InitCaptureOam`, `_CopyCaptureOam`, `_SetupDualScreenFrameA/B`, `_SetCameraFrameB` | the DS dual-screen capture; replaced by the single tracking camera (section 3) |
| `TransformDemo_DrawPlayerModel`, `_DrawTransModels`, `_SpawnEffect`, `_DestroyFade` | `TransformDemo_DrawPlayerModels`, `_DrawModels`, `TransformDemoEffect_Spawn`, `BrightnessFade_Destroy` |
| `Ending_DrawSprites`, `StaffRoll_DrawSprites`, `_DrawPictureSprites`, `_SetCreditSpritePos`, `_ToSubScreenY`, `_GetSpriteFrameSize` | `Ending_DrawMain`, `StaffRoll_Main` / `_Draw`, `StaffRollCredit_Main`, closing-card composition, `AnimatorSprite_FrameBox` |
| `StaffRoll_InitBackgrounds`, `_InitSprite*` | `StaffRoll_LoadAssets` (+ `staff_load`), `StaffRoll_InitSprites` |
| `StaffRoll_State_None`, `StaffRoll_EmptyCallback`, `ExtraEnding_Dummy` | empty (NULL state) |
| `*_HitTestSprite`, `Demo_IsSkipAreaTouched`, `PlayerDemo_EnableTouch`, `_DisableTouch` | touch: the skip is the START button (the DS also accepts START) |
| `PostStaffRoll_Show*UnlockNotice`, `_State_*UnlockNotice`, `PostStaffRollFade_Main`, `_CreateFadeTask`, `_State_GoToSegaLogo` | the menus module (`mn_title.c`), reached through `CS_NEXT_UNLOCK_NOTICES` |
| `DemoInput_*`, `DemoPlay_*` | the title-screen attract demos (recorded pad input in a stage), not cutscenes: not in this module (see 8) |
| `ZoneMap_State_PlayTalkDemo`, `ZoneMap_SelectTalkDemo`, `ZoneMap_Unk_ov001_022e1aac` | `mn_zonemap.c` hook (2.4) + `cutscene_zonemap_talk_id` |
| `SceneChange_ToEnding`, `_ToExtraEnding`, `_ToStaffRoll`, `_ToExtraStaffRoll`, `_ToPostStaffRoll`, `_ToStage`, `_ToExtraBoss`, `_ToExtraAct`, `_ToZoneMap_1_0` | `cs_scene_change` (internal chain) or `CsResult.next` |

## 8. Gaps and assumptions

- **3D materials**:
  - Texture-pattern, material-colour and texture-SRT animations (`nsbtp`, `nsbma`, `nsbta`) are not converted by the model converter. Joint and visibility animations are played.
  - Toon edge colours are not drawn.
  - Polygons crossing the near plane are dropped by the model renderer, not clipped.
- **BGM fades** stop the stream after the fade time; there is no volume ramp.
- **PolyDemoUnder2**: the decorative logo panel of the bottom screen is not composed. Only the skip button is.
- **TransformDemo**: one tracking camera replaces the two-screen capture. Composition differs from the DS by design.
- **ClearDemo** (act results) is the UI module's (`UI_RESULTS`). Its BG, model and time-attack rank parts are not translated there.
  - The order "after-boss DemoScript, then results" needs main.c (2.3). Today the UI shows the results right after the boss.
- **Attract demos** (`DemoInput` / `DemoPlay`) are not part of this module. The menus report `MENU_REQ_DEMO`.
- **Menus flow** (not edited, only reported):
  - `Game_StartStoryStage` / `Game_AdvanceActWithDemo` in `mn_story.c` request the boss act without `DemoScript_StartScript0`.
  - The extra story starts zone 8 act 2 without `DemoScript_StartScript3`.
  - main.c inserts these scripts (2.3).
- **Load stalls**: each cutscene loads its pictures, sprites and models when it starts, or at a
  PlayerDemo scene change, as the DS does. Under Flycast this takes several seconds of emulated time
  (~120 KB/s, section 9).
  - There is no loading screen or prefetch yet.
  - Candidates: smaller sprite atlases (`ac_sf` is 512 KB), loading the PlayerDemo scene models during the previous scene.
- **Memory**:
  - A cutscene loads its pictures and models when it starts and frees them at the end.
  - The staff roll holds 0.87 MB of credit rows (CSR1), plus 3 ring textures 256x256 and up to 12 BG pictures. Flycast measured 2.8 MB of heap in use during the extra staff roll.
  - The integrator should free stage data before full-screen cutscenes, as the DS overlay change does.

## 9. Verification (what was run)

- `make -C dreamcast/game/cutscene host-test`: 2683/2683 checks.
  - TalkDemo: all 40 scripts in English (page count, BGM, first page after the band opened at frame 34–40, full-image mode).
  - The same 40 scripts in the 5 other languages (page counts, message pages present).
  - PlayerDemo: all 34 scripts, every command at its frame (frame = scene start + max(t,1)) in order, scene changes.
  - TransformDemo 0–4 (sound cues at 0x1e/0x78/0xa0/0xb4/0x12c, fade-out, end frame).
  - DemoScript: 70 runs (modes 0/1 for both characters, all zones, new / replay; modes 2, 3, 4), checking op sequences, next scene and progress.
  - Ending (Sonic 9 / Blaze 8 captions, 4 pictures, chain to the staff roll and its progress result).
  - StaffRoll 0/1/2 (credits, extra pictures, game-cleared, no missing credit map in any of the 6 languages), skip in each mode, Ending skip.
  - PostStaffRoll (talk 0x37), ExtraEnding (PlayerDemo 0x2a → extra staff roll).
  - Zone-map talk ids for every story step.
  - With `DUMP=dir`, the host compositor writes PPM frames. These were looked at for every kind; that is how the staff-roll ring-texture bug was found and fixed.
- `make dc` (kos-cc -Werror) and `make link` (`cutscene_test.elf`) succeed.
- `make -C dreamcast/game/menus host-test`: 38/38, with and without the talk hooks. `make -C dreamcast/game/menus link` succeeds.
- Flycast, with `cutscene_test.elf` on `tools/build_test_disc.sh` output (serial log + screenshots of
  the emulator window; A taps sent with xdotool; times are the emulated clock, `timer_ms_gettime64`):
  - Zone-map talk 0x15: all 11 pages ran (serial log). The screenshot shows the portrait, box, band and A prompt composed correctly over the backdrop.
  - TransformDemo 0 and PlayerDemo 1: ran to their end (serial log only; their screenshots caught another window and are not evidence).
  - TransformDemo 4 drew Super Sonic and Burning Blaze with the effects.
  - PlayerDemo 21 drew the island scene with the models and captions.
  - The Blaze ending drew its pictures, captions and white fades, then chained into the staff roll.
  - In that first run the staff roll showed no credits and no characters: the six 2.4 MB credit maps did not fit in RAM. This was fixed by the CSR1 compaction and by not loading the sub-screen copies.
  - Then the Blaze staff roll (`CS_STAFFROLL 1`) and the extra staff roll (`CS_STAFFROLL 2`) showed the credits scrolling, with Blaze, Cream and Cheese running in the first.
  - ExtraEnding (`CS_EXTRA_ENDING`): PlayerDemo 0x2a with talks 0x38 / 0x39 over the 3D scene, fades, then the extra staff roll.
    - Talk 0x39 keeps running over the staff roll, as the original's tasks do: `Overlay_Request` destroys no task.
    - In one run the staff-roll part showed black screenshots while the emulator ran slowly (two other Flycast instances on a 2-core host). The rerun reached frame 6000 at 60 fps. The host compositor shows the talk over the credits at those frames.
  - Speed (emulated clock): once loaded, 600 frames take 10.0 s (60 fps) in the staff roll and ~10.8 s (~55 fps) in PlayerDemo 0x2a with a talk over it.
  - Loading is slow under emulation. The serial `cs: load` lines show ~120 KB/s from the emulated disc:
    - PlayerDemo 21 scene: ~9 s for 1.1 MB of models;
    - extra staff roll start: ~15 s (the 512 KB `ac_sf` atlas alone takes 4.3 s).
  - The DS loads the same files at the same points. Hardware read speed was not measured.
- Not done: Dreamcast hardware; the game ELF with these hooks (main.c is the integrator's).

/*
 * Attract demos (Dreamcast port): recorded pad input played back in a stage.
 *
 * Translated from src/game/Cutscenes/DemoScript/DemoInput_{Create,Main,Destructor}.c,
 * DemoPlay_{Start,Create,Main,Destructor}.c and GameSystem/SceneChange_ToDemoPlay.c.
 * See docs/dreamcast/translation/MENUS.md section 10 (attract demos).
 *
 * Frame order (main.c, MENU_REQ_DEMO; the DS task order DemoInput 1 < Stage_Main <
 * player 0x100 < objects < BrightnessFade 0x3e00 < DemoPlay 0x4000):
 *
 *     DemoInfo di;
 *     demo_start(r.demo, &di);                 // DemoPlay_Create (table, key file, gRandSeed, flags)
 *     ... load stage (di.zone, di.act), spawn the player at (di.start_x, di.start_y) ...
 *     demo_after_stage_load();                  // gRandSeed again + the DS loading frames (lead)
 *     per frame:
 *         input_poll(&real);
 *         DemoPad p;  demo_input_frame(&real, &p);       // DemoInput_Main (gPad for this frame)
 *         ui_update(&p.in, &player);                     // Stage_Main sees the recorded pad
 *         dcp_player_frame(&player, p.held, p.pressed, p.released);
 *         ...objects, boss, ui_post_update...
 *         int k = demo_play_frame(&real);                // DemoPlay_Main
 *         if (k != DEMO_RUNNING) { demo_stop(); menus_stage_exit(k == DEMO_EXIT_TITLE ?
 *                 MENU_EXIT_DEMO_END : MENU_EXIT_DEMO_TIMEOUT, 0, 0, emeralds, 0); }
 */
#ifndef SRDC_DEMO_H
#define SRDC_DEMO_H
#include <stdint.h>
#include "../engine/input.h"

typedef struct {
    int zone, act, character;   /* data_02073bd0 / bd1 / bd2: gGameState.unk1C, unk1D, unk0 */
    int start_x, start_y;       /* data_02073bd4 / bd6 (px): data_022c4590.unk1C / unk20, the
                                   Player_Create start point when gGameState.unk18 & 8. Both 0:
                                   the level's PlayerStart (no demo uses that) */
    int limit;                  /* data_02073bd8: DemoPlay_Main ends the demo after this many frames */
    uint32_t seed;              /* the key file's first word (DemoInput_Create: gRandSeed) */
    int records;                /* key file records up to the zero-duration end record */
} DemoInfo;

/* gPad of one frame in DS key bits (A 1, B 2, SELECT 4, START 8, RIGHT 0x10, LEFT 0x20,
 * UP 0x40, DOWN 0x80, R 0x100, L 0x200, X 0x400, Y 0x800): Pad_Update(gPad, keys) */
typedef struct {
    uint16_t held, pressed, released;   /* gPad cur / trg / rel: dcp_player_frame's arguments */
    uint16_t rep;                       /* gPad rep (key repeat, delay 20 / interval 2) */
    int recorded;                       /* 1: from the key file (gSystemFlags & 0x40), 0: the real pad */
    InputState in;                      /* the same keys as an InputState (ui_update, pause) */
} DemoPad;

enum { DEMO_RUNNING = 0, DEMO_EXIT_TITLE = 1, DEMO_EXIT_SEGA_LOGO = 2 };

/* Where demotab.bin and key/ live (default "/cd/demo"). */
void demo_set_path(const char *dir);
/* The DS runs DemoInput during the stage loading (StageStart_State_*: file requests,
 * timed by the card). The port loads the stage between frames, so these frames are
 * consumed by demo_after_stage_load. Default DEMO_LEAD_FRAMES (MENUS.md 10.3). */
#define DEMO_LEAD_FRAMES 0
void demo_set_lead_frames(int n);
int demo_lead_frames(void);

/* DemoPlay_Start / DemoPlay_Create(gGameState.unk1E) + DemoInput_Create(gPad, key file):
 * reads the table entry and the key file, sets g_dcp_world.zone/act/mode (3)/flags (4, 8
 * when there is a start point), check_x/check_y (start point), rand_seed (gRandSeed).
 * Returns 0, or -1 when the data is missing (the caller ends the demo at once). */
int demo_start(int index, DemoInfo *out);
/* After the stage and the player exist (StageStart_State_* is done): gRandSeed is set to
 * the key file's seed (DemoInput_Create ran before the stage was built; nothing between
 * them may draw random numbers in the port) and the lead frames are consumed. */
void demo_after_stage_load(void);
/* DemoInput_Main: this frame's gPad (recorded keys while playing, the real pad after the
 * end record, as on the DS). real = this frame's controller. Call once per frame, first. */
void demo_input_frame(const InputState *real, DemoPad *out);
/* DemoPlay_Main: fade-in once the stage timer runs, skip on A/B/START/X/Y (real pad),
 * the frame limit, the blinking "DEMO PLAY" label (ui_demo_label). Call once per frame,
 * after the world and ui_post_update. */
int demo_play_frame(const InputState *real);
/* DemoPlay_Destructor + DemoInput_Destructor: clears gGameState.unk18 bits 4 and 8, the
 * label, frees the key file. Call when the demo stage is torn down. */
void demo_stop(void);
int demo_active(void);
/* gDemoInputMode (0 off, 1 ended, 2 playing, 5 stopping) and the DemoPlay frame counter */
int demo_input_mode(void);
uint32_t demo_timer(void);
/* DS key bits -> InputState bits (A->IN_JUMP, B->IN_TRICK, X->IN_BOOST, Y->IN_ALT, START, D-pad) */
uint32_t demo_keys_to_input(uint16_t keys);
uint16_t demo_input_to_keys(uint32_t held);
#endif

/*
 * Extra Zone boss (stage 26, zone 8 act 2; overlay ov001 ExtraBoss / ExtraBossObj / ExtraBossPlayer)
 * - native, self-contained mode for the Dreamcast port.  See docs/dreamcast/translation/BOSSES.md
 * section "Extra Zone boss".
 *
 * The fight has no level.bin: on the DS ExtraBoss_Start loads /narc/extra_act_lz7.narc, /mod/bsex_md.nsbmd
 * and the Super Sonic / Burning Blaze models itself; natively boss_extra_init loads their converted
 * packages (model_dir: .dcm / .dca / .dcv, data_dir: tools/bossexdata.py output).  The module runs its own
 * player control (8-way flight, dash, Blaze's fire shot), camera, lights, background and HUD, and draws
 * the whole fight into one 640x480 view.  Five rounds: 0 Sonic (scrolling meteor
 * field, then the weapon boss), 1 Blaze (the field, then the hand boss), 2 Sonic (weapon boss), 3 Blaze (hand boss),
 * 4 Sonic (weapon boss, final).  Losing a life sends the pair back to the Sonic round of the pair.
 *
 *   boss_extra_set_paths("/cd/models", "/cd/boss/extra");
 *   boss_extra_set_lives(sonic_lives, blaze_lives);       // StoryProgress_RestoreFromSave per character
 *   if (boss_extra_init(character) == 0)
 *       for (;;) {
 *           input_poll(&in);
 *           int r = boss_extra_run_frame(&in);            // BOSS_EXTRA_RUNNING / _CLEARED / _FAILED
 *           render_begin_frame();
 *           render_begin_list(PVR_LIST_OP_POLY); boss_extra_draw(PVR_LIST_OP_POLY); render_end_list();
 *           render_begin_list(PVR_LIST_TR_POLY); boss_extra_draw(PVR_LIST_TR_POLY); render_end_list();
 *           render_begin_list(PVR_LIST_PT_POLY); boss_extra_draw(PVR_LIST_PT_POLY); render_end_list();
 *           render_end_frame();
 *           if (r != BOSS_EXTRA_RUNNING) break;
 *       }
 *   boss_extra_result(&res);                               // time / rings for ExtraAct, lives to save
 *   boss_extra_shutdown();
 *
 * Controller (the fight reads only buttons; there is no touch-screen or stylus input):
 *   D-pad / analog stick -> DS D-pad (8-way flight; input_poll folds the stick into held)
 *   A -> DS A, B -> DS B, X -> DS B, Y -> DS A     Super Sonic: A or B = dash.
 *                                                  Burning Blaze: B = dash, A = fire shot.
 *   START -> DS START (pause request, see boss_extra_paused)
 */
#ifndef BOSS_EXTRA_H
#define BOSS_EXTRA_H

#include <stdint.h>
#include "../engine/input.h"

enum { BOSS_EXTRA_RUNNING = 0, BOSS_EXTRA_CLEARED = 1, BOSS_EXTRA_FAILED = 2 };

/* boss_phase() values while the Extra boss runs (gBossStatus.unk4 is not written by the original
 * Extra boss; the port sets these so the UI can show the fight state) */
enum {
    BOSSEX_PHASE_TITLE = 0x40,     /* round intro (StageTitleCard_CreateExtraBoss) */
    BOSSEX_PHASE_SCROLL = 0x41,    /* round 0: meteor field before the boss (ExtraBoss_State_ScrollSpawn) */
    BOSSEX_PHASE_WEAPON = 0x42,    /* weapon boss (ExtraBoss_CreateWeaponBoss) alive */
    BOSSEX_PHASE_HAND = 0x43,      /* hand boss (ExtraBoss_CreateHandBoss) alive */
    BOSSEX_PHASE_DEFEAT = 0x44,    /* boss defeat sequence (work->unk318 camera track) */
    BOSSEX_PHASE_SWITCH = 0x45,    /* character switch between rounds (ExtraBoss_State_CharacterSwitch) */
    BOSSEX_PHASE_PLAYER_DOWN = 0x46, /* rings ran out / time over: fade out, lose a life */
    BOSSEX_PHASE_CLEAR = 0x47      /* final boss defeated (work flags 0x100) */
};

typedef struct BossExtraResult {
    int cleared;           /* 1: DemoScript_StartAfterBoss (ExtraAct follows), 0: game over / quit */
    int32_t time;          /* gExtraBossResultTime (frames, clamped to 0x8c9f) */
    int32_t rings;         /* gExtraBossResultRings (0..999) */
    int lives_sonic;       /* SaveData_SetLivesBoth(work->unk60, work->unk61) */
    int lives_blaze;
    int lives_current;     /* data_022c4590.unk14 after ExtraBoss_Shutdown: Blaze's lives clamped 2..99 */
    int round;             /* work->unk40 reached (0..4) */
} BossExtraResult;

typedef struct BossExtraHooks {
    void *ctx;
    /* round intro: StageTitleCard_CreateExtraBoss (UI module).  If set, the integrator shows the
     * title card and calls boss_extra_start_battle() when it reaches StageTitleCard_State_StartStage.
     * If NULL, the module waits BOSS_EXTRA_TITLE_FRAMES frames itself. */
    void (*title_card)(void *ctx, int character);
    /* PauseMenu_Create(0): START pressed (the module stops updating until boss_extra_resume) */
    void (*pause)(void *ctx);
} BossExtraHooks;
extern BossExtraHooks g_boss_extra_hooks;
#define BOSS_EXTRA_TITLE_FRAMES 150

void boss_extra_set_paths(const char *model_dir, const char *data_dir);
void boss_extra_set_lives(int sonic, int blaze);   /* defaults 3 / 3 */
void boss_extra_set_difficulty(int level);         /* gGameState.unk14 (0 normal, 1 hard) */
/* ExtraBoss_Start.  character = gGameState.unk0 at entry (the fight sets the player slot itself and
 * always starts with Super Sonic; the value is restored by boss_extra_shutdown).  Returns 0, <0 if a
 * file is missing. */
int boss_extra_init(int character);
int boss_extra_run_frame(const InputState *in);
/* KOS: submits the polygons of `list` (PVR_LIST_OP_POLY / PT / TR) with the list already open.
 * Host: records the frame for the test renderer (list 0 = everything). */
void boss_extra_draw(int list);
void boss_extra_shutdown(void);
void boss_extra_result(BossExtraResult *r);
void boss_extra_start_battle(void);  /* StageTitleCard_State_StartStage -> ExtraBoss_StartBattle */
int boss_extra_paused(void);
void boss_extra_resume(void);
int boss_extra_character(void);      /* gExtraBossWork->unk3C: 0 Super Sonic, 1 Burning Blaze */
int boss_extra_round(void);          /* gExtraBossWork->unk40 */
int boss_extra_rings(void);          /* gExtraBossWork->unk4C */
int32_t boss_extra_time(void);       /* gExtraBossWork->unk44 */

#endif

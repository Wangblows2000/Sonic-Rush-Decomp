/*
 * ExtraAct (Dreamcast port): the Extra Zone results scene ("SONIC & BLAZE" /
 * TIME BONUS / RING BONUS / TOTAL / rank / "EXTRA ZONE CLEAR" over the 3D
 * finish model), translated from src/game/Zone/Extra_Zone/Results (ExtraAct_*,
 * 37 functions). It runs after the Extra boss: cutscene CS_SCRIPT 2 ends with
 * CS_NEXT_EXTRA_ACT (SceneChange_ToExtraAct), and ExtraAct ends with
 * SceneChange_ToExtraEnding (cutscene CS_EXTRA_ENDING). See MENUS.md section 12.
 *
 * It is a front-end scene on the menus engine (tasks, AnimatorSprite, one
 * 640x480 composition); the menus must not run meanwhile.
 *
 *     extraact_start(res.time, res.rings, character);   // boss_extra_result()
 *     for (;;) {
 *         input_poll(&in);
 *         int done = extraact_run_frame(&in);
 *         render_begin_frame(); extraact_draw(); render_end_frame();
 *         if (done) break;
 *     }
 *     extraact_shutdown();
 *     run_cutscene(CS_EXTRA_ENDING, 0, 8, NULL);   // SceneChange_ToExtraEnding
 */
#ifndef SRDC_EXTRAACT_H
#define SRDC_EXTRAACT_H
#include <stdint.h>
#include "../engine/input.h"

/* Where the finish model lives (default "/cd/models"):
 *   narc__extra_act__eff_dmg_end01__eff_dmg_end01.dcm   (the background model)
 *   narc__extra_act__eff_dmg_end02__eff_dmg_end02.dcm   (the animated model)
 *   narc__extra_act__eff_dmg_end02.dca                  (its joint animation)
 * The tables come from <menus dir>/extraact.bin, the sprites from
 * <sprites>/act__ac_ex_cldm (menus_set_paths). */
void extraact_set_model_dir(const char *dir);
/* ExtraAct_Start with gExtraBossResultTime (frames) and gExtraBossResultRings.
 * Returns 0, or -1 when extraact.bin is missing (call extraact_shutdown and go on). */
int extraact_start(uint32_t time, uint32_t rings, int character);
/* One frame (Pad_Update, the task list). Returns 1 once the scene is over
 * (ExtraAct_ReleaseAssets + SceneChange_ToExtraEnding). */
int extraact_run_frame(const InputState *in);
/* KOS: opens and closes the OP, PT and TR lists itself (3D model, then the 2D
 * composition and the brightness overlay). Host: no-op. */
void extraact_draw(void);
void extraact_shutdown(void);
/* gExtraAct results: [0] time bonus, [1] ring bonus (rings x 100), [2] total, [3] rank (0 S .. 3 C) */
void extraact_result(int32_t out[4]);
/* host tests: transformed vertices of the drawn models (x, y, 1/w per vertex) */
int extraact_project(float *xyz, int max);
#endif

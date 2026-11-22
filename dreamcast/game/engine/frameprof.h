/*
 * frameprof.h - frame pacing and memory probe for test builds.
 *
 * Enabled by /cd/debug/frameprof.txt (its number = seconds between summaries outside gameplay,
 * default 10).  Frame times go to a RAM histogram; nothing is printed while a gameplay scene runs.
 * One summary per scene (at the scene change), or every N seconds in a non-gameplay scene
 * (front end, cutscenes): frames, sustained fps, frame-interval and work-time distribution, the
 * worst frames with their sections, then heap / VRAM / palette rows / sound RAM.
 *
 * A frame is the span between two frameprof_frame() calls (the start of a frame's drawing, after
 * the vblank / TA wait).  Work = from frameprof_frame() to frameprof_work_end() (the frame's CPU
 * time before it waits for the next one), less the FP_WAIT time added inside it (blocking waits such as
 * a palette write held until the render in progress ends).  Vblanks per frame (pvr_get_vbl_count) are the
 * pacing that reaches the screen; the timer intervals also show a 2.5 ms step once per second (the KOS
 * timer in Flycast), which is not a late frame.
 */
#ifndef FRAMEPROF_H
#define FRAMEPROF_H

#include <stdint.h>

/* sections: update (objects, player), the whole draw, blocking waits, then parts of the draw (the
 * main.c draw ms/frame groups: zone BG, BG layers, player model, objects, UI, boss; FP_D_OMODEL is the
 * object models: objects_models_prepare plus objects_draw_models) */
enum { FP_OBJECTS, FP_PLAYER, FP_DRAW, FP_WAIT, FP_D_BG, FP_D_LAYERS, FP_D_PMODEL, FP_D_OBJ, FP_D_UI, FP_D_BOSS,
       FP_D_OMODEL, FP_SECTIONS };

void frameprof_init(void);              /* reads /cd/debug/frameprof.txt */
int frameprof_enabled(void);
/* a new scene: prints the summary of the previous one; gameplay = summary only at the change */
void frameprof_scene(const char *name, int gameplay);
const char *frameprof_scene_name(int *gameplay);   /* the current scene (to come back to it) */
void frameprof_work_end(void);          /* before the wait for the next frame */
void frameprof_frame(void);             /* a frame starts (after the wait) */
void frameprof_add(int section, uint32_t us);   /* time of a section in the current frame */
void frameprof_flush(void);             /* summary now (e.g. before exit) */

#endif

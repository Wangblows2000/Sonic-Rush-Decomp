/*
 * special.h - native Dreamcast port of the Sonic Rush special stages
 * (src/game/SpecialStage/, overlay 1).  See docs/dreamcast/translation/SPECIAL.md.
 *
 *   special_set_path("/cd/special");            // disc tree written by tools/spdata.py
 *   special_hooks (optional: audio is wired by default on Dreamcast)
 *   if (special_init(stage, 0) == 0) {
 *       for (;;) {
 *           input_poll(&in);
 *           int r = special_run_frame(&in);      // 0 running, 1 cleared, 2 failed
 *           render_begin_frame(); special_draw(); render_end_frame();
 *           if (r) break;
 *       }
 *       special_result(&res);                    // emerald, rings, score for the game flow
 *       special_shutdown();
 *   }
 */
#ifndef SPECIAL_H
#define SPECIAL_H

#include <stdint.h>
#include "../engine/input.h"

#define SPECIAL_NUM_STAGES 7

typedef struct {
    int stage;              /* 0..6 (gSpecialStageWork->unk4 = gGameState.unk1C at entry) */
    int cleared;            /* 1 = emerald won (gSpecialStageWork->unk24 = 1, SpecialStage_RequestExit(1)) */
    uint32_t emerald_flags; /* gGameState.unk28 after SpecialStage_Shutdown (bit stage set when cleared) */
    int rings;              /* player rings (SpecialStagePlayer @0x148) at the end */
    int quota_1;            /* first-half quota (SST header +0x10, SpecialStage_GetRingQuota) */
    int quota_2;            /* goal quota (SST header +0x12) */
    int score;              /* the original passes no score back: always 0 (see SPECIAL.md) */
    int frames;             /* gSpecialStageWork->unk28: play frames */
} SpecialResult;

/* audio hooks (arc/id are the sys SDAT sequence-archive numbers of the original calls) */
typedef struct {
    void (*sfx)(int arc, int id);            /* Sound_PlaySeqArc(0, arc, id) */
    void (*music)(int seq);                  /* Sound_PlaySeq(0, seq): 0x1e SEQ_SS_GAME, 0x1f SEQ_SS_END */
    void (*music_stop)(int fade_frames);     /* NNS_SndPlayerStopSeq(&gBGMSoundHandle, fade) */
    void (*sfx_stop_arc)(int arc, int id, int fade); /* NNS_SndPlayerStopSeqBySeqArcIdx */
} SpecialHooks;
extern SpecialHooks special_hooks;

void special_set_path(const char *dir);      /* default "/cd/special" */
void special_set_language(int lang);         /* gGameLanguage 0..5 (default 1 English) */
/* entry: SpecialStage_Start (SceneChange_ToSpecialStage).  stage_index 0..6 is the
 * emerald / SST file (the original uses gGameState.unk1C, the zone index of the act the
 * special ring was in); character must be 0 (Sonic): Blaze never gets special stages
 * (GmkSpecial_Create refuses gGameState.unk0 == 1).  emerald_flags = gGameState.unk28.
 * Returns 0 on success, <0 if assets are missing. */
int special_init(int stage_index, int character);
void special_set_emerald_flags(uint32_t flags);   /* gGameState.unk28 before special_init */
int special_run_frame(const InputState *in);  /* 0 running, 1 cleared (emerald), 2 failed */
/* Emits the frame: OP, PT and TR lists, each opened and closed here (render_begin_list /
 * render_end_list), so call it between render_begin_frame and render_end_frame with no
 * list open.  Host builds: no-op apart from the test quad capture. */
void special_draw(void);
void special_shutdown(void);
void special_result(SpecialResult *r);
int special_paused(void);                     /* START pause (PauseMenu_Create(1)) active */
/* the controller legend card at the intro prompt: on (default) until the player begins a
 * special stage once in this session; special_show_legend(1) shows it again next time */
void special_show_legend(int on);
/* test builds only: keep the player's rings at the goal quota (reaches the results card unattended) */
void special_debug_force_quota(int on);

#endif

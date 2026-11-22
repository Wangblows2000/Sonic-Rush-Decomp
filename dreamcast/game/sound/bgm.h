/*
 * Music selection and the in-stage music events of Sonic Rush, translated from
 * src/game/Stage (Stage_PlayBGM, Stage_SelectBGMLayer, Stage_FadeBGMLayer) and
 * src/game/Sound (Sound_PlaySeq, Sound_PlayBGMOnPlayer3, StageJingle_*),
 * Player_AddLives / Player_GiveInvincibility (audio part) and the music half of
 * Player_UpdateGravityFlip. The original plays every track as a sequence of the
 * loaded sound archive; the port streams the renders through engine/audio.c,
 * looked up in /cd/music/streams.txt (tools/dreamcast/audio/musictab.py):
 *   (sdat, seq) -> file, loop, layer, player, RAM jingle.
 *
 * Handles of the original and what plays them here:
 *   gStageBGMHandleB / A (Stage_PlayBGM, acts)   disc stream, layer of a .lyr (or one WAV)
 *   gBGMSoundHandle (Sound_PlaySeq, boss music,  disc stream, or RAM jingle slot 1
 *     pinch, sys music, Sound_PlayBGMOnPlayer3)  (SEQ_1up and other RAM jingles)
 *   StageJingle (SEQ_muteki 0x16, SEQ_brethless  RAM jingle slot 0
 *     0x17)
 * The stage music keeps running muted under a StageJingle (Stage_FadeBGMLayer(0, 0))
 * and fades back in at its current position (Stage_FadeBGMLayer(vol, 0x5A / 0x3C)),
 * as on the DS. Sound_PlayBGMOnPlayer3 replaces whatever plays on gBGMSoundHandle
 * (in a boss act: the boss music; zone SDATs then hold no SEQ_1up: silence).
 *
 * Integration: see docs/dreamcast/AUDIO.md section 14 (main.c patch).
 */
#ifndef SOUND_BGM_H
#define SOUND_BGM_H
#include <stdint.h>

struct DcPlayer;

void bgm_set_dir(const char *dir);              /* default "/cd/music" */
int  bgm_init(void);                            /* loads streams.txt (lazy; 0 = ok) */

/* Stage_SetupZoneResources -> Stage_PlayBGM(0): starts the act / boss music of
 * (zone, act, chara) (gGameState.unk1C / unk1D / unk0), sets the music archive
 * (the stage SDAT), preloads the stage jingles (call inside the load bracket),
 * binds the player (StageJingle / gravity flip; may be NULL) and destroys a
 * StageJingle of the previous stage. Zone 8 act 2 = the Extra boss ("ze"). */
void bgm_stage_start(int zone, int act, int chara, struct DcPlayer *p);
void bgm_bind_player(struct DcPlayer *p);
/* the restart of the current stage after a death (StageRestart: tasks destroyed,
 * zone resources reloaded, Stage_PlayBGM(0)) */
void bgm_stage_restart(void);
/* Stage_DestroyTasks without a new stage (special stage, exit): the StageJingle task ends */
void bgm_stage_end(void);
/* music archive for Sound_PlaySeq (gBGMSoundHandle) without starting music ("ze", "sys") */
void bgm_set_context(const char *sdat);
const char *bgm_context(void);

/* ---- original functions ---- */
void Stage_PlayBGM(int layer);
void Stage_SelectBGMLayer(int layer);                       /* 1 = alternate mix (underwater / gravity flip) */
void Stage_FadeBGMLayer(int vol, int frames, int layer_flag);
void StageJingle_Create(struct DcPlayer *p, int seq);       /* 0x16 SEQ_muteki, 0x17 SEQ_brethless */
void Sound_PlayBGMOnPlayer3(int seq);                        /* 0x18 SEQ_1up */
void Player_AddLives_Sound(int n);                           /* the Sound_PlayBGMOnPlayer3 of Player_AddLives (mode check) */
void Player_GiveInvincibility(struct DcPlayer *p);           /* SE 0x3d, StageJingle 0x16, timers (no EffInvincibility) */
/* Sound_PlaySeq(0, seq) in the music archive `sdat` (NULL: the current context) */
int  bgm_play(const char *sdat, int seq);
void bgm_stop(int fade_frames);                              /* NNS_SndPlayerStopSeq(&gBGMSoundHandle, fade) */
void bgm_move_volume(int vol, int frames);                   /* NNS_SndPlayerMoveVolume(&gBGMSoundHandle, vol, frames) */
void bgm_stop_sound_all(void);                               /* NNS_SndStopSoundAll: music, jingles, effects */
void bgm_pause_players(int on);                              /* PauseMenu: NNS_SndPlayerPauseByPlayerNo(3..9, on) + System_Freeze */

/* ---- hooks (signatures of g_ui_hooks / g_boss_hooks / special_hooks / g_obj_hooks.effect) ---- */
void bgm_ui_music_hook(void *ctx, int32_t id);               /* ClearDemo / GameOver: sys music (+ the ClearDemo SE bank), -1 stop all */
void bgm_ui_music_pause_hook(void *ctx, int on);
void bgm_ui_sfx_hook(void *ctx, int32_t arc, int32_t id);   /* Sound_PlaySeqArcEx(0, arc, id) */
void bgm_boss_music_hook(void *ctx, int32_t seq);            /* Sound_PlaySeq(0, seq) in the boss SDAT */
void bgm_boss_music_stop_hook(void *ctx);                    /* NNS_SndPlayerStopSeqAll(0) */
void bgm_boss_music_fade_hook(void *ctx, int32_t frames);    /* g_boss_hooks.music_fade: StopSeq(&gBGMSoundHandle, frames) */
void bgm_boss_music_volume_hook(void *ctx, int32_t vol, int32_t frames); /* g_boss_hooks.music_volume: MoveVolume */
void bgm_special_music(int seq);                             /* special_hooks.music: sys SEQ_SS_GAME / SEQ_SS_END */
void bgm_special_music_stop(int fade);                       /* special_hooks.music_stop */
/* g_obj_hooks.effect kinds OBJ_FX_BGM_LAYER / OBJ_FX_JINGLE; returns 1 if handled */
int  bgm_obj_effect(struct DcPlayer *p, int32_t kind, int32_t a);

/* once per game frame (sound_update calls it): StageJingle_Main, gravity-flip layer,
 * test triggers (/cd/debug/bgmtest.txt) */
void bgm_update(void);

/* state for tests / traces */
typedef struct {
    int stage_active, layered, layer_sel;    /* stage BGM on the disc stream, .lyr, Stage_SelectBGMLayer */
    int seq_b, seq_a;                         /* sequences on gStageBGMHandleB / A */
    int jingle_state;                         /* StageJingle w->state (0 none) */
    int bgm_seq;                              /* last sequence on gBGMSoundHandle (-1 none) */
    int frozen;
    char stage_sdat[8], ctx_sdat[8];
    uint32_t plays, misses;                   /* bgm_play calls, sequences without a stream */
} BgmState;
const BgmState *bgm_state(void);

#endif

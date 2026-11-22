/*
 * Native boss module (Dreamcast port of Sonic Rush) - public API.
 *
 * Translation of src/game/Boss/ (Boss_*, BossArena_*, BossCamera_*, BossEffect_*),
 * src/game/Zone/Leaf_Storm/Boss/ (BossStage1_*) and src/game/Boss/Boss1/
 * (Boss1, Boss1Neck, Boss1Hammer, EffPebble = Egg Hammer, Leaf Storm act 3).
 * See docs/dreamcast/translation/BOSSES.md.
 *
 * Frame order (DS task ids in brackets):
 *
 *     objects_frame_begin();          // ObjRect_CheckAllGroup: hammer boxes vs player boxes
 *     dcp_player_frame(&player, ...); // player (0x100)
 *     objects_update(&player);        // map objects, rings
 *     boss_update(&player);           // Boss1 (0x500), Boss1Neck (0x501), Boss1Hammer (0x502),
 *                                     // boss effects (0xa00), ScreenShake (0x3e00),
 *                                     // BossCamera (0x3f00)
 *     boss_camera(&cam);              // 3D camera for this frame (player model, arena)
 *     boss_draw(PVR_LIST_OP_POLY); boss_draw(PVR_LIST_PT_POLY); boss_draw(PVR_LIST_TR_POLY);
 *
 * Positions are 24.8 fixed point (DcPlayer.x).  The 3D world is DS pixels:
 * world = (x/256, -y/256, z/256), like Boss1Neck_Draw (unk48 = x << 4 in fx32).
 */
#ifndef BOSS_H
#define BOSS_H

#include <stdint.h>
#include "../player/dcp_player.h"
#include "../model/model.h"

/* gBossStatus.unk4 (boss_phase) values written by the Boss1 code */
enum {
    BOSS_PHASE_NONE = 0,
    BOSS_PHASE_IDLE = 1,     /* Boss1_Action_Idle */
    BOSS_PHASE_SLAM = 2,     /* Boss1_Action_WaitSlam / WaitTripleSlam */
    BOSS_PHASE_BIGSLAM = 3,  /* Boss1_Action_WaitBigSlam */
    BOSS_PHASE_ROLL = 4,     /* Boss1_Action_WaitHammerRoll */
    BOSS_PHASE_HURT = 5,     /* Boss1_Action_Hurt */
    BOSS_PHASE_DEFEAT = 6,   /* Boss1_Action_Defeat */
    BOSS_PHASE_MISSILE = 0x1b /* Boss1_Action_WaitMissileAttack (zone 5 variant) */
};

/* Side effects outside the boss module (all optional). Each sits where the
 * original call was. */
typedef struct BossHooks {
    void *ctx;
    void (*sfx)(void *ctx, s32 id);                   /* Sound_PlaySeqArc(0, 0, id) */
    void (*voice)(void *ctx, s32 id);                 /* Sound_PlayVoice(0, 0, id) */
    void (*music)(void *ctx, s32 seq);                /* Sound_PlaySeq(0, seq) (0xf: pinch) */
    void (*music_stop)(void *ctx);                    /* NNS_SndPlayerStopSeqAll(0) */
    void (*navi)(void *ctx, s32 face, s32 msg, s32 time); /* NaviMsg_SetFaceAnim + NaviMsg_ShowMessage */
    void (*cleared)(void *ctx);                       /* StageShutdown_Create(Stage_OnBossCleared) */
    void (*ring_flags)(void *ctx, u32 set);           /* RingManager_SetFlags(GetFlags() | set) */
    void (*missile)(void *ctx, s32 x, s32 y, s32 z, s32 tx, s32 ty, s32 tz, s32 speed); /* Boss5PartD3 (not translated) */
    void (*log)(void *ctx, const char *event, s32 a, s32 b); /* native: test/debug trace */
} BossHooks;
extern BossHooks g_boss_hooks;

/* Where the converted packages live (defaults "/cd/models" and "/cd/boss"):
 * model_dir holds build/dreamcast/models/<file>, data_dir bossdata.bin. */
void boss_set_paths(const char *model_dir, const char *data_dir);
/* gGameState.unk14 (0 or 1; 1 -> 8 hit points) and gGameState.unk0 (player
 * slot used by the palette tables).  Call before boss_init. */
void boss_set_difficulty(int level_idx);
/* Stage set-up: stage_id = rlvl_header_t.stage (zone*3 + act; 2 = z13).
 * level = the whole level.bin (the boss map object, type 0x3f, is taken from
 * it), player = gPlayers[0] (must already be created).  Loads bossdata.bin and
 * the stage's models (BossStage1_OnModelLoaded), creates the boss camera
 * (BossCamera_Create(2)) and the boss (Boss1_Create).  textures: upload model
 * textures to the PVR (1 on Dreamcast, 0 for host tests).
 * Returns 0 on success, <0 if this stage has no translated boss or an asset
 * is missing. */
int boss_init(int stage_id, const uint8_t *level, DcPlayer *player, int textures);
void boss_shutdown(void);
/* gBossStatus.unk0 |= 1: StageTitleCard_State_StartStage starts the fight.
 * Until then Boss1_State_WaitForStart waits (parts hidden). */
void boss_start(void);
void boss_update(DcPlayer *player);
/* Draw every boss model + effect whose material class matches `list`
 * (PVR_LIST_OP_POLY / PT / TR) with the boss camera. KOS build only
 * (on the host it only counts triangles via dcm_transform_vertices). */
int boss_draw(int list);

s32 boss_health(void);       /* gBossStatus.unk8 */
s32 boss_health_max(void);   /* gBossStatus.unkA */
s32 boss_phase(void);        /* gBossStatus.unk4 */
int boss_defeated(void);     /* Boss1_Action_Defeat ran (phase 6) */
int boss_cleared(void);      /* Boss1_State_DefeatFinish requested the stage exit */

/* Camera / screen state for the integrator */
void boss_camera(dcm_camera_t *out);   /* BossCamera (eye/at + shake, perspective) */
void boss_camera_eye(float eye[3], float at[3]);
void boss_shake_offset(s32 *x, s32 *y); /* ScreenShake_GetOffsetX/Y (px) */
s32 boss_brightness(void);              /* gDisplayMain.unk48: 16 = white .. 0 (master brightness) */
s32 boss_exc_background(void);          /* Boss_DrawExcBackground index shown (6/7) or -1 */

/* statistics / trace for tests */
typedef struct BossStats {
    u32 hits, attacks, slams, triple_slams, big_slams, rolls, missiles;
    u32 player_hurts_before;  /* filled by the test, not the module */
    u32 effects_spawned, pebbles_spawned;
} BossStats;
const BossStats *boss_stats(void);
/* name of the current state function of part 0 (Boss1), 1 (neck), 2 (hammer) */
const char *boss_state_name(int part);
/* hammer centre (24.8) and whether its damage box is armed (tests) */
void boss_hammer_pos(s32 *x, s32 *y, s32 *z);
int boss_hammer_box_armed(int box);

#endif

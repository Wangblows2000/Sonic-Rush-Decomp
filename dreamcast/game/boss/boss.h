/*
 * Native boss module (Dreamcast port of Sonic Rush) - public API.
 *
 * Translation of src/game/Boss/ (Boss_*, BossArena_*, BossCamera_*, BossEffect_*),
 * src/game/Zone/Leaf_Storm/Boss/ (BossStage1_*) and src/game/Boss/Boss1/
 * (Boss1, Boss1Neck, Boss1Hammer, EffPebble = Egg Hammer, Leaf Storm act 3),
 * src/game/Zone/Water_Palace/Boss/ (Boss2, Boss2Water, Boss2WaterLight,
 * BossStage2 = Egg Turtle, Water Palace act 3) and the Water Palace 3D water
 * effects it spawns (EffRipple3D, EffSplash3D).
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
#include "../objects/objects.h" /* ObjBox */

/* gBossStatus.unk4 (boss_phase) values written by the Boss1 code */
enum {
    BOSS_PHASE_NONE = 0,
    BOSS_PHASE_IDLE = 1,     /* Boss1_Action_Idle */
    BOSS_PHASE_SLAM = 2,     /* Boss1_Action_WaitSlam / WaitTripleSlam */
    BOSS_PHASE_BIGSLAM = 3,  /* Boss1_Action_WaitBigSlam */
    BOSS_PHASE_ROLL = 4,     /* Boss1_Action_WaitHammerRoll */
    BOSS_PHASE_HURT = 5,     /* Boss1_Action_Hurt */
    BOSS_PHASE_DEFEAT = 6,   /* Boss1_Action_Defeat */
    BOSS_PHASE_MISSILE = 0x1b, /* Boss1_Action_WaitMissileAttack (zone 5 variant) */
    /* gBossStatus.unk4 values written by the Boss2 code */
    BOSS2_PHASE_IDLE = 7,      /* Boss2_Action_Idle (Boss2_Create, Boss2_SelectNextAttack 1/2/3) */
    BOSS2_PHASE_DASH = 8,      /* Boss2_Action_DashAtPlayer: dash / hop / jump at the player */
    BOSS2_PHASE_ATTACK2 = 9,   /* Boss2_Action_Attack2: aim and strike with the head */
    BOSS2_PHASE_ATTACK3 = 10,  /* Boss2_Action_Attack3: water drained, suck and catch */
    BOSS2_PHASE_HURT = 11,     /* Boss2_OnHit_DamageAttack2 / DamageAttack3 */
    BOSS2_PHASE_DEFEAT = 12,   /* Boss2_Action_Defeat */
    /* gBossStatus.unk4 values written by the Boss3 code */
    BOSS3_PHASE_IDLE = 0xd,    /* Boss3_Action_Idle */
    BOSS3_PHASE_PUSH = 0xe,    /* Boss3_Action_WaitPushBall: the body pushes the ball at the player */
    BOSS3_PHASE_SUMMON = 0xf,  /* Boss3_Action_WaitSummonBall: a new ball */
    BOSS3_PHASE_KICK = 0x10,   /* Boss3_Action_WaitKickBall: kicked ball rolls round the arena */
    BOSS3_PHASE_THROW = 0x11,  /* Boss3_Action_WaitThrowAttack: jump to the centre, throw balls */
    BOSS3_PHASE_HURT = 0x12,   /* Boss3_Action_Hurt */
    BOSS3_PHASE_DEFEAT = 0x13, /* Boss3_Action_Defeat */
    /* gBossStatus.unk4 values written by the Boss4 code */
    BOSS4_PHASE_SPIN = 0x14,     /* Boss4_Action_StartSpin: the seesaw spins round the arena */
    BOSS4_PHASE_CLASH = 0x15,    /* Boss4_Action_Clash: the arm was hit (an enemy drops) */
    BOSS4_PHASE_HURT = 0x16,     /* Boss4_Action_Hurt */
    BOSS4_PHASE_SLAM = 0x18,     /* Boss4_Action_StartSlam: the arm slams, shock waves */
    BOSS4_PHASE_TILTSPIN = 0x19, /* Boss4_Action_StartTiltSpin */
    BOSS4_PHASE_DEFEAT = 0x1a    /* Boss4_Action_Defeat */
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
    /* Boss2 plays looping sound effects on its own handles (Sound_AllocHandle, 1..3)
     * and stops them with NNS_SndPlayerStopSeq.  If sfx_handle is NULL, sfx is used. */
    void (*sfx_handle)(void *ctx, s32 handle, s32 id); /* Sound_PlaySeqArc(handle, 0, id) */
    void (*sfx_stop)(void *ctx, s32 handle);           /* NNS_SndPlayerStopSeq(handle, 0) */
    void (*water_fx)(void *ctx, s32 x, s32 y, s32 z);  /* EffWater3D_Create (3D BAC sprite, not translated) */
    /* Boss4_Action_Defeat disarms the player's hit boxes: box 0 = body (gPlayers->coll2A0, flags @0x2AA),
     * 1 = attack (coll2CC, @0x2D6): flags = (flags & ~clear) | set.  The boxes belong to the objects
     * module (ObjPlayerCtx body / atk). */
    void (*player_box_flags)(void *ctx, DcPlayer *p, s32 box, u16 clear, u16 set);
    /* BossHelp (src/game/UI/Hud/BossHelp_*: the help prompt sprite, drawn by the UI).
     * kind BOSS_HELP_CREATE: BossHelp_Create(a = animation index; Boss6 tilt: 0);
     * kind BOSS_HELP_DESTROY: BossHelp_Destroy (only sent while one is shown; a = the index shown).
     * b is unused (0).  The current state is also boss_help(). */
    void (*help)(void *ctx, s32 kind, s32 a, s32 b);
    /* BossF: TalkDemo_Create (gTalkDemoId = id: 0xf Sonic / 0x10 Blaze story) in the middle of the fight;
     * the DS freezes the game (System_Freeze) until the talk ends: skip boss_update_pre / boss_update
     * (and the player) while it runs (cutscene_start(CS_TALK, id, ..) / cutscene_active()). */
    void (*talk)(void *ctx, s32 id);
    /* gBGMSoundHandle fades (appended): NNS_SndPlayerStopSeq(&gBGMSoundHandle, frames)
     * (Boss7_State_ClashFadeOutBGM, BossFPartD8_State_SkyChargeStartFadeOut: 0x3c) and
     * NNS_SndPlayerMoveVolume(&gBGMSoundHandle, vol, frames) (Boss7PartD5_MoveState_WaitDefeatScene:
     * 0x20 over 0xb4). If music_fade is NULL, music_stop is called. */
    void (*music_fade)(void *ctx, s32 frames);
    void (*music_volume)(void *ctx, s32 vol, s32 frames);
    /* StageObj_Load3dBAC one-shot effects (appended): play animation `anim` of the converted 3D BAC
     * `bac_id` once at (x, y, z) (24.8, boss-arena world coordinates when flags & 1, as
     * objects_fx3d_spawn's OBJ_FX3D_WORLD; flags & 2: facing right). main.c: objects_fx3d_spawn.
     * Boss7: EffBoostA_3D / EffBoostA2_3D / EffFire01_3D / EffKemuriSpin3D. */
    int (*fx3d)(void *ctx, const char *bac_id, s32 anim, s32 x, s32 y, s32 z, u32 flags, s32 scale);
    /* NNS_SndPlayerSetVolume(gSESoundHandle, vol) (appended): Boss7_State_ClashStruggle 0x30 after its SE */
    void (*sfx_volume)(void *ctx, s32 vol);
} BossHooks;
enum { BOSS_HELP_CREATE = 0, BOSS_HELP_DESTROY = 1 };
extern BossHooks g_boss_hooks;

/* Where the converted packages live (defaults "/cd/models" and "/cd/boss"):
 * model_dir holds build/dreamcast/models/<file>, data_dir bossdata.bin. */
void boss_set_paths(const char *model_dir, const char *data_dir);
/* gGameState.unk14 (0 or 1; 1 -> 8 hit points) and gGameState.unk0 (player
 * slot used by the palette tables).  Call before boss_init. */
void boss_set_difficulty(int level_idx);
/* Stage set-up: stage_id = rlvl_header_t.stage (zone*3 + act; 2 = z13 Boss1,
 * 5 = z23 Boss2, 8 = z33 Boss3 (map object 0x3d), 11 = z43 Boss4 (0x3c)).
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
/* Boss tasks that run before the player (task id < 0x100; BossF's body, 0xf0, whose arm the player
 * stands on and rides).  Call before dcp_player_frame every frame; a no-op for the other bosses.
 * Without it boss_update runs them (one frame later relative to the player). */
void boss_update_pre(DcPlayer *player);
/* Draw every boss model + effect whose material class matches `list`
 * (PVR_LIST_OP_POLY / PT / TR) with the boss camera. KOS build only
 * (on the host it only counts triangles via dcm_transform_vertices). */
int boss_draw(int list);

s32 boss_health(void);       /* gBossStatus.unk8 */
s32 boss_health_max(void);   /* gBossStatus.unkA */
s32 boss_phase(void);        /* gBossStatus.unk4 */
int boss_defeated(void);     /* Boss1_Action_Defeat (phase 6) / Boss2_Action_Defeat (phase 12) ran */
int boss_cleared(void);      /* Boss1_State_DefeatFinish requested the stage exit */

/* Camera / screen state for the integrator */
void boss_camera(dcm_camera_t *out);   /* BossCamera (eye/at + shake, perspective) */
void boss_camera_eye(float eye[3], float at[3]);
void boss_shake_offset(s32 *x, s32 *y); /* ScreenShake_GetOffsetX/Y (px) */
s32 boss_brightness(void);              /* gDisplayMain.unk48: 16 = white .. 0 (master brightness) */
s32 boss_exc_background(void);          /* Boss_DrawExcBackground index shown (6/7) or -1 */
/* Player model transform in boss arenas (Boss2: gPlayers->unk70 = Boss2_Draw_PlayerOnArena,
 * the player walks on a circle).  Returns 1 and fills trans (fx32 px, world) and
 * rot (3x3 4.12, row-major) when the boss overrides the player draw; 0 otherwise
 * (then draw the player at (x/256, -y/256, z/256) as in BOSSES.md). */
int boss_player_model(s32 trans[3], s32 rot[9]);
/* Lights the boss stage uses this frame (Boss2: above / under the water),
 * already in view space for dcm_draw.  Returns 0 if the stage defaults apply. */
int boss_lights(dcm_lights_t *out);
/* gCamera.unk62 water level (px) the boss writes (Boss2Water_Main); -1 if none.
 * The same value is written to g_obj_camera[0].water. */
s32 boss_water_level(void);

/* statistics / trace for tests */
typedef struct BossStats {
    u32 hits, attacks, slams, triple_slams, big_slams, rolls, missiles;
    u32 player_hurts_before;  /* filled by the test, not the module */
    u32 effects_spawned, pebbles_spawned;
    /* Boss2 */
    u32 dashes, jumps, strikes, catches, water_lowers, water_raises, ripples, splashes;
    /* Boss3 (jumps = jumps over the ball, catches = ball caught) */
    u32 pushes, kicks, throw_attacks, summons, balls_spawned, balls_thrown, ball_explosions, reflects, staggers;
    /* Boss4 (slams above) */
    u32 tilt_spins, clashes, shot_bursts, shots, enemies, enemy_bounces, waves, electrified;
    /* Boss5 (missiles above: attacks chosen) */
    u32 missiles_fired, missile_explosions;
} BossStats;
const BossStats *boss_stats(void);
/* name of the current state function of part 0 (Boss1), 1 (neck), 2 (hammer) */
const char *boss_state_name(int part);
/* Boss1: hammer centre (24.8); Boss2: body ("body0" node) position (24.8, arena arc
 * coordinates are not applied: world x/z).  And whether hit box `box` is armed (tests). */
void boss_hammer_pos(s32 *x, s32 *y, s32 *z);
int boss_hammer_box_armed(int box);
/* Boss2: boss arc position (24.8, like the player's x) and its ObjRect box (px) */
int boss2_box(int box, s32 *x, s32 *y, s32 *z, s16 rect[4]);
s32 boss2_arc_x(void);
/* Boss3: body arc position (24.8), and the ball the body holds / kicked (gBoss3->unk218) */
typedef struct Boss3BallInfo {
    s32 x, y;            /* arc position / height (24.8) */
    s32 vel_x;           /* rolling speed (24.8 px per frame, sign = direction) */
    u32 flags;           /* unk20C: 1 rolling, 2 thrown, 4 thorns, 8 harmful/hittable cycle, 0x10 harmful,
                            0x40 reflected by the player, 0x80 being summoned */
    int hittable;        /* box 0x134 armed: a jumping player reflects / staggers */
    int harmful;         /* box 0x160 armed: hurts the player */
    u16 cycle_period, cycle_count; /* unk224 / unk226 */
} Boss3BallInfo;
s32 boss3_arc_x(void);
int boss3_ball(Boss3BallInfo *out);
int boss3_balls_alive(void);
/* Boss4: seesaw state for tests (positions 24.8, arc coordinates) */
typedef struct Boss4Info {
    s32 body_x, body_y, arm_x, arm_y; /* parts 0xce / 0xcf (Boss4_ArenaToStagePos) */
    s32 spin, tilt;                   /* unkBB8 / unkBBC */
    int electrified;                  /* unkBD0 */
    ObjBox body_box;                  /* the body's damage box (last registration) */
    int body_box_valid;               /* registered this frame (not slam / defeat) */
    int enemy;                        /* a Boss4PartD1 enemy is alive */
    s32 enemy_x, enemy_y;
    int shots, waves;                 /* Boss4PartD0 / D2 alive */
} Boss4Info;
int boss4_info(Boss4Info *out);

/* BossHelp_Create index shown (-1: none); see BossHooks.help */
s32 boss_help(void);

/* Boss6 (z63, stage 17, map object 0x3a): gBossStatus.unk4 values */
enum {
    BOSS6_PHASE_MOVE = 0x1c,     /* Boss6_SelectNextAttack 1/2/3/4: walk */
    BOSS6_PHASE_BULLETS = 0x1e,  /* Boss6_Action_StartPartD4Attack */
    BOSS6_PHASE_PICK = 0x1f,     /* Boss6_Action_StartPickAttack */
    BOSS6_PHASE_TILT = 0x20,     /* Boss6_Action_StartTilt: the deck tilts (mash A / B) */
    BOSS6_PHASE_EDGEJUMP = 0x21, /* Boss6_Action_StartEdgeJump */
    BOSS6_PHASE_HURT = 0x22,     /* Boss6_OnHit_Damage / DamageFlip */
    BOSS6_PHASE_DEFEAT = 0x23    /* Boss6_Action_Defeat */
};
/* Boss6 state for tests (positions 24.8 unless noted) */
typedef struct Boss6Info {
    s32 x, y;                    /* boss position */
    int turned;                  /* unkD5E: turned round (camera on the other side) */
    int tilt_left;               /* unk20C & 0x100 */
    s32 prop_speed;              /* unkF68 (0x549f flying level, 0 at a tilt start) */
    s32 core_x, core_y;          /* "body_core" node (px, DS y down) */
    s32 pelvis_x, pelvis_y;      /* "pelvis" node (px) */
    ObjBox box[11];              /* unkD60 hit boxes (last registration) */
    u8 armed[11];
    int bullets_alive;
    ObjBox bullet_box[8][2];
    int pieces_alive;
    s32 palette_body, palette_core; /* Boss_LoadModelPalettes state (file index) */
    /* counters */
    u32 attacks_d4, attacks_pick, attacks_tilt, attacks_jump;
    u32 hits_pick, hits_jump, turns, tilts, jumps, landings;
    u32 bullets, bullet_hits, stuns, mash_presses, edge_knockbacks;
    u32 pieces[4];               /* Boss6_CreatePiece kinds: plate, wind, shoot flash, bomb */
    int help_shown;
} Boss6Info;
int boss6_info(Boss6Info *out);

#endif

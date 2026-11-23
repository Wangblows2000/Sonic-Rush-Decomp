/*
 * Native player module (Dreamcast port of Sonic Rush player physics).
 *
 * Faithful translation of the decompiled player (src/game/Player/) and of the
 * object movement / map collision core (src/game/Stage/ObjectSystem/).
 * Field comments give the original work-struct offset ("@0x34") so every
 * translated line can be compared with the decompilation.
 * See docs/dreamcast/translation/PLAYER.md.
 */
#ifndef DCP_PLAYER_H
#define DCP_PLAYER_H

#include "dcp_types.h"
#include "dcp_collision.h"

/* ---------------------------------------------------------------- input */
/* DS key bits as used after Player_RemapButtons (held @0x420, pressed @0x422) */
#define DCP_KEY_A 0x0001
#define DCP_KEY_B 0x0002
#define DCP_KEY_SELECT 0x0004
#define DCP_KEY_START 0x0008
#define DCP_KEY_RIGHT 0x0010
#define DCP_KEY_LEFT 0x0020
#define DCP_KEY_UP 0x0040
#define DCP_KEY_DOWN 0x0080
#define DCP_KEY_R 0x0100 /* trick / air action */
#define DCP_KEY_L 0x0200
#define DCP_KEY_X 0x0400 /* boost (X or Y) */
#define DCP_KEY_Y 0x0800
#define DCP_KEY_JUMP (DCP_KEY_A | DCP_KEY_B)
#define DCP_KEY_BOOST (DCP_KEY_X | DCP_KEY_Y)

/* ------------------------------------------------- move_flags (@0x58) */
#define DCP_MF_GROUND 0x00000001     /* standing on a surface (set by collision) */
#define DCP_MF_CEILING 0x00000002    /* head hit */
#define DCP_MF_WALL_L 0x00000004     /* wall contact (first horizontal pass) */
#define DCP_MF_WALL_R 0x00000008     /* wall contact (second horizontal pass) */
#define DCP_MF_AIR 0x00000010        /* airborne movement (vel_x/vel_y), ground probes not rotated */
#define DCP_MF_GNDSPEED 0x00000040   /* move along angle with gnd_vel */
#define DCP_MF_GRAVITY 0x00000080    /* apply gravity while not grounded */
#define DCP_MF_NOCOLLIDE 0x00000100  /* skip map collision */
#define DCP_MF_NOOBJ 0x00000200      /* skip solid objects */
#define DCP_MF_NOHORZ 0x00000400     /* skip horizontal collision */
#define DCP_MF_NOVERT 0x00000800     /* skip vertical collision */
#define DCP_MF_NOMAP 0x00001000      /* skip map tiles (objects only) */
#define DCP_MF_NOMOVE 0x00002000     /* skip StageObj_Move */
#define DCP_MF_KEEPVEL 0x00004000    /* collision does not zero velocities */
#define DCP_MF_JUMPING 0x00008000    /* in a jump/launch arc */
#define DCP_MF_SNAP1 0x00010000      /* floor was 1px away last frame */
#define DCP_MF_SLOPEFORCE 0x00020000 /* StageObj_Move applies slope_acc on steep slopes */
#define DCP_MF_SLOPEIDLE 0x00040000  /* slope force also at gnd_vel == 0 */
#define DCP_MF_OOBSOLID 0x00080000   /* probe flag 0x40 */
#define DCP_MF_THRUOK 0x00100000     /* jump-through tiles are solid this frame */
#define DCP_MF_SUBSTEP 0x00200000    /* split fast movement into 8px (4px airborne) collision steps */
#define DCP_MF_WASGROUND 0x00400000  /* grounded at the start of this collision pass */
#define DCP_MF_ANGLESMOOTH 0x00800000/* ease angle toward the floor angle (1 unit/frame) */
#define DCP_MF_RIDEPUSH 0x02000000   /* can be pushed by a touched object */
#define DCP_MF_NOWIND 0x08000000     /* ignore global wind push */
#define DCP_MF_CLEARPUSH 0x10000000  /* clear push_* before moving */

/* --------------------------------------------- player flags (@0x32C) */
#define DCP_PF_FACE_L 0x00000001   /* (bit 0, also "no jump cut" during air states) */
#define DCP_PF_TRICKABLE 0x00000002/* launched: R does tricks */
#define DCP_PF_COMBO_END 0x00000004
#define DCP_PF_TRICKED 0x00000008
#define DCP_PF_HOMING 0x00000040
#define DCP_PF_BOOST 0x00000080
#define DCP_PF_HIGHSPEED 0x00000100
#define DCP_PF_DEAD 0x00000400
#define DCP_PF_TRICK_A 0x00004000
#define DCP_PF_TRICK_F 0x00008000
#define DCP_PF_VSHIT 0x00010000
#define DCP_PF_BOOSTHIT 0x00020000
#define DCP_PF_TENSION_DEBT 0x00040000
#define DCP_PF_NOBOOST 0x00100000
#define DCP_PF_NOINPUT 0x00200000
#define DCP_PF_NOTENSION 0x00400000
#define DCP_PF_GAMEOVER 0x00800000
#define DCP_PF_GOAL 0x01000000
#define DCP_PF_GRAVRESET 0x02000000
#define DCP_PF_UNDERWATER 0x04000000
#define DCP_PF_SLOPEPUSHED 0x08000000
#define DCP_PF_BARRIER 0x10000000
#define DCP_PF_MAGNET 0x20000000

/* -------------------------------------------- disp_flags (@0x5C) */
#define DCP_DF_FLIP 0x0001       /* facing left */
#define DCP_DF_UPSIDE 0x0002
#define DCP_DF_LOOP 0x0004       /* animation loops */
#define DCP_DF_ANIM_DONE 0x0008  /* set by the animation system when a non-looping anim ended */
#define DCP_DF_BLINK 0x0020

/* ------------------------------------------- physics table (ROM data) */
/* One 0x30-byte entry of gPlayerPhysicsTable (ov000 0x02368b7c), per character.
 * Loaded at runtime from build/dreamcast/player/physics.bin (converter
 * tools/extract_physics.py); never compiled into the source. */
typedef struct DcpPhysics {
    s16 acc;          /* +0x00 -> @0x356 ground/air acceleration */
    s16 top;          /* +0x02 -> @0x358 top speed (halved in zone>=9 or act>=2) */
    s16 dec;          /* +0x04 -> @0x35A brake / friction */
    s16 sd_base;      /* +0x06 -> @0x35C spin dash base charge */
    s16 sd_add;       /* +0x08 -> @0x35E spin dash charge per press */
    s16 sd_max;       /* +0x0A -> @0x360 spin dash max charge */
    s16 roll_dec;     /* +0x0C -> @0x362 roll friction */
    s16 hs_top;       /* +0x0E -> @0x364 top speed in high-speed mode */
    s16 boost_acc;    /* +0x10 -> @0x366 */
    s16 boost_top;    /* +0x12 -> @0x368 boost speed (also min speed on boost start) */
    s16 boost_dec;    /* +0x14 -> @0x36A */
    s16 hs_min;       /* +0x16 -> @0x36C below this |move| high-speed mode ends */
    s16 slope_bonus;  /* +0x18 -> @0x36E top speed bonus * sin(angle) downhill */
    s16 p1a;          /* +0x1A -> @0x374 (gimmick use) */
    s16 invuln_len;   /* +0x1C -> @0x376 invulnerability frames after a hit */
    s16 fall_grace;   /* +0x1E -> @0x3CC (u8) frames on a wall/ceiling before falling off */
    s16 slope_acc;    /* +0x20 -> @0x03C slope acceleration factor */
    s16 gnd_max;      /* +0x22 -> @0x03E absolute ground speed cap */
    s16 roll_slope;   /* +0x24 -> slope_acc while rolling */
    s16 jump;         /* +0x26 -> @0x370 jump impulse */
    s16 gravity;      /* +0x28 -> @0x040 */
    s16 fall_max;     /* +0x2A -> @0x042 */
    s16 hs_time;      /* +0x2C high-speed timer limit (Player_UpdateHighSpeedTimer) */
    s16 ride_max;     /* +0x2E -> @0x048 */
} DcpPhysics;

/* --------------------------------------- object reference for riding */
/* What the object system exposes for platforms the player stands on/touches
 * (fields of the original StageObj header that StageObj_HandleRide reads). */
typedef struct DcpObjRef {
    s32 x, y, z;      /* @0x10 */
    s32 px, py, pz;   /* @0x1C previous position */
    s16 vx, vy, vz;   /* @0x28 last displacement */
    s16 max;          /* @0x48 */
    u32 obj_flags;    /* @0x54 (bit 2: being destroyed) */
    u32 move_flags;   /* @0x58 (0x100: use vx/vy/vz, 0x4000000: pushes) */
} DcpObjRef;

typedef void (*DcpFn)(DcPlayer *p);

/* ---------------------------------------------------------- player */
struct DcPlayer {
    u8 type;          /* @0x000 */
    u8 angle;         /* @0x001 ground angle */
    u8 rot2;          /* @0x002 */
    u8 rot3;          /* @0x003 */
    u8 slope_lim;     /* @0x004 slope-force threshold (0x1c, 0x0e rolling) */
    u8 grav_rot;      /* @0x005 gravity rotation (0 or 0x80) */
    s8 shake_x;       /* @0x006 */
    s8 shake_y;       /* @0x007 */
    s32 trick_input;  /* @0x008 */
    s32 timer;        /* @0x00C */
    s32 x, y, z;      /* @0x010 */
    s32 px, py, pz;   /* @0x01C */
    s16 move_x, move_y, move_z; /* @0x028 displacement applied this frame */
    s16 vel_x;        /* @0x02E */
    s16 vel_y;        /* @0x030 */
    s16 vel_z;        /* @0x032 */
    s16 gnd_vel;      /* @0x034 */
    s16 push_x, push_y, push_z; /* @0x036 */
    s16 slope_acc;    /* @0x03C */
    s16 gnd_max;      /* @0x03E */
    s16 gravity;      /* @0x040 */
    s16 fall_max;     /* @0x042 */
    s16 unk44, unk46; /* @0x044 */
    s16 ride_max;     /* @0x048 */
    u32 obj_flags;    /* @0x054 (bit0: collision layer 1) */
    u32 move_flags;   /* @0x058 DCP_MF_* */
    u16 disp_flags;   /* @0x05C DCP_DF_* */
    u16 hit;          /* @0x05E tile-attribute hits this frame */
    u16 prev_hit;     /* @0x060 */
    u16 hitstop;      /* @0x062 low nibble: freeze frames, high nibble: shake */
    s8 box_l, box_t, box_r, box_b; /* @0x064 map hitbox (-7,-12,7,13) */
    DcpFn state;      /* @0x068 */
    s32 col_x80_saved;/* (scratch) */
    DcpObjRef *ride;  /* @0x080 object stood on */
    DcpObjRef *touch; /* @0x084 object touched */
    u8 chara;         /* @0x324 0 = Sonic, 1 = Blaze */
    u8 pid;           /* @0x325 player index */
    u8 id;            /* @0x326 */
    u8 cam;           /* @0x327 */
    s16 anim;         /* @0x328 current animation id */
    s16 charge;       /* @0x32A spin dash charge / trick timer */
    u32 pf;           /* @0x32C DCP_PF_* */
    u32 pf2;          /* @0x330 gimmick / 3D flags */
    DcpFn act_idle;   /* @0x334 */
    DcpFn act_move;   /* @0x338 */
    DcpFn act_crouch; /* @0x33C */
    DcpFn act_jump;   /* @0x340 */
    DcpFn act_air;    /* @0x344 Sonic: homing attack, Blaze: hover */
    DcpFn act_348;    /* @0x348 */
    s16 tension;      /* @0x350 0..0x12c0 */
    s16 dash_timer;   /* @0x352 no-friction frames */
    s16 acc, top, dec, sd_base, sd_add, sd_max, roll_dec, hs_top; /* @0x356.. */
    s16 boost_acc, boost_top, boost_dec, hs_min, slope_bonus, jump; /* @0x366.. */
    s16 land_speed;   /* @0x372 */
    s16 p374;         /* @0x374 */
    s16 invuln_len;   /* @0x376 */
    s16 run_thr[5];   /* @0x378 0.15/0.3/0.5/0.7/0.9 * top */
    s16 hs_timer;     /* @0x3AC */
    s16 rings;        /* @0x3AE */
    s16 rings_total;  /* @0x3B0 */
    s16 invuln;       /* @0x3B2 */
    s16 invinc;       /* @0x3B4 */
    s16 tension_max;  /* @0x3B6 "tension max" (infinite boost) timer */
    s16 combo_timer;  /* @0x3B8 */
    s16 combo;        /* @0x3BA */
    s16 crush;        /* @0x3BC */
    s16 vs_timer;     /* @0x3BE */
    s16 confuse;      /* @0x3C0 */
    s16 slowdown;     /* @0x3C2 */
    s16 air_lock;     /* @0x3C4 */
    s16 saved_vx;     /* @0x3C6 */
    s16 respawn_inv;  /* @0x3C8 */
    s16 unk3CA;       /* @0x3CA */
    u8 fall_grace;    /* @0x3CC */
    u8 still_cnt;     /* @0x3CD */
    u8 boost_cool;    /* @0x3CE */
    u8 trick_cnt;     /* @0x3CF */
    u8 trick_lvl;     /* @0x3D0 */
    u8 fx_timer;      /* @0x3D1 */
    s16 jump_y;       /* @0x3DA */
    DcpObjRef *homing;/* @0x3DC homing target */
    s16 hover_timer;  /* @0x3E0 */
    s16 flame_cnt;    /* @0x3E2 */
    u8 *trick_obj;    /* @0x3E4 (byte +0xA: trick level counter) */
    void *gimmick;    /* @0x3E8 */
    u8 layer_ovr;     /* @0x410 */
    u8 layer_set;     /* @0x411 */
    u16 held;         /* @0x420 */
    u16 pressed;      /* @0x422 */
    u16 released;     /* @0x424 */
    u16 map_right, map_left, map_up, map_down, map_a, map_b; /* @0x42C remap */
    u16 pad_raw[3];   /* native: gPad held/pressed/released for Player_ReadInput */
    void *user;       /* native: owner pointer for hooks */
    s32 warp_x, warp_y; /* @0x408 @0x40C Battle Play: Player_WarpToRival target (respawn there, pf2 0x400000) */
    u8 vs_attack;     /* native, Battle Play: pf 0x20000 was set this frame (the rival reads it, Player_ApplyNetState) */
};

/* ------------------------------------------------- global game state */
typedef struct DcpWorld {
    u8 zone;          /* gGameState.unk1C */
    u8 act;           /* gGameState.unk1D */
    u32 mode;         /* gGameState.unk10 */
    u32 flags;        /* gGameState.unk18 (1: timer running, 0x20: VS, 0x40: special-stage return) */
    u32 frame;        /* gGameState.unk20 (stage frame counter) */
    u32 objmgr;       /* gObjManagerFlags (1: pause, 8: map collision on, 0x10: hit boxes on) */
    s32 map_left;     /* gStageMapCollision.unk1C (px) */
    s32 map_right;    /* gStageMapCollision.unk24 (px) */
    s32 death_y;      /* data_ov000_0236abd8.unk10 (px): die when (y>>8) >= death_y - 0x1a */
    s16 timescale;    /* data_0207cf7c (0x100 = 1.0) */
    s16 wind_x, wind_y; /* data_022c4ef8 */
    u32 rand_seed;    /* gRandSeed */
    s32 lives;        /* data_022c4590.unk14 */
    s32 check_x, check_y; /* data_022c4590.unk1C / unk20: respawn point (px) */
} DcpWorld;

extern DcpWorld g_dcp_world;

/* ----------------------------------------------------- side effects */
/* Everything that is not physics (sound, effects, animation playback, HUD,
 * rings, score, camera) goes out through these hooks. All are optional. */
enum { DCP_CH_VOICE = 0, DCP_CH_BOOST = 1, DCP_CH_TRICK = 2, DCP_CH_GLOBAL = 3 };
enum {
    DCP_FX_BRAKE_SMOKE, DCP_FX_BOOST, DCP_FX_BOOST_BURST, DCP_FX_HIGHSPEED, DCP_FX_TRAIL,
    DCP_FX_SPIN_SMOKE1, DCP_FX_SPIN_SMOKE2, DCP_FX_TRICK, DCP_FX_TRICK_COMBO,
    DCP_FX_BOOST_JET, DCP_FX_BLAZE_FLAME, DCP_FX_ATTACK_HITBOX_BOOST, DCP_FX_AIR_BUBBLE,
    DCP_FX_TRICK_FORWARD, /* Player_Action_TrickForward (Sonic): EffTrickForwardBoost_Create(w, 0, 0) */
    DCP_FX_TRICK_SPARKLE  /* Player_Action_TrickCombo: EffTrickSparkle_Create(a = combo level) */
};
typedef struct DcpHooks {
    void *ctx;
    void (*anim)(void *ctx, DcPlayer *p, s32 anim);                 /* Player_SetAnimation */
    void (*anim_speed)(void *ctx, DcPlayer *p, s32 speed);          /* Player_SetAnimSpeedFromVelocity */
    void (*sfx)(void *ctx, DcPlayer *p, s32 ch, s32 id);            /* Sound_PlaySeqArcEx */
    void (*sfx_stop)(void *ctx, DcPlayer *p, s32 ch);               /* NNS_SndPlayerStopSeq */
    void (*effect)(void *ctx, DcPlayer *p, s32 fx, s32 a, s32 b);   /* Eff*_Create / _Spawn */
    void (*shake)(void *ctx, s32 strength);                         /* ScreenShake_Start */
    void (*tension_hud)(void *ctx, DcPlayer *p, s32 v);             /* Hud_SetTensionGauge */
    void (*score)(void *ctx, DcPlayer *p, s32 points);              /* ScorePopup_Create + add */
    dcp_bool (*score_active)(void *ctx);                            /* ScorePopup_IsActive */
    void (*spawn_ring)(void *ctx, DcPlayer *p, s32 x, s32 y, s16 vx, s16 vy, u16 flags); /* RingManager_SpawnRing */
    void (*atkdef)(void *ctx, DcPlayer *p, s32 box, s32 atk, s32 def); /* ObjRect_SetAtkDefFlags */
    dcp_bool (*gravity_flipped)(void *ctx, s32 x, s32 y);           /* Stage_IsGravityFlipped */
    void (*fade)(void *ctx, s32 kind, s32 speed);                   /* BrightnessFade_Create */
    dcp_bool (*fade_done)(void *ctx);                               /* BrightnessFade_IsDone */
    void (*restart)(void *ctx, DcPlayer *p);                        /* StageRestart_Create / Camera_ResetOnRespawn */
    void (*add_life)(void *ctx, DcPlayer *p, s32 n);                /* Player_AddLives */
    void (*sfx_volume)(void *ctx, DcPlayer *p, s32 ch, s32 vol);    /* NNS_SndPlayerSetVolume (0..0x7f) */
    void (*model_speed)(void *ctx, DcPlayer *p, s32 fx);            /* w->unk124->unk114 = fx (3D anim speed, 0x1000 = 1) */
    /* Battle Play (appended): Player_AddTension VS branch (+0x12c0: TensionSound_Create(0x2f, 0x80) for the
     * local player + EffVsTensionMax_Create) and Player_DrainTension (TensionSound_Create(0x32, 0x80) +
     * EffVsTensionDrain_Create) */
    void (*vs_tension)(void *ctx, DcPlayer *p, s32 max);
} DcpHooks;

extern DcpHooks g_dcp_hooks;

/* ----------------------------------------------------------- tables */
/* sincos: 8192 s16 (sin, cos interleaved, 4096 steps); atan: 129 s16.
 * Generated by tools/extract_physics.py (computed, verified vs the ROM). */
void dcp_set_trig(const s16 *sincos8192, const s16 *atan129);
/* raw: little-endian gPlayerPhysicsTable bytes (n entries of 0x30) */
int dcp_set_physics(const u8 *raw, u32 size);
const DcpPhysics *dcp_physics(u8 chara);

/* ------------------------------------------------------ math (dcp_math.c) */
s16 dcp_sin(u16 a16);   /* data_0206e470[(a>>4)*2] */
s16 dcp_cos(u16 a16);
u16 dcp_atan2(s32 y, s32 x); /* FX_Atan2 (returns 16-bit angle) */
s32 dcp_spd_down(s32 v, s32 dec);          /* ObjSpd_DownSet */
s32 dcp_spd_up(s32 v, s32 acc, s32 lim);   /* ObjSpd_UpSet */
u8 dcp_angle_toward8(u8 cur, u8 tgt, u8 step); /* ObjAngle_MoveToward8 */
dcp_bool dcp_angle_upside_down(u32 a);     /* ObjAngle_IsUpsideDown */
void dcp_rotate_vec(s16 *x, s16 *y, s32 angle8); /* Math_RotateVec2S16 */
u16 dcp_rand(void);                        /* gRandSeed LCG */
/* (s32)(v * d) with IEEE double rounding, d = mant * 2^-shift (exact double) */
typedef struct { u64 mant; u32 shift; } DcpDouble;
s32 dcp_mul_double(s32 v, DcpDouble d);
/* exact binary values of the double literals used by Player_LoadPhysicsParams */
#define DCP_D_0_15 ((DcpDouble){ 0x13333333333333ull, 55 }) /* 0x1.3333333333333p-3 */
#define DCP_D_0_3 ((DcpDouble){ 0x13333333333333ull, 54 })  /* 0x1.3333333333333p-2 */
#define DCP_D_0_5 ((DcpDouble){ 0x10000000000000ull, 53 })  /* 0x1.0p-1 */
#define DCP_D_0_7 ((DcpDouble){ 0x16666666666666ull, 53 })  /* 0x1.6666666666666p-1 */
#define DCP_D_0_9 ((DcpDouble){ 0x1ccccccccccccdull, 53 })  /* 0x1.ccccccccccccdp-1 */

/* ------------------------------------------- object core (dcp_object.c) */
void dcp_obj_move(DcPlayer *p);            /* StageObj_Move */
void dcp_obj_collision(DcPlayer *p);       /* StageObj_Collision */
void dcp_collide_map(DcPlayer *p);         /* StageObj_CollideMap(Impl) */
void dcp_collide_horizontal(DcPlayer *p, dcp_bool no_move, s32 spd); /* StageObj_CollideMapHorizontal */
void dcp_collide_vertical(DcPlayer *p);    /* StageObj_CollideMapVertical (from asm) */
dcp_bool dcp_check_ceiling(DcPlayer *p);   /* StageObj_CheckCeiling */
s32 dcp_probe(DcPlayer *p, s32 x, s32 y, u32 flags, u32 dir, u8 *angle, u8 *attr); /* StageObj_ProbeCollision */
s32 dcp_probe_obj(DcPlayer *p, s32 x, s32 y, u32 flags, u32 dir, u8 *angle, u8 *attr);
u8 dcp_probe_flags(DcPlayer *p);           /* StageObj_GetMapProbeFlags */
void dcp_apply_attr(DcPlayer *p, u8 attr); /* StageObj_ApplyCollisionAttr */
void dcp_handle_ride(DcPlayer *p);         /* StageObj_HandleRide (ride/touch part) */

/* -------------------------------------------------- player (dcp_player.c) */
void dcp_player_create(DcPlayer *p, u8 chara, u8 pid, s32 x_px, s32 y_px); /* Player_Create (physics part) */
void dcp_player_init_state(DcPlayer *p);   /* Player_InitState */
void dcp_player_load_physics(DcPlayer *p); /* Player_LoadPhysicsParams */
/* one frame: StageObj_Main order for the player task (In, state, Move, Collision, Last) */
void dcp_player_frame(DcPlayer *p, u16 pad_held, u16 pad_pressed, u16 pad_released);
void dcp_player_in(DcPlayer *p);           /* Player_In */
void dcp_player_last(DcPlayer *p);         /* Player_Last */
u16 dcp_remap_buttons(DcPlayer *p, u16 in);/* Player_RemapButtons */
void dcp_set_anim(DcPlayer *p, s32 anim);  /* Player_SetAnimation (state part) */

/* states (Player_State_*) */
void dcp_state_ground_idle(DcPlayer *p);
void dcp_state_ground_move(DcPlayer *p);
void dcp_state_crouch(DcPlayer *p);
void dcp_state_roll(DcPlayer *p);
void dcp_state_spindash(DcPlayer *p);
void dcp_state_in_air(DcPlayer *p);
void dcp_state_homing(DcPlayer *p);
void dcp_state_hover(DcPlayer *p);
void dcp_state_hurt(DcPlayer *p);
void dcp_state_die(DcPlayer *p);
/* actions (Player_Action_*) */
void dcp_act_ground_idle(DcPlayer *p);
void dcp_act_ground_move(DcPlayer *p);
void dcp_act_crouch(DcPlayer *p);
void dcp_act_roll(DcPlayer *p);
void dcp_act_spindash(DcPlayer *p);
void dcp_act_jump(DcPlayer *p);
void dcp_act_fall(DcPlayer *p);
void dcp_act_land(DcPlayer *p, u8 ang);
void dcp_act_airdash(DcPlayer *p);
void dcp_act_homing(DcPlayer *p);
void dcp_act_hover(DcPlayer *p);
void dcp_act_trick_up(DcPlayer *p);
void dcp_act_trick_forward(DcPlayer *p);
void dcp_act_trick_combo(DcPlayer *p);
void dcp_act_launch(DcPlayer *p, s16 vx, s16 vy);     /* Player_Action_Launch */
void dcp_act_spring(DcPlayer *p, s16 vx, s16 vy);     /* Player_Action_SpringLaunch */
void dcp_act_dash_panel(DcPlayer *p, s16 vx, s16 vy); /* Player_Action_DashPanel */
void dcp_act_knockback(DcPlayer *p, s16 vx, s16 vy);  /* Player_Action_Knockback */
void dcp_act_die(DcPlayer *p);
/* Battle Play (Attract item): Player_Action_Warp / Player_State_Warp - shrink away, then
 * Player_Respawn at (warp_x, warp_y) (pf2 0x400000 set by the state) */
void dcp_act_warp(DcPlayer *p);
void dcp_state_warp(DcPlayer *p);
void dcp_on_hurt(DcPlayer *p, u32 hit_flags);         /* Player_OnHurt (hit_flags = attacker box unk20) */
/* helpers */
void dcp_apply_ground_input(DcPlayer *p);
void dcp_apply_air_input(DcPlayer *p);
void dcp_apply_vertical_input(DcPlayer *p);
void dcp_apply_slope_force(DcPlayer *p, const u8 *angle_override);
void dcp_apply_roll_friction(DcPlayer *p);
dcp_bool dcp_check_fall_off(DcPlayer *p);
void dcp_settle_air_rotation(DcPlayer *p);
void dcp_update_highspeed_timer(DcPlayer *p);
void dcp_start_highspeed(DcPlayer *p);
void dcp_stop_highspeed(DcPlayer *p);
void dcp_start_boost(DcPlayer *p);
void dcp_stop_boost(DcPlayer *p);
void dcp_update_boost(DcPlayer *p);
void dcp_drain_boost_tension(DcPlayer *p);
void dcp_add_tension(DcPlayer *p, s16 v);
void dcp_add_tension_combo(DcPlayer *p, s16 v);
void dcp_drain_tension(DcPlayer *p);
void dcp_add_rings(DcPlayer *p, s16 d);
void dcp_scatter_rings(DcPlayer *p, u8 *scatter_level); /* RingManager_ScatterRings */
extern u8 *g_dcp_scatter_level; /* RingManager unk9D5 for Player_OnHurt (set by the ring manager owner) */
void dcp_add_trick_score(DcPlayer *p, u32 b);
void dcp_stop_skid(DcPlayer *p);
void dcp_update_run_anim(DcPlayer *p);
void dcp_set_run_anim_for_speed(DcPlayer *p);
dcp_bool dcp_check_ledge_balance(DcPlayer *p, dcp_bool set);
void dcp_handle_surface_landing(DcPlayer *p);
void dcp_update_gravity_flip(DcPlayer *p);
void dcp_reset_gimmick_state(DcPlayer *p);
void dcp_respawn(DcPlayer *p);
void dcp_check_crushed(DcPlayer *p);
void dcp_update_ride_max(DcPlayer *p);     /* Player_Unk_ov000_023460ec */

#endif

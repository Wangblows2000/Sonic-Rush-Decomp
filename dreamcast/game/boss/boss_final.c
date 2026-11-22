/*
 * BossF (the final boss, stage zf3 = stage id 23, DS zone 7 act 2): native
 * translation of all 267 functions of src/game/Zone/Final_Zone/Boss/
 * (BossF_*, BossFPartD8..DE_*, BossStageF_*), plus the Final Zone stage
 * functions the fight needs: StageZoneScript_ZoneFBoss (camera shake) and
 * ZoneFx_ZoneFBoss_Init / _Update (the passage seen behind the arena).
 *
 * The boss is a giant robot standing behind the arena (bsf_body, part 0xd8)
 * on a flying base (bsf_base, BossF itself); the player fights on the base
 * (map x 0..0x340 px, the level floor at y 768).
 *
 *   BossF        (task 0x500, map object 0x38)  base model, sky, passage, laser-fin effects
 *   BossFPartD8  (task 0xf0, MapObject_Spawn(0xd8))  the body: the attack state machine, 7 hit
 *                boxes on the nodes body_core / body_fist_l / body_fist_r, and the arm's
 *                collision map (bsf_st_r.df/.di) the player walks on and rides
 *   BossFPartD9  (0x501, 0xd9)  bomb ball thrown from the core during the hand ride
 *   BossFPartDA  (0x501, 0xda)  shock wave rolled by the arm (bsf_shock)
 *   BossFPartDB  (0x501, 0xdb)  slam waves (bsf_wave_sl)
 *   BossFPartDC  (0x501, 0xdc)  laser from the core (eff_laser_fin), sweeps / chases the player
 *   BossFPartDD  (0x501, 0xdd)  laser impact on the floor (eff_laser_fin_b): the harmful part
 *   BossFPartDE  (0x501, 0xde)  beam ball (bsf_ball + two eff_beam_fin ends): drops, aims,
 *                sweeps a beam between its two ends
 *
 * Every function keeps its original name and body.  The per-file Unk structs
 * are merged with the original offsets in the field names.  BossFPartD8's
 * work words 0x434 / 0x438 / 0x43C / 0x440 are read as u16 by some states and
 * as s32 by others; they are unions (u434.w = the s32, unk434 / unk436 = the two
 * u16 halves, little endian as on the DS), and each function uses the width its
 * original file declares.  DS services are replaced as in boss4.c:
 *   - node callbacks (BossFPartD8_NodeCallback_Body: GE slots 0x1e..0x1a) ->
 *     boss_model_joint_world after the pose
 *   - palette swaps (Boss_LoadModelPalettes) -> selection state
 *   - material / pattern / SRT animations -> timing-only animation objects
 *   - BLDCNT / BLDY (gDisplayMain.unk20 / unk24) -> bossf_blend()
 *   - the visibility animation of the base (bsf.nsbva) -> drawn with dca_visibility
 *   - ObjCollisionMap_Register of the arm -> the objects module's colmap list, the
 *     owner seen by the player's probes is a proxy Obj kept at the part's position
 *   - BossHelp_Create / _Destroy, EffEnemyRoomFix_Create -> g_boss_hooks.help
 *   - TalkDemo_Create -> g_boss_hooks.talk
 */
#include <stdio.h>
#include <string.h>
#include "boss_internal.h"
#include "boss_final.h"

#ifdef _arch_dreamcast
#include <dc/pvr.h>
#endif

void rect_set_box2d(ObjBox *b, s16 l, s16 t, s16 r, s16 bb); /* ObjRect_SetBox2D */
void obj_player_on_hurt(ObjBox *body, ObjBox *attacker);     /* Player_OnHurt */
void colmap_register(ObjColMap *e);                          /* ObjCollisionMap_Register */
void colmap_forget(ObjColMap *e);                            /* drop a freed entry */

typedef dcp_bool BOOL;
#ifndef TRUE
#define TRUE DCP_TRUE
#define FALSE DCP_FALSE
#endif

typedef struct BossF BossF;
typedef struct BossFPartD8 BossFPartD8;
typedef struct BossFPartD9 BossFPartD9;
typedef struct BossFPartDA BossFPartDA; /* DA and DB share the layout */
typedef struct BossFPartDC BossFPartDC;
typedef struct BossFPartDD BossFPartDD;
typedef struct BossFPartDE BossFPartDE;

/* a work word read as s32 by some states and as two u16 by others */
typedef union {
    s32 w;
    u16 h[2];
} BFWord;

struct BossF {                /* 0x9D0 bytes (BossF_Create) */
    DcPlayer h;
    BossFn func;              /* @0x68 */
    ObjBox unk134, unk160, unk18C; /* MapObject_Init boxes (not armed) */
    u32 unk20C;               /* @0x20C 1 talk demo shown, 2/4/8 laser-fin effects (sky impacts 1/2/3),
                                 0x10 camera shake (StageZoneScript), 0x20 shake offset applied,
                                 0x40 the arm lies on the floor (bombs keep rolling) */
    u16 unk210;               /* @0x210 frame counter (laser-fin effects) */
    u16 unk212;               /* @0x212 attack pattern column */
    u16 unk214;               /* @0x214 attack pattern row (random at create) */
    BossFPartD8 *unk218;      /* @0x218 the body */
    BossDrawPlayerFn unk21C;  /* @0x21C gPlayers->unk70 saved at the start */
    const BossModelRes *unk220; /* @0x220 bsf_base */
    const BossAnimRes *unk224;  /* @0x224 "bsf" file 8 bsf.nsbca */
    const BossAnimRes *unk228;  /* @0x228 file 9 bsf.nsbva (visibility) */
    const BossAnimRes *unk22C;  /* @0x22C file 10 bsf.nsbta (timing only) */
    const BossAnimRes *unk230;  /* @0x230 file 11 bsf.nsbma (timing only) */
    const BossModelRes *unk234; /* @0x234 bsf_pas_dust01 */
    const BossModelRes *unk238; /* @0x238 /mod/bsf_sky.nsbmd (NULL once the sky_b is loaded) */
    const BossModelRes *unk23C; /* @0x23C /mod/bsf_sky_b.nsbmd */
    const BossModelRes *unk240; /* @0x240 /mod/bsf_passage.nsbmd (4 models) */
    BossModel unk244;         /* @0x244 base (speed @0x358) */
    BossModel unk37C[4];      /* @0x37C passage: sate01, peel01, clo00, clo01 */
    BossModel unk85C;         /* @0x85C passage dust */
    s32 unk994, unk998;       /* @0x994 ZoneFx stage 2 / 3 reached */
    s32 unk9A0;               /* @0x9A0 satellite side */
    s32 unk9A4, unk9A8;       /* @0x9A4 peel y (two copies) */
    s32 unk9AC, unk9B0;       /* @0x9AC cloud y */
    s32 unk9B4, unk9B8;       /* @0x9B4 cloud kinds */
    s32 unk9BC, unk9C0;       /* @0x9BC dust y */
    BossVec unk9C4;           /* @0x9C4 shake offset added to the camera (StageZoneScript_ZoneFBoss) */
    int alive;
};

struct BossFPartD8 {          /* 0x5A8 bytes (BossFPartD8_Create) */
    DcPlayer h;
    BossFn func;              /* @0x68 */
    BossFn unk78;             /* @0x78 BossFPartD8_Collide */
    ObjBox unk134, unk160, unk18C; /* MapObject_Init boxes (templates of coll[]) */
    ObjBox coll[7];           /* @0x20C body, fist L attack / crush / damage, fist R attack / crush / damage */
    fx32 unk340[12];          /* @0x340 "body_core" node (MtxFx43; unk364/368/36C = translation) */
    fx32 unk370[12];          /* @0x370 "body_sh_l" */
    fx32 unk3A0[12];          /* @0x3A0 "body_sh_r" (unk3C4/3C8 = translation) */
    fx32 unk3D0[12];          /* @0x3D0 "body_fist_l" */
    fx32 unk400[12];          /* @0x400 "body_fist_r" (unk424/428 = translation) */
    u16 unk430;               /* @0x430 bomb timer */
    u16 unk432;
    BFWord u434, u438, u43C, u440; /* @0x434..@0x443 (see the top of the file) */
    s32 unk444, unk448;       /* @0x444 sky charge start position y / z */
    s16 unk44C;               /* @0x44C sky charge lerp */
    u16 unk44E;
    u16 unk450;               /* @0x450 sky shake */
    u16 unk452;               /* @0x452 flash fade timer */
    u16 unk454;               /* @0x454 sky phase timer */
    u16 unk456;               /* @0x456 blink */
    s32 unk458;               /* @0x458 attack variant */
    u16 unk45C;               /* @0x45C pause after a stop (BossFPartD8_MoveTowardX) */
    u16 unk45E;               /* @0x45E RotY of the body */
    const u8 *unk460;         /* @0x460 "bsf" file 12 bsf_st_r.df (collision map tiles) */
    const u8 *unk464;         /* @0x464 file 13 bsf_st_r.di (tile angles) */
    const BossModelRes *unk468; /* @0x468 bsf_body */
    const BossAnimRes *unk46C;  /* @0x46C bsf.nsbca */
    BossModel unk470;         /* @0x470 body (flags[0] = unk57C, speed = unk584, anm[0] = unk554) */
    /* native */
    void *unk1B8;             /* StageObj @0x1B8: the collision map registered (self) or NULL */
    Obj cm;                   /* owner of the collision map as the objects module sees it (h / ref
                                 follow the part; cm.cmap = StageObj @0x1B8..@0x1DA) */
    BossEffect *tackle;       /* @0x434 holds the eff_tackle task from SkyChargeStart3 to Impact3 */
    int alive;
};
#define unk434 u434.h[0]
#define unk436 u434.h[1]
#define unk434w u434.w
#define unk438 u438.h[0]
#define unk43A u438.h[1]
#define unk438w u438.w
#define unk43C u43C.h[0]
#define unk43E u43C.h[1]
#define unk43Cw u43C.w
#define unk440 u440.h[0]
#define unk442 u440.h[1]
#define unk440w u440.w
#define unk57C unk470.flags[0]
#define unk584 unk470.speed
/* node translations (VecFx32 views of the MtxFx43s) */
#define unk364 unk340[9]
#define unk368 unk340[10]
#define unk36C unk340[11]
#define unk3C4 unk3A0[9]
#define unk3C8 unk3A0[10]
#define unk3F4 unk3D0[9]
#define unk3F8 unk3D0[10]
#define unk424 unk400[9]
#define unk428 unk400[10]

struct BossFPartD9 {          /* BossFPartD9_Create */
    DcPlayer h;
    BossFn func;
    BossFn unk78;
    ObjBox coll[3];           /* @0x134 OnHit, @0x160 */
    u16 unk20C;               /* @0x20C roll angle (RotZ) */
    u16 unk20E;               /* @0x20E RotY */
    s32 unk210, unk214, unk218; /* @0x210 start position (fx32, world) */
    u16 unk21C;               /* @0x21C fly lerp */
    const BossModelRes *unk220;
    const BossAnimRes *unk224;
    BossModel unk228;
    int alive;
};

struct BossFPartDA {          /* BossFPartDA_Create / BossFPartDB_Create */
    DcPlayer h;
    BossFn func;
    BossFn unk78;
    ObjBox coll[3];           /* @0x160 harmful */
    const BossModelRes *unk20C;
    const BossAnimRes *unk210;
    BossModel unk214;
    int alive;
};

struct BossFPartDC {          /* BossFPartDC_Create */
    DcPlayer h;
    BossFn func;
    BossFn unk78;
    ObjBox coll[3];
    u32 unk20C;               /* @0x20C 1 reached the path end, 2 chase, 4 chase left, 8 fading, 0x10 glow gone */
    s32 unk210;               /* @0x210 frame counter */
    s16 unk214;               /* @0x214 speed when the fade starts */
    s16 unk216;               /* @0x216 fade */
    s16 unk218;               /* @0x218 chase acceleration */
    u16 unk21A;               /* @0x21A chase turn timer */
    s16 unk21C;               /* @0x21C distance since the last impact */
    s32 unk220, unk224, unk228; /* @0x220 laser end on the floor (24.8) */
    s32 unk22C, unk230, unk234; /* @0x22C path end */
    const BossModelRes *unk238; /* "bsf" file 0x6e eff_laser_fin.nsbmd */
    BossModel unk23C;         /* scale x @0x254, z @0x25C */
    int alive;
};

struct BossFPartDD {          /* BossFPartDD_Create */
    DcPlayer h;
    BossFn func;
    BossFn unk78;
    ObjBox coll[3];           /* @0x160 harmful (frames 0..8) */
    const BossModelRes *unk20C; /* file 0x6f eff_laser_fin_b.nsbmd */
    const BossAnimRes *unk210;  /* 0x70 .nsbma (timing only) */
    const BossAnimRes *unk214;  /* 0x71 .nsbtp (timing only) */
    BossModel unk218;         /* flags[1] = unk325, flags[2] = unk326 */
    int alive;
};

typedef struct BossFBeamEnd { /* 0x144 bytes: position + model */
    s32 x, y, z;              /* +0x00 (24.8) */
    BossModel m;              /* +0x0C eff_beam_fin */
} BossFBeamEnd;

struct BossFPartDE {          /* BossFPartDE_Create */
    DcPlayer h;
    BossFn func;
    BossFn unk78;
    ObjBox coll[3];           /* @0x134 the beam (BossFPartDE_OnHit) */
    s32 unk20C;               /* @0x20C kind: 0 single, 1 chain */
    u16 unk210;               /* @0x210 RotX */
    u16 unk212;               /* @0x212 RotZ */
    s16 unk214;               /* @0x214 chain index */
    BossFBeamEnd e[2];        /* @0x218 (unk218 / unk21C / unk220, unk224 model) and @0x35C (unk35C..) */
    s32 unk4A0;               /* @0x4A0 beam ends shown */
    s32 unk4A4;               /* @0x4A4 beam ends see-through (wireframe alpha) */
    u32 unk4A8;               /* @0x4A8 sound handle (native: 1..3) */
    s16 unk4AC;               /* @0x4AC resize lerp */
    u16 unk4AE;               /* @0x4AE pause timer */
    u16 unk4B0;               /* @0x4B0 beam-fin effect counter */
    s32 unk4B4;               /* @0x4B4 resize target */
    s32 unk4B8;               /* @0x4B8 width */
    s16 unk4BC;               /* @0x4BC resize speed */
    u16 unk4BE;               /* @0x4BE sweeps done */
    s32 unk4C0;               /* @0x4C0 sweep direction */
    u16 unk4C4;               /* @0x4C4 sweep timer */
    s16 unk4C6;               /* @0x4C6 end shift */
    const BossModelRes *unk4C8; /* bsf_ball */
    const BossModelRes *unk4CC; /* file 0x72 eff_beam_fin.nsbmd */
    const BossAnimRes *unk4D0;  /* 0x73 eff_beam_fin.nsbtp (timing only) */
    BossModel unk4D4;         /* ball */
    int alive;
};
#define unk218x e[0].x
#define unk35Cx e[1].x

/* pools (the DS allocates a task per object) */
#define BF_D9 8
#define BF_DA 8
#define BF_DB 12
#define BF_DC 4
#define BF_DD 24
#define BF_DE 12
static BossF s_bf;
static BossFPartD8 s_d8;
static BossFPartD9 s_d9[BF_D9];
static BossFPartDA s_da[BF_DA];
static BossFPartDA s_db[BF_DB];
static BossFPartDC s_dc[BF_DC];
static BossFPartDD s_dd[BF_DD];
static BossFPartDE s_de[BF_DE];
static BossF *gBossF;
static BossFStats s_st;

/* BossStageF resources: data_ov000_02369b88[6] = bsf_base, bsf_body, bsf_ball, bsf_shock, bsf_wave_sl,
 * bsf_pas_dust01 (data_02074498), the /mod files bsf_sky, bsf_sky_b, bsf_passage (4 models),
 * and the zf3_act_lz7.narc ("bsf") files the parts read */
static BossModelRes data_ov000_02369b88[6];
static BossModelRes s_sky, s_sky_b, s_passage[4];
static BossAnimRes s_bsf_ca, s_bsf_va, s_bsf_ta, s_bsf_ma;     /* files 8, 9, 10, 11 */
static BossModelRes s_laser_fin, s_laser_fin_b, s_beam_fin;    /* 0x6e, 0x6f, 0x72 */
static BossAnimRes s_lfb_ma, s_lfb_tp, s_bf_tp;               /* 0x70, 0x71, 0x73 */
static const u8 *s_st_r_df, *s_st_r_di;                        /* 12, 13: bsf_st_r.df / .di */
static int s_joint[5];        /* body_core, body_sh_l, body_sh_r, body_fist_l, body_fist_r (bsf_body) */
static s32 s_chara;           /* gGameState.unk0 (story character: palette tables, talk demo id) */
static s32 s_pal[4][2];       /* native: Boss_LoadModelPalettes state: [all, A, B, one][first index, flash] */
static s32 s_bld_mode, s_bld_evy; /* gDisplayMain.unk20 bits 6-7 / unk24 (BLDY), BG0 target */
static u32 s_snd_next;        /* native: handles for the looping beam sounds */
static int s_pre_ran;         /* boss_update_pre ran BossFPartD8 this frame */

/* rodata data_ov000_02363da4..023643d4 / data data_ov000_02365c54..023661a4 (bossdata.bin) */
#define data_ov000_02363da4 BOSS_DATA(0x02363da4, const s8 *)
#define data_ov000_02363da8 BOSS_DATA(0x02363da8, const s8 *)
#define data_ov000_02363db0 BOSS_DATA(0x02363db0, const u8 *)
#define data_ov000_02363db4 BOSS_DATA(0x02363db4, const s8 *)
#define data_ov000_02363db8 BOSS_DATA(0x02363db8, const u16 *)
#define data_ov000_02363dbc BOSS_DATA(0x02363dbc, const u16 *)
#define data_ov000_02363dc0 BOSS_DATA(0x02363dc0, const s16 *)
#define data_ov000_02363dc4 BOSS_DATA(0x02363dc4, const s8(*)[2])
#define data_ov000_02363dc8 BOSS_DATA(0x02363dc8, const s16 *)
#define data_ov000_02363dcc BOSS_DATA(0x02363dcc, const s16 *)
#define data_ov000_02363dd0 BOSS_DATA(0x02363dd0, const u16 *)
#define data_ov000_02363dd4 BOSS_DATA(0x02363dd4, const u16 *)
#define data_ov000_02363dd8 BOSS_DATA(0x02363dd8, const u16 *)
#define data_ov000_02363ddc BOSS_DATA(0x02363ddc, const u16 *)
#define data_ov000_02363de0 BOSS_DATA(0x02363de0, const s8 *)
#define data_ov000_02363de4 BOSS_DATA(0x02363de4, const s16 *)
#define data_ov000_02363df0 BOSS_DATA(0x02363df0, const u8 *)
#define data_ov000_02363e00 BOSS_DATA(0x02363e00, const u8(*)[4])
#define data_ov000_02363e08 BOSS_DATA(0x02363e08, const u8(*)[4])
#define data_ov000_02363e10 BOSS_DATA(0x02363e10, const s8(*)[4])
#define data_ov000_02363e18 BOSS_DATA(0x02363e18, const s8(*)[4])
#define data_ov000_02363e20 BOSS_DATA(0x02363e20, const u8(*)[4])
#define data_ov000_02363e28 BOSS_DATA(0x02363e28, const s8(*)[4])
#define data_ov000_02363e30 BOSS_DATA(0x02363e30, const s8(*)[4])
#define data_ov000_02363e38 BOSS_DATA(0x02363e38, const s8(*)[4])
#define data_ov000_02363e40 BOSS_DATA(0x02363e40, const u8(*)[4])
#define data_ov000_02363e48 BOSS_DATA(0x02363e48, const u8 *)
#define data_ov000_02363e54 BOSS_DATA(0x02363e54, const s16(*)[3])
#define data_ov000_02363e60 BOSS_DATA(0x02363e60, const s16(*)[3])
#define data_ov000_02363e6c BOSS_DATA(0x02363e6c, const u16(*)[3])
#define data_ov000_02363e78 BOSS_DATA(0x02363e78, const u16(*)[3])
#define data_ov000_02363e84 BOSS_DATA(0x02363e84, const u16(*)[3])
#define data_ov000_02363e90 BOSS_DATA(0x02363e90, const u16(*)[3])
#define data_ov000_02363e9c BOSS_DATA(0x02363e9c, const u8(*)[8])
#define data_ov000_02363eac BOSS_DATA(0x02363eac, const u8(*)[2][4])
#define data_ov000_02363ebc BOSS_DATA(0x02363ebc, const u8(*)[2][4])
#define data_ov000_02363ecc BOSS_DATA(0x02363ecc, const u8(*)[2][4])
#define data_ov000_02363edc BOSS_DATA(0x02363edc, const u8(*)[8])
#define data_ov000_02363eec BOSS_DATA(0x02363eec, const u8(*)[2][4])
#define data_ov000_02363efc BOSS_DATA(0x02363efc, const char *) /* "body_core" */
#define data_ov000_02363f0c BOSS_DATA(0x02363f0c, const char *) /* "body_sh_l" */
#define data_ov000_02363f1c BOSS_DATA(0x02363f1c, const char *) /* "body_sh_r" */
#define data_ov000_02363f2c BOSS_DATA(0x02363f2c, const char *) /* "body_fist_l" */
#define data_ov000_02363f3c BOSS_DATA(0x02363f3c, const char *) /* "body_fist_r" */
#define data_ov000_02363f4c BOSS_DATA(0x02363f4c, const u16 *)
#define data_ov000_02363f60 BOSS_DATA(0x02363f60, const u16 *)
#define data_ov000_02363f7c BOSS_DATA(0x02363f7c, const u16(*)[8])
#define data_ov000_02363f9c BOSS_DATA(0x02363f9c, const u16(*)[8])
#define data_ov000_02363fbc BOSS_DATA(0x02363fbc, const s16(*)[8])
#define data_ov000_02363fdc BOSS_DATA(0x02363fdc, const u16(*)[8])
#define data_ov000_02363ffc BOSS_DATA(0x02363ffc, const u16(*)[9])
#define data_ov000_02364020 BOSS_DATA(0x02364020, const s16(*)[9])
#define data_ov000_02364044 BOSS_DATA(0x02364044, const s16(*)[9])
#define data_ov000_02364068 BOSS_DATA(0x02364068, const s16(*)[10])
#define data_ov000_02364090 BOSS_DATA(0x02364090, const s16(*)[10])
#define data_ov000_023640b8 BOSS_DATA(0x023640b8, const u16(*)[13])
#define data_ov000_023640ec BOSS_DATA(0x023640ec, const u16(*)[13])
#define data_ov000_02364120 BOSS_DATA(0x02364120, const u16(*)[5])
#define data_ov000_0236415c BOSS_DATA(0x0236415c, const s16(*)[5])
#define data_ov000_02364198 BOSS_DATA(0x02364198, const s32(*)[10])
#define data_ov000_023641e8 BOSS_DATA(0x023641e8, const u16(*)[8][3])
#define data_ov000_02364248 BOSS_DATA(0x02364248, const u16(*)[6])
#define data_ov000_023642b4 BOSS_DATA(0x023642b4, const u16(*)[8])
#define data_ov000_02364344 BOSS_DATA(0x02364344, const u16(*)[8])
#define data_ov000_02365c54 BOSS_DATA(0x02365c54, const u16 *)
#define data_ov000_02365c5c BOSS_DATA(0x02365c5c, const u8(*)[3])
#define data_ov000_02365da0 BOSS_DATA(0x02365da0, const s16(*)[3][8])
#define data_ov000_02365e00 BOSS_DATA(0x02365e00, const u16(*)[3][8])
#define data_ov000_02365e20 BOSS_DATA(0x02365e20, const u16(*)[0x18])
#define data_ov000_02365e60 BOSS_DATA(0x02365e60, const s32(*)[3][8])

#define CAM data_ov000_0236a444
#define gGameState s_chara
#define gStageMapCollision_unk1C g_dcp_world.map_left
#define gStageMapCollision_unk24 g_dcp_world.map_right
#define gStageMapCollision_unk18 (s_map_w_blocks)
static s32 s_map_w_blocks;    /* gStageMapCollision.unk18: map width in 64 px blocks */

/* ============================================================ helpers */
static inline BOOL IsMode23(void) { return g_dcp_world.mode == 2 || g_dcp_world.mode == 3; }
static inline s32 GetLevel(void) { return IsMode23() ? 1 : g_boss_game.unk14; }
static inline s32 GetIdxB(void) {
    BOOL b = TRUE;
    if (g_dcp_world.mode != 2 && g_dcp_world.mode != 3)
        b = FALSE;
    if (b)
        return 1;
    return g_boss_game.unk14;
}
static inline s32 GetIdxS(void) {
    switch (g_dcp_world.mode) {
    case 2:
    case 3:
        return 1;
    }
    return g_boss_game.unk14;
}
static inline s32 Mul64(s64 a, s64 b) { return (s32)((a * b + 0x800) >> 12); }
#define FX_MUL(a, b) FX_Mul((a), (b))
static inline s32 FX_DivS32(s32 a, s32 b) { return b != 0 ? a / b : 0; }
static inline u16 FX_Atan2(s32 y, s32 x) { return dcp_atan2(y, x); }

/* FX_Sqrt (NitroSDK square-root unit: 64-bit root of v << 32, rounded to fx32) */
static u32 isqrt64(u64 v) {
    u64 r = 0, bit = (u64)1 << 62;
    while (bit > v)
        bit >>= 2;
    while (bit) {
        if (v >= r + bit) {
            v -= r + bit;
            r = (r >> 1) + bit;
        } else
            r >>= 1;
        bit >>= 2;
    }
    return (u32)r;
}
static s32 FX_Sqrt(s32 v) {
    if (v > 0)
        return (s32)((isqrt64((u64)v << 32) + 0x200) >> 10);
    return 0;
}
/* VEC_* (NitroSDK fx: 64-bit products, the dot / cross rounded with + 0x800) */
static inline void VEC_Subtract(const BossVec *a, const BossVec *b, BossVec *o) {
    o->x = a->x - b->x;
    o->y = a->y - b->y;
    o->z = a->z - b->z;
}
static inline void VEC_Add(const BossVec *a, const BossVec *b, BossVec *o) {
    o->x = a->x + b->x;
    o->y = a->y + b->y;
    o->z = a->z + b->z;
}
static fx32 VEC_DotProduct(const BossVec *a, const BossVec *b) {
    return (fx32)(((s64)a->x * b->x + (s64)a->y * b->y + (s64)a->z * b->z + 0x800) >> 12);
}
static void VEC_CrossProduct(const BossVec *a, const BossVec *b, BossVec *o) {
    BossVec t;
    t.x = (fx32)(((s64)a->y * b->z - (s64)a->z * b->y + 0x800) >> 12);
    t.y = (fx32)(((s64)a->z * b->x - (s64)a->x * b->z + 0x800) >> 12);
    t.z = (fx32)(((s64)a->x * b->y - (s64)a->y * b->x + 0x800) >> 12);
    *o = t;
}
static void VEC_MultAdd(fx32 a, const BossVec *v1, const BossVec *v2, BossVec *d) {
    d->x = (fx32)(((s64)a * v1->x) >> 12) + v2->x;
    d->y = (fx32)(((s64)a * v1->y) >> 12) + v2->y;
    d->z = (fx32)(((s64)a * v1->z) >> 12) + v2->z;
}
static fx32 VEC_Distance(const BossVec *a, const BossVec *b) {
    s64 t, d;
    d = a->x - b->x;
    t = d * d;
    d = a->y - b->y;
    t += d * d;
    d = a->z - b->z;
    t += d * d;
    return (fx32)(((s32)isqrt64((u64)(t * 4)) + 1) >> 1);
}
static void VEC_Normalize(const BossVec *s, BossVec *d) {
    s64 t = (s64)s->x * s->x + (s64)s->y * s->y + (s64)s->z * s->z;
    s64 q;
    fx32 sq;
    if (t == 0) {
        d->x = d->y = d->z = 0;
        return;
    }
    q = (s64)(0x0100000000000000ULL / (u64)t); /* DIVCNT 2: 64/64 */
    sq = (fx32)isqrt64((u64)(t * 4));
    q = q * sq;
    d->x = (fx32)((q * s->x + (1LL << 44)) >> 45);
    d->y = (fx32)((q * s->y + (1LL << 44)) >> 45);
    d->z = (fx32)((q * s->z + (1LL << 44)) >> 45);
}
/* Math_LineFromPoints / Math_LineClosestPoint / Math_SegmentHitsSphere (src/game/GameSystem, copied):
 * a line is { direction, the point closest to the origin } */
typedef struct {
    BossVec d, p;
} BossFLine;
static void Math_LineFromPoints(const BossVec *p, const BossVec *q, BossFLine *o) {
    VEC_Subtract(q, p, &o->d);
    VEC_Normalize(&o->d, &o->d);
    VEC_MultAdd(-VEC_DotProduct(&o->d, p), &o->d, p, &o->p);
}
static void Math_LineClosestPoint(const BossFLine *s, const BossVec *b, BossVec *c) {
    s32 x = VEC_DotProduct(&s->d, b);
    VEC_MultAdd(x - VEC_DotProduct(&s->d, &s->p), &s->d, &s->p, c);
}
static BOOL Math_SegmentHitsSphere(const BossFLine *a, const BossVec *p0, const BossVec *p1, fx32 r, const BossVec *c) {
    BossVec n, d0, d1;
    Math_LineClosestPoint(a, c, &n);
    VEC_Subtract(p0, &n, &d0);
    VEC_Subtract(p1, &n, &d1);
    VEC_Subtract(&n, c, &n);
    if ((s64)r * r < (s64)n.x * n.x + (s64)n.y * n.y + (s64)n.z * n.z)
        return FALSE;
    return VEC_DotProduct(&d0, &d1) <= 0;
}

/* float joint matrix -> MtxFx43 (NNS_G3dGetCurrentMtx) */
static void mtx_to_fx43(const dcm_mtx_t *m, fx32 *out) {
    int r, c;
    for (r = 0; r < 4; r++)
        for (c = 0; c < 3; c++) {
            float v = m->m[r][c] * 4096.0f;
            out[r * 3 + c] = (fx32)(v >= 0 ? v + 0.5f : v - 0.5f);
        }
}

/* the player's model translation (gPlayers->unk17C, fx32): the boss transform when it draws the player,
 * otherwise the StageObj_Draw3D position */
static void player_model_pos(s32 *x, s32 *y, s32 *z) {
    if (g_boss_game.player_trans_valid) {
        *x = g_boss_game.player_trans[0];
        *y = g_boss_game.player_trans[1];
        *z = g_boss_game.player_trans[2];
        return;
    }
    *x = gPlayers->x << 4;
    *y = -(gPlayers->y << 4);
    *z = gPlayers->z << 4;
}

static void Player_State_Blank(DcPlayer *p) { (void)p; }
/* Player_Action_Blank (the player module lacks it) */
static void Player_Action_Blank(DcPlayer *s) {
    dcp_player_init_state(s);
    dcp_reset_gimmick_state(s);
    s->state = Player_State_Blank;
}
#define Player_SetAnimation(p, a) dcp_set_anim((p), (a))
#define Player_Action_Knockback(p, vx, vy) dcp_act_knockback((p), (s16)(vx), (s16)(vy))
#define Player_Action_AttackBounce(p) obj_player_attack_bounce(p)
#define Player_Action_GroundIdle(p) dcp_act_ground_idle(p)
#define Player_OnHurt(a, b) obj_player_on_hurt((a), (b))

/* hooks (BossHelp_Create / BossHelp_Destroy are the shared ones in boss_core.c: g_boss_hooks.help) */
static void bf_help_create(s32 idx) {
    s_st.helps++;
    BossHelp_Create((u32)idx);
}
static void bf_help_destroy(void) {
    s_st.help_destroys++;
    BossHelp_Destroy();
}
#define BossHelp_Create_F(i) bf_help_create(i)
#define BossHelp_Destroy_F() bf_help_destroy()
/* EffEnemyRoomFix_Create (src/game/UI/Hud: two HUD sprites at screen (x, y), b frames) -> g_boss_hooks.help
 * kind BOSS_HELP_ENEMY_ROOM_FIX (a = x, b = frames; y 0x30, not flipped in the BossF call) */
static void EffEnemyRoomFix_Create(s32 x, s16 y, s32 flip, s32 c) {
    (void)y;
    (void)flip;
    if (g_boss_hooks.help)
        g_boss_hooks.help(g_boss_hooks.ctx, BOSS_HELP_ENEMY_ROOM_FIX, x, c);
}
static void TalkDemo_Create(s32 id) {
    s_st.talks++;
    boss_log("talk", id, 0);
    if (g_boss_hooks.talk)
        g_boss_hooks.talk(g_boss_hooks.ctx, id);
}
static void Sound_PlaySeq(s32 seq) {
    if (g_boss_hooks.music)
        g_boss_hooks.music(g_boss_hooks.ctx, seq);
}
static void NNS_SndPlayerStopSeqAll(void) {
    if (g_boss_hooks.music_stop)
        g_boss_hooks.music_stop(g_boss_hooks.ctx);
}
static void RingManager_SetFlags1(void) {
    if (g_boss_hooks.ring_flags)
        g_boss_hooks.ring_flags(g_boss_hooks.ctx, 1);
}

/* Boss_LoadModelPalettes: selection state only (the textures are pre-decoded) */
static void Boss_LoadModelPalettes(int set, const u16 *idx, int flash) {
    s_pal[set][0] = idx != NULL ? idx[0] : -1;
    s_pal[set][1] = flash;
}

/* ============================================================ palettes */
/* BossF_LoadModelPalettesAll (modified: palette selection state) */
static void BossF_LoadModelPalettesAll(s32 k, BOOL f) {
    const u16 *t;
    if (f)
        t = data_ov000_023640ec[k];
    else
        t = data_ov000_023640b8[k];
    Boss_LoadModelPalettes(0, t, f); /* bsf_body, f_00..f_12_1p_pl, slots data_ov000_02363f60, 13 */
}
/* BossFPartD8_LoadModelPalettesA (modified) */
static void BossFPartD8_LoadModelPalettesA(s32 k, BOOL f) {
    const u16 *t;
    if (f)
        t = data_ov000_02363e90[k];
    else
        t = data_ov000_02363e84[k];
    Boss_LoadModelPalettes(1, t, f); /* f_04/f_09/f_05 (fist L), slots data_ov000_02363de8 */
}
/* BossFPartD8_LoadModelPalettesB (modified) */
static void BossFPartD8_LoadModelPalettesB(s32 k, BOOL f) {
    const u16 *t;
    if (f)
        t = data_ov000_02363e78[k];
    else
        t = data_ov000_02363e6c[k];
    Boss_LoadModelPalettes(2, t, f); /* f_07/f_10/f_06 (fist R), slots data_ov000_02363df8 */
}
/* BossF_LoadPalette (modified) */
static void BossF_LoadPalette(s32 a, s32 b) {
    const u16 *t;
    if (b)
        t = &data_ov000_02363ddc[a];
    else
        t = &data_ov000_02363dd0[a];
    Boss_LoadModelPalettes(3, t, b); /* one palette of bsf_body, slot data_ov000_02363dac */
}

/* ============================================================ BLDCNT / BLDY */
static void bld_set(s32 mode, s32 evy) {
    s_bld_mode = mode;
    s_bld_evy = evy & 0x1f;
}
static void bld_evy_add(s32 d) { s_bld_evy = (s_bld_evy + d) & 0x1f; } /* 5-bit field */

/* ============================================================ objects */
/* MapObject_Init (obj_manager.c obj_mapobject_init) fields used by the parts */
static void MapObject_Init(DcPlayer *h, ObjBox *coll, const ObjParam *prm, s32 x, s32 y) {
    int i;
    h->obj_flags |= 0x40;
    h->x = x;
    h->y = y;
    h->type = (prm != NULL && prm->type < 0x40) ? 2 : 3;
    h->gravity = 0x2a;
    h->fall_max = 0xf00;
    h->obj_flags |= 1;
    h->disp_flags |= 1;
    h->move_flags |= 0x200;
    for (i = 0; i < 3; i++)
        coll[i].mask = 0x12;
    coll[0].cb = NULL; /* MapObject_OnHit_Default (replaced by the callers) */
    rect_set_atkdef(&coll[0], 9, 0xa);
    rect_set_atkdef(&coll[1], 0x13, 0x13);
    rect_set_atkdef(&coll[2], 0x13, 0x13);
    for (i = 0; i < 3; i++)
        coll[i].flags &= ~4;
}
/* MapObject_Destructor: drop the boxes */
static void boxes_forget(ObjBox *b, int n) {
    int i;
    for (i = 0; i < n; i++)
        rect_forget(&b[i]);
}
/* MapObject_Collide_Default: the boxes with an owner, every frame */
static void collide_default(DcPlayer *h, ObjBox *coll) {
    int i;
    if (h->obj_flags & 0xc)
        return;
    for (i = 0; i < 3; i++)
        if (coll[i].owner != NULL)
            obj_handle_collider(h, coll[i].owner, OBJ_OWNER_OBJECT, &coll[i], 0);
}

/* StageObj_Main with the ride and map-collision steps (obj_manager.c obj_main): BossFPartD9 rolls
 * on the map and on the arm (move flags without 0x100); the other parts go through boss_obj_main */
static void bf_obj_main_map(DcPlayer *h, BossFn *state, const BossObjOps *ops, void *self, int *alive) {
    u16 saved = 0;
    int restore = 0;
    if (h->obj_flags & 4) {
        if (ops->dtor)
            ops->dtor(self);
        *alive = 0;
        return;
    }
    if (h->obj_flags & 8)
        h->obj_flags |= 4;
    if ((h->obj_flags & 0x10) == 0) {
        if (obj_view_out(h->x, h->y, 0, 0, 0, 0, 0))
            h->obj_flags |= 4;
    }
    dcp_handle_ride(h);
    if (!obj_pause_check(h->obj_flags)) {
        if (h->hitstop != 0) {
            if (h->hitstop & 0xf)
                h->hitstop--;
            if (h->hitstop & 0xf0)
                h->hitstop -= 0x10;
        }
        if (!obj_pause_check(h->obj_flags)) {
            if ((h->hitstop & 0xf) == 0 && (h->obj_flags & 0x80) == 0 && *state != NULL)
                (*state)(self);
            if ((h->move_flags & 0x2000) == 0)
                dcp_obj_move(h);
            if ((g_dcp_world.objmgr & 8) && (h->move_flags & 0x100) == 0) {
                h->ride = NULL;
                h->touch = NULL;
                dcp_obj_collision(h);
            }
        }
    }
    if ((h->hitstop & 0xf) || obj_pause_check(h->obj_flags)) {
        saved = h->disp_flags;
        h->disp_flags = saved | 0x10;
        restore = 1;
    }
    if ((h->obj_flags & 4) == 0 && ops->draw != NULL)
        ops->draw(self);
    if (restore)
        h->disp_flags = saved;
    if (obj_pause_check(h->obj_flags))
        return;
    if ((h->hitstop & 0xf) == 0 && (g_dcp_world.objmgr & 0x10) && ops->collide != NULL)
        ops->collide(self);
    h->push_x = 0;
    h->push_y = 0;
    h->push_z = 0;
    h->shake_x = 0;
    h->shake_y = 0;
}

/* draw instances: Model_Draw calls of this frame (a model drawn twice keeps both matrices) */
#define BF_INST_MAX 96
static struct {
    const BossModel *m;
    dcm_mtx_t mtx;
    const u8 *vis;
    u8 alpha;
} s_inst[BF_INST_MAX];
static int s_ninst;
static u8 s_base_vis[DCM_MAX_JOINTS];
static void Model_Draw_inst(BossModel *m, const u8 *vis, u8 alpha) {
    boss_model_pose(m, NULL);
    if (!m->posed || m->res == NULL || !m->res->loaded)
        return;
    if (s_ninst < BF_INST_MAX) {
        s_inst[s_ninst].m = m;
        s_inst[s_ninst].mtx = m->mdl_mtx;
        s_inst[s_ninst].vis = vis;
        s_inst[s_ninst].alpha = alpha;
        s_ninst++;
    }
}
#define Model_Draw(m) Model_Draw_inst((m), NULL, 255)

/* MapObject_Spawn(type, x, y, flags, ...) for the BossF parts */
static void *MapObject_Spawn(u16 type, s32 x, s32 y, u16 flags);

#define STATE(name) static void name(void *p_)
#define D8 BossFPartD8 *s = (BossFPartD8 *)p_
/* BossFPartD8 states / actions */
STATE(BossFPartD8_State_Idle);
STATE(BossFPartD8_State_WaitNextAttack);
STATE(BossFPartD8_State_HandSlamRaise);
STATE(BossFPartD8_State_HandSlamRaiseWait);
STATE(BossFPartD8_State_HandSlamNext);
STATE(BossFPartD8_State_HandSlamTrack);
STATE(BossFPartD8_State_HandSlamStrike);
STATE(BossFPartD8_State_HandSlamImpact);
STATE(BossFPartD8_State_HandSlamStartRecover);
STATE(BossFPartD8_State_HandSlamVulnerable);
STATE(BossFPartD8_State_HandSlamLift);
STATE(BossFPartD8_State_HandSlamLiftWait);
STATE(BossFPartD8_State_HandSlamHurt);
STATE(BossFPartD8_State_HandSlamHurtWait);
STATE(BossFPartD8_State_DoublePressStart);
STATE(BossFPartD8_State_DoublePressRaise);
STATE(BossFPartD8_State_DoublePressSetTrack);
STATE(BossFPartD8_State_DoublePressTrack);
STATE(BossFPartD8_State_DoublePressStrike);
STATE(BossFPartD8_State_DoublePressImpact);
STATE(BossFPartD8_State_DoublePressStartVulnerable);
STATE(BossFPartD8_State_DoublePressVulnerable);
STATE(BossFPartD8_State_DoublePressEnd);
STATE(BossFPartD8_State_DoublePressEndWait);
STATE(BossFPartD8_State_DoublePressHurt);
STATE(BossFPartD8_State_DoublePressHurtWait);
STATE(BossFPartD8_State_PartDAThrowMoveRight);
STATE(BossFPartD8_State_PartDAThrowMoveToPos);
STATE(BossFPartD8_State_PartDAThrowStart);
STATE(BossFPartD8_State_PartDAThrowStartWait);
STATE(BossFPartD8_State_PartDAThrowSetDelay);
STATE(BossFPartD8_State_PartDAThrowDelay);
STATE(BossFPartD8_State_PartDAThrowWindup);
STATE(BossFPartD8_State_PartDAThrowRelease);
STATE(BossFPartD8_State_PartDAThrowFollowThrough);
STATE(BossFPartD8_State_PartDAThrowFollowThroughWait);
STATE(BossFPartD8_State_PartDAThrowStartVulnerable);
STATE(BossFPartD8_State_PartDAThrowVulnerable);
STATE(BossFPartD8_State_PartDAThrowEnd);
STATE(BossFPartD8_State_PartDAThrowEndWait);
STATE(BossFPartD8_State_PartDAThrowHurt);
STATE(BossFPartD8_State_PartDAThrowHurtWait);
STATE(BossFPartD8_State_HandRideStartWait);
STATE(BossFPartD8_State_HandRideWait);
STATE(BossFPartD8_State_HandRideStartWarn);
STATE(BossFPartD8_State_HandRideWarn);
STATE(BossFPartD8_State_StartHandRideMove);
STATE(BossFPartD8_State_HandRideMove);
STATE(BossFPartD8_State_HandRideStartHold);
STATE(BossFPartD8_State_HandRideHold);
STATE(BossFPartD8_State_HandRideStartReturn);
STATE(BossFPartD8_State_HandRideReturn);
STATE(BossFPartD8_State_HandRideEnd);
STATE(BossFPartD8_State_HandRideEndWait);
STATE(BossFPartD8_State_HandRideRetract);
STATE(BossFPartD8_State_HandRideRetractWait);
STATE(BossFPartD8_State_ReturnHome);
STATE(BossFPartD8_State_PartDCVolleyStart);
STATE(BossFPartD8_State_PartDCVolleyRaise);
STATE(BossFPartD8_State_PartDCVolleyStartDelay);
STATE(BossFPartD8_State_PartDCVolleyDelay);
STATE(BossFPartD8_State_PartDCVolleyFire);
STATE(BossFPartD8_State_PartDCVolleySetInterval);
STATE(BossFPartD8_State_PartDCVolleyInterval);
STATE(BossFPartD8_State_PartDCVolleyEnd);
STATE(BossFPartD8_State_PartDCVolleyEndWait);
STATE(BossFPartD8_State_PartDEAttackStart);
STATE(BossFPartD8_State_PartDEAttackWindup);
STATE(BossFPartD8_State_PartDEAttackStartDelay);
STATE(BossFPartD8_State_PartDEAttackDelay);
STATE(BossFPartD8_State_PartDEAttackSpawn);
STATE(BossFPartD8_State_PartDEAttackWait);
STATE(BossFPartD8_State_PartDEAttackEnd);
STATE(BossFPartD8_State_PartDEAttackEndWait);
STATE(BossFPartD8_State_Hurt);
STATE(BossFPartD8_State_HurtStartRecover);
STATE(BossFPartD8_State_HurtRecover);
STATE(BossFPartD8_State_HurtInSky);
STATE(BossFPartD8_State_SkyChargeStartWait);
STATE(BossFPartD8_State_WaitTimer_ov000_022f0f04);
STATE(BossFPartD8_State_SkyChargeStartFadeOut);
STATE(BossFPartD8_State_SkyChargeFadeOut);
STATE(BossFPartD8_State_LoadSkyModel);
STATE(BossFPartD8_State_WaitTimer_ov000_022f0c8c);
STATE(BossFPartD8_State_SkyChargeStartFadeIn);
STATE(BossFPartD8_State_SkyChargeFadeIn);
STATE(BossFPartD8_State_StartWaitTalkDemo);
STATE(BossFPartD8_State_WaitTalkDemo);
STATE(BossFPartD8_State_SkyChargeStartRetreat);
STATE(BossFPartD8_State_SkyChargeRetreat);
STATE(BossFPartD8_State_SkyChargeStartWait1);
STATE(BossFPartD8_State_WaitTimer_ov000_022f09b4);
STATE(BossFPartD8_State_SkyChargeStart1);
STATE(BossFPartD8_State_SkyCharge1);
STATE(BossFPartD8_State_SkyChargeImpact1);
STATE(BossFPartD8_State_SkyChargeShake1);
STATE(BossFPartD8_State_SkyChargeStartRetreat1);
STATE(BossFPartD8_State_SkyChargeRetreat1);
STATE(BossFPartD8_State_SkyChargeStartWait2);
STATE(BossFPartD8_State_WaitTimer_ov000_022f061c);
STATE(BossFPartD8_State_SkyChargeStart2);
STATE(BossFPartD8_State_SkyCharge2);
STATE(BossFPartD8_State_SkyChargeImpact2);
STATE(BossFPartD8_State_SkyChargeShake2);
STATE(BossFPartD8_State_SkyChargeStartRetreat2);
STATE(BossFPartD8_State_SkyChargeRetreat2);
STATE(BossFPartD8_State_SkyChargeStartWait3);
STATE(BossFPartD8_State_WaitTimer_ov000_022f0280);
STATE(BossFPartD8_State_SkyChargeStart3);
STATE(BossFPartD8_State_SkyCharge3);
STATE(BossFPartD8_State_SkyChargeImpact3);
STATE(BossFPartD8_State_SkyChargeShake3);
STATE(BossFPartD8_State_SkyChargeDestroyBase);
STATE(BossFPartD8_State_SkyChargeAftermath);
STATE(BossFPartD8_State_DefeatCamera1);
STATE(BossFPartD8_State_DefeatCamera2);
STATE(BossFPartD8_State_DefeatCamera3);
STATE(BossFPartD8_State_DefeatFade_ov000_022ef4ec);
STATE(BossFPartD8_State_DefeatFade_ov000_022ef5a4);
STATE(BossFPartD8_State_DefeatFade_ov000_022ef664);
STATE(BossFPartD8_State_DefeatExplode);
STATE(BossFPartD8_State_DefeatFinish);
static void BossFPartD8_Action_ReturnHome(BossFPartD8 *s);
static void BossFPartD8_SetAnimation(BossFPartD8 *p, s32 a, s32 b, s32 c);
static void BossFPartD8_InitHitboxes(BossFPartD8 *s);
static void BossFPartD8_DisableHitboxes(BossFPartD8 *s);
static void BossFPartD8_TriggerPartDAThrow(s32 a);
static void BossFPartD8_TriggerHandRide(void);
static void BossFPartD8_TriggerReturnHome(void);
static void BossFPartD8_ChooseAttack(void);
static void BossFPartD8_Draw_RidingPlayer(void);
static void BossF_State_SpawnLaserFinEffects(void *p_);

/* ============================================================ BossF */
/* BossF_InitCamera (modified: POWCNT display swap dropped, single screen) */
static void BossF_InitCamera(void) {
    /* *(vu16 *)0x4000304 |= 0x8000: which LCD shows the main engine - not used (one 640x480 view) */
    CAM.unkDC = 2;
    CAM.unk4 = 0xaaa;
    CAM.unkC = 0x1a66000;
    CAM.unk14 = FX_Div(0x1000, 0x34cc);
    CAM.t[0].ox = CAM.t[0].oy = CAM.t[0].oz = NULL;          /* cam->unk0..8 */
    CAM.t[0].offx = CAM.t[0].offy = CAM.t[0].offz = 0;       /* cam->unkC..14 */
    CAM.t[0].lerp = 0x200;                                   /* cam->unk30 */
    CAM.t[0].accel = 0x2aa;                                  /* cam->unk38 */
    CAM.t[1].ox = gPlayers;                                  /* cam->unk3C */
    CAM.t[1].oy = NULL;                                      /* cam->unk40 */
    CAM.t[1].oz = gPlayers;                                  /* cam->unk44 */
    CAM.t[1].tgt.y = -0x2c0000;                              /* cam->unk58 */
    CAM.t[1].offx = 0;                                       /* cam->unk48 */
    CAM.t[1].offy = 0x28000;                                 /* cam->unk4C */
    CAM.t[1].offz = 0;                                       /* cam->unk50 */
    CAM.t[1].lerp = 0x400;                                   /* cam->unk6C */
    CAM.t[1].accel = 0x2aa;                                  /* cam->unk74 */
    CAM.unkC0 = CAM.unkC4 = 0x1c2000;                        /* cam->unk78 / unk7C */
    CAM.unkC8 = 0x200;                                       /* cam->unk80 */
    CAM.unkCC = CAM.unkD0 = -0x28000;                        /* cam->unk84 / unk88 */
    CAM.unkD4 = 0x80;                                        /* cam->unk8C */
    CAM.unkD6 = CAM.unkD8 = 0;                               /* cam->unk8E / unk90 */
    CAM.unkDA = 0x100;                                       /* cam->unk92 */
    gPlayers->map_up = 0x40;                                 /* gPlayers->unk430 */
    gPlayers->map_down = 0x80;                               /* gPlayers->unk432 */
}

/* BossF_UpdateCameraShake (copied) */
static void BossF_UpdateCameraShake(u16 *p, s32 a, s32 sh) {
    CAM.unk3C.x = FX_Mul(a * (ScreenShake_GetOffsetX() << 12), (*p << 12) >> sh);
    CAM.unk3C.y = FX_Mul(a * (ScreenShake_GetOffsetY() << 12), (*p << 12) >> sh);
    if (*p != 0)
        (*p)--;
}

/* BossF_SpawnLaserFinEffect (modified: the .nsbma of eff_laser_fin_b runs as a timing-only animation,
 * so the effect lives as long as on the DS) */
static void BossF_SpawnLaserFinEffect(void) {
    BossEffect *o;
    s32 x;
    s32 y;
    s32 z;
    x = Rand() & 0x1ff;
    x = (x + (Rand() & 0xff)) << 8;
    y = ((Rand() & 0x1f) << 10) + 0x30000;
    z = ((Rand() & 0xf) << 10) + 0x3000;
    o = BossEffect_CreateLaserFinB(1, x, y, z);
    Model_SetAnimation(&o->unk134, 1, &s_lfb_ma, 0);
    o->h.vel_y = -0x1000;  /* base.unk30 */
    o->unk134.speed = 0x1599; /* unk248 */
}

/* BossF_State_SpawnLaserFinEffects (copied) */
static void BossF_State_SpawnLaserFinEffects(void *p_) {
    BossF *p = (BossF *)p_;
    s32 t;
    p->unk210++;
    t = p->unk210 & 0xf;
    if (t <= 8) {
        if (t >= 8)
            goto case8;
        if (t > 4)
            return;
        if (t < 0)
            return;
        if (t == 0)
            goto case0;
        if (t == 4)
            goto case4;
        return;
    }
    if (t == 12)
        goto case4;
    return;
case0:
    if (p->unk20C & 2)
        BossF_SpawnLaserFinEffect();
    return;
case8:
    if (p->unk20C & 4)
        BossF_SpawnLaserFinEffect();
    return;
case4:
    if (p->unk20C & 8)
        BossF_SpawnLaserFinEffect();
}

/* BossF_Action_StartLaserFinEffects (copied) */
static void BossF_Action_StartLaserFinEffects(BossF *s) { s->func = BossF_State_SpawnLaserFinEffects; }

/* BossF_PartD8StartWaitNextAttack (copied) */
static void BossF_PartD8StartWaitNextAttack(BossFPartD8 *s) {
    BossFPartD8_SetAnimation(s, 6, 0x1000, 1);
    BossFPartD8_InitHitboxes(s);
    BossFPartD8_DisableHitboxes(s);
    s->h.disp_flags |= 1;
    BossF_InitCamera();
    gStageMapCollision_unk1C = 0;
    gStageMapCollision_unk24 = 0x340;
    s->unk434 = (Rand() & 0x3f) + 0x1e;
    gBossStatus.unk4 = 0x2f;
    s->func = BossFPartD8_State_WaitNextAttack;
    boss_log("idle", s->unk434, gBossStatus.unk8);
}

/* BossF_StartNextAttack (copied) */
static void BossF_StartNextAttack(void) { BossF_PartD8StartWaitNextAttack(gBossF->unk218); }

/* BossF_State_WaitForStart (modified: sky model from BossStageF, gPlayers->unk70 = draw_player hook) */
static void BossF_State_WaitForStart(void *p_) {
    BossF *s = (BossF *)p_;
    BossModel *a;
    BossFPartD8 *p1;
    if (!(gBossStatus.unk0 & 1))
        return;
    a = &CAM.unkE0; /* data_ov000_0236a524 */
    p1 = s->unk218;
    s->h.disp_flags &= ~0x20;
    p1->h.disp_flags &= ~0x20;
    CAM.unk0 |= 1;
    Model_Init(a, 0);
    Model_SetResource(a, s->unk238);
    Model_SetAnimation(a, 0, s->unk224, 0);
    Model_SetAnimation(a, 1, s->unk230, 0);
    Model_SetAnimation(a, 3, s->unk22C, 0);
    a->speed = 0x800; /* unk114 */
    a->scale[0] = a->scale[1] = a->scale[2] = 0x1355b50;
    BossF_InitCamera();
    CAM.t[0].cur.x = 0x180000; /* c->unk24 */
    CAM.t[0].cur.y = -0x2e8000;
    CAM.t[0].cur.z = 0x1c2000;
    CAM.t[0].tgt = CAM.t[0].cur; /* c->unk18 */
    CAM.t[1].cur.x = 0x180000; /* c->unk60 */
    CAM.t[1].cur.y = -0x2c0000;
    CAM.t[1].cur.z = 0;
    CAM.t[1].tgt = CAM.t[1].cur; /* c->unk54 */
    s->unk21C = g_boss_game.draw_player; /* gPlayers->unk70 */
    NaviMsg_SetFaceAnim(9);
    NaviMsg_ShowMessage(0xf, 0xf0);
    ScreenShake_Start(0xc);
    BossF_StartNextAttack();
    BossF_Action_StartLaserFinEffects(gBossF);
    boss_log("start", gBossStatus.unk8, 0);
}

/* BossF_Draw (modified: the base's visibility animation (bsf.nsbva, slot 4) is applied at the draw) */
static void BossF_Draw(void *p_) {
    BossF *s = (BossF *)p_;
    BossModel *a = &s->unk244;
    if (s->h.disp_flags & 0x20)
        return;
    Model_Animate(a);
    if (a->anm[4] != NULL && a->anm[4]->res != NULL && a->anm[4]->res->loaded && a->res != NULL && a->res->loaded) {
        dca_visibility(&a->anm[4]->res->set, a->anm[4]->idx, (float)(a->anm[4]->frame >> 12), &a->res->mdl, s_base_vis);
        Model_Draw_inst(a, s_base_vis, 255);
    } else
        Model_Draw(a);
}

/* BossF_Destructor (modified: resources are released by bossf_unload; BLDCNT state) */
static void BossF_Destructor(void *p_) {
    BossF *w = (BossF *)p_;
    s32 i;
    /* NNS_G3dResDefaultRelease("exc:eff_*.nsbmd"): boss_effects_shutdown */
    w->unk238 = NULL;
    w->unk23C = NULL;
    w->unk240 = NULL;
    boss_model_release(&w->unk244);
    boss_model_release(&w->unk85C);
    for (i = 0; i < 4; i++)
        boss_model_release(&w->unk37C[i]);
    s_bld_mode = 0; /* gDisplayMain.unk20.b6 = 0 */
    ScreenShake_Start(0);
    boxes_forget(&w->unk134, 3);
    gBossF = NULL;
}

/* StageZoneScript_ZoneFBoss (copied: the screen shake moves the boss camera while gBossF->unk20C & 0x10) */
static void StageZoneScript_ZoneFBoss(void) {
    BossF *w = gBossF;
    if (w == NULL)
        return;
    if (w->unk20C & 0x10) {
        if (w->unk20C & 0x20)
            VEC_Subtract(&CAM.unk3C, &w->unk9C4, &CAM.unk3C); /* data_ov000_0236a480 */
        w->unk20C |= 0x20;
        w->unk9C4.x = ScreenShake_GetOffsetX() << 10;
        w->unk9C4.y = ScreenShake_GetOffsetY() << 10;
        VEC_Add(&CAM.unk3C, &w->unk9C4, &CAM.unk3C);
        return;
    }
    if (w->unk20C & 0x20) {
        VEC_Subtract(&CAM.unk3C, &w->unk9C4, &CAM.unk3C);
        w->unk20C &= ~0x20;
    }
}

/* ZoneFx_ZoneFBoss_Init (copied) */
static void ZoneFx_ZoneFBoss_Init(s32 *p) { *p = 0; }

/* ZoneFx_ZoneFBoss_Update (copied: the passage behind the arena: satellites, peels, clouds after
 * 0xb40 / 0x1680 frames of stage time, dust always; models drawn twice keep both matrices) */
static void ZoneFx_ZoneFBoss_Update(void) {
    BossF *w = gBossF;
    BossModel *o;
    if (w == NULL)
        return;
    if (!(gBossStatus.unk0 & 1))
        return;
    if (w->unk238 != NULL) {
        if (g_dcp_world.frame > 0x1680 && w->unk998 != 0) {
            w->unk9AC += 0x64000;
            w->unk9B0 += 0x64000;
            if (w->unk9AC >= 0x1088000) {
                w->unk9AC = w->unk9B0;
                w->unk9B0 = w->unk9AC - 0x1388000;
                w->unk9B4 = w->unk9B8;
                if (g_dcp_world.frame > 0x1680)
                    w->unk9B8 = 1;
            }
            o = w->unk9B4 ? &w->unk37C[3] : &w->unk37C[2];
            o->trans[0] = 0x1a0000;
            o->trans[2] = 0;
            o->trans[1] = w->unk9AC;
            Model_Draw(o);
            o = w->unk9B8 ? &w->unk37C[3] : &w->unk37C[2];
            o->trans[0] = 0x1a0000;
            o->trans[2] = 0;
            o->trans[1] = w->unk9B0;
            Model_Draw(o);
        } else if (g_dcp_world.frame > 0xb40 && w->unk994 != 0) {
            w->unk9A4 += 0xc000;
            w->unk9A8 += 0xc000;
            o = &w->unk37C[1];
            w->unk20C |= 0x10;
            if (w->unk9A4 >= 0x1088000) {
                w->unk9A4 = w->unk9A8;
                if (g_dcp_world.frame > 0x1680) {
                    if (w->unk9A4 >= 0x1088000)
                        w->unk998 = 1;
                } else {
                    w->unk9A8 = w->unk9A4 - 0x1388000;
                }
            }
            o->trans[0] = 0x1a0000;
            o->trans[2] = 0;
            o->trans[1] = w->unk9A4;
            Model_Draw(o);
            o->trans[1] = w->unk9A8;
            Model_Draw(o);
        } else {
            o = &w->unk37C[0];
            o->trans[1] += 0x1800;
            if (o->trans[1] >= 0xc8000) {
                if (g_dcp_world.frame > 0xb40) {
                    w->unk994 = 1;
                } else {
                    if (w->unk9A0 != 0) {
                        w->unk9A0 = 0;
                        o->trans[0] = 0xc8000;
                        o->trans[2] = -0x3e8000;
                    } else {
                        w->unk9A0 = 1;
                        o->trans[0] = 0x258000;
                        o->trans[2] = -0x708000;
                    }
                    o->trans[1] = -0x3c8000;
                }
            }
            Model_Draw(o);
        }
    }
    w->unk9BC += 0x10000;
    w->unk9C0 += 0x10000;
    o = &w->unk85C;
    if (w->unk9BC >= 0x1088000) {
        w->unk9BC = w->unk9C0;
        w->unk9C0 = w->unk9BC - 0x1388000;
    }
    o->trans[0] = 0x1a0000;
    o->trans[2] = 0;
    o->trans[1] = w->unk9BC;
    Model_Draw(o);
    o->trans[1] = w->unk9C0;
    Model_Draw(o);
}

/* ============================================================ BossFPartD8: helpers */
/* gPlayers->unk70 (the player's draw callback): NULL = the stage's default 3D draw */
static void set_player_draw(BossDrawPlayerFn f) {
    g_boss_game.draw_player = f;
    if (f == NULL)
        g_boss_game.player_trans_valid = 0;
}

/* BossFPartD8_SetAnimation (copied) */
static void BossFPartD8_SetAnimation(BossFPartD8 *p, s32 a, s32 b, s32 c) {
    Model_SetAnimation(&p->unk470, 0, p->unk46C, (u32)a);
    p->unk584 = b;
    if (c)
        p->unk57C |= 4;
    else
        p->unk57C &= ~4;
}

/* BossFPartD8_InitHitboxes (copied) */
static void BossFPartD8_InitHitboxes(BossFPartD8 *s) {
    ObjBox *c;
    rect_set_box3d(&s->coll[0], -0x24, -0x28, 0x24, 8, -0x7d00, 0x7d00);
    rect_set_box3d(&s->coll[1], -0x3c, -0x30, 0x24, 0x38, -0x7d00, 0x7d00);
    rect_set_box3d(&s->coll[2], -0x78, -0x30, 0x10, 0x38, -0x7d00, 0x7d00);
    c = &s->coll[3];
    rect_set_box3d(c, -0x3c, -0x30, 0x24, 0x38, -0x7d00, 0x7d00);
    c = &s->coll[4];
    rect_set_box3d(c, -0x28, -0x30, 0x38, 0x38, -0x7d00, 0x7d00);
    c = &s->coll[5];
    rect_set_box3d(c, -0x14, -0x30, 0x74, 0x38, -0x7d00, 0x7d00);
    c = &s->coll[6];
    rect_set_box3d(c, -0x28, -0x30, 0x38, 0x38, -0x7d00, 0x7d00);
}

/* BossFPartD8_DisableHitboxes (copied) */
static void BossFPartD8_DisableHitboxes(BossFPartD8 *s) {
    s32 i;
    for (i = 0; i < 7; i++)
        s->coll[i].flags &= ~4; /* unk216 */
}

/* BossFPartD8_MoveTowardX (copied) */
static void BossFPartD8_MoveTowardX(BossFPartD8 *w, s32 target, s32 range, s32 max, s16 accel, s32 decel, s16 lim) {
    s32 d;
    s32 v = w->h.vel_x;
    s32 x = w->h.x;
    d = target - x;
    if (max > range)
        range = max;
    if (v == 0) {
        if (w->unk45C == 0) {
            if (d < 0)
                d = -d;
            if (range < d) {
                if (x < target)
                    w->h.vel_x = (s16)max;
                else
                    w->h.vel_x = (s16)-max;
            }
        }
    } else {
        if (!((x < target && v < 0) || (x > target && v > 0))) {
            if (d < 0)
                d = -d;
            if (range < d)
                goto accelerate;
        }
        if (v < 0) {
            if (decel < -v)
                w->h.vel_x += (s16)decel;
            else
                w->h.vel_x = 0;
        } else if (v > 0) {
            if (decel < v)
                w->h.vel_x -= (s16)decel;
            else
                w->h.vel_x = 0;
        }
        if (w->h.vel_x == 0)
            w->unk45C = (Rand() & 0x1f) + 0x1e;
        goto end;
    accelerate:
        if (v > 0)
            w->h.vel_x += accel;
        else
            w->h.vel_x -= accel;
        w->h.vel_x = w->h.vel_x < -lim ? -lim : (w->h.vel_x > lim ? lim : w->h.vel_x);
    }
end:
    if (w->h.x < 0x8000) {
        w->h.x = 0x8000;
        w->h.vel_x = 0;
    }
    if (w->h.x > 0x2c000) {
        w->h.x = 0x2c000;
        w->h.vel_x = 0;
    }
    if (w->unk45C != 0)
        w->unk45C--;
}

/* BossFPartD8_TrackPlayerX_* (copied) */
static void BossFPartD8_TrackPlayerX_Far(BossFPartD8 *p0) { BossFPartD8_MoveTowardX(p0, gPlayers->x, 0x4000, 0x200, 0x60, 0x80, 0x700); }
static void BossFPartD8_TrackPlayerX_Fast(BossFPartD8 *p0) { BossFPartD8_MoveTowardX(p0, gPlayers->x, 0x4000, 0x300, 0x80, 0x40, 0x600); }
static void BossFPartD8_TrackPlayerX_Near(BossFPartD8 *p0) { BossFPartD8_MoveTowardX(p0, gPlayers->x, 0x2000, 0x200, 0x60, 0x80, 0x700); }
static void BossFPartD8_TrackPlayerX_NearSlow(BossFPartD8 *p0) { BossFPartD8_MoveTowardX(p0, gPlayers->x, 0x2000, 0x100, 0x20, 0x40, 0x700); }
static void BossFPartD8_TrackPlayerX_Slow(BossFPartD8 *p0) { BossFPartD8_MoveTowardX(p0, gPlayers->x, 0x4000, 0x100, 0x30, 0x40, 0x700); }
static void BossFPartD8_TrackPlayerX_Offset(BossFPartD8 *p, s32 b) {
    s32 x;
    DcPlayer *plr = gPlayers;
    if (b)
        x = plr->x - 0x6400;
    else
        x = plr->x + 0x6400;
    BossFPartD8_MoveTowardX(p, x, 0x4000, 0x300, 0x80, 0x40, 0x600);
}

/* BossFPartD8_IsPlayerCrouching (copied) */
static BOOL BossFPartD8_IsPlayerCrouching(void) { return (u16)(s16)(gPlayers->anim - 3) <= 1; }

/* BossFPartD8_IsPlayerRiding (modified: the ride pointer is the collision map owner's DcpObjRef) */
static s32 BossFPartD8_IsPlayerRiding(void) {
    if (gPlayers->move_flags & 1) {
        if (gPlayers->ride == &gBossF->unk218->cm.ref)
            return 1;
    }
    return 0;
}

/* BossFPartD8_Unk_ov000_022f89a8 (copied: the right map edge follows the player back to 0x340) */
static BOOL BossFPartD8_Unk_ov000_022f89a8(void) {
    s32 v;
    v = (gPlayers->x >> 8) + 0xc;
    if (v < gStageMapCollision_unk24)
        gStageMapCollision_unk24 = v;
    if (gStageMapCollision_unk24 > 0x340)
        return FALSE;
    gStageMapCollision_unk24 = 0x340;
    return TRUE;
}

/* BossFPartD8_Unk_ov000_022f8f78 (copied) */
static BOOL BossFPartD8_Unk_ov000_022f8f78(void) { return gPlayers->x > 0x2fa00; }

/* BossFPartD8_GetRidePosition (copied: the point of the hand at radius unk43C from the right shoulder) */
static void BossFPartD8_GetRidePosition(BossFPartD8 *s, s32 *x, s32 *y, u8 *z) {
    u16 a;
    u16 ang;
    u16 b;
    s32 px;
    s32 py;
    a = (u16)(FX_Atan2(s->unk3C8 - s->unk428, s->unk3C4 - s->unk424) + 0x8000);
    ang = (u16)(a - (s->unk438 - s->unk43A));
    px = s->unk3C4 + FX_Mul(COS(ang), s->unk43Cw);
    py = s->unk3C8 + FX_Mul(SIN(ang), s->unk43Cw);
    b = (u16)(s->unk43A - ang);
    if (x != NULL)
        *x = px >> 4;
    if (y != NULL)
        *y = -py >> 4;
    if (z != NULL)
        *z = (u8)(b >> 8);
}

/* BossFPartD8_Draw_RidingPlayer (modified: StageObj_Draw3DEx(x, y, z, rot) -> the player model
 * transform for boss_player_model(); the integrator applies the stage model matrix, RotY(0x4000) and
 * scale, before it) */
static void BossFPartD8_Draw_RidingPlayer(void) {
    DcPlayer *p = gPlayers;
    s32 x, y;
    u8 z;
    MtxFx33 ry, rz;
    u16 face;
    BossFPartD8_GetRidePosition(gBossF->unk218, &x, &y, &z);
    g_boss_game.player_trans[0] = x << 4;
    g_boss_game.player_trans[1] = -(y << 4);
    g_boss_game.player_trans[2] = p->z << 4;
    face = (u16)((p->disp_flags & 1) ? 0xc000 - 0x4000 : 0);
    MTX_RotY33(&ry, SIN(face), COS(face));
    MTX_RotZ33(&rz, SIN((u16)-(z << 8)), COS((u16)-(z << 8)));
    MTX_Concat33(&ry, &rz, &g_boss_game.player_rot);
    g_boss_game.player_trans_valid = 1;
}

/* BossFPartD8_SetCameraFollowPlayer (copied) */
static void BossFPartD8_SetCameraFollowPlayer(void) {
    CAM.t[1].ox = gPlayers;     /* g->unk3C */
    CAM.t[1].oy = gPlayers;     /* g->unk40 */
    CAM.t[1].offx = 0x14000;    /* g->unk48 */
    CAM.t[1].offy = 0x32000;    /* g->unk4C */
    CAM.t[1].offz = 0x46000;    /* g->unk50 */
    CAM.t[1].lerp = 0x400;      /* g->unk6C */
    CAM.t[1].accel = 0x2aa;     /* g->unk74 */
    CAM.t[0].lerp = 0x400;      /* g->unk30 */
    CAM.t[0].accel = 0x2aa;     /* g->unk38 */
    CAM.unkC0 = 0x12c000;       /* g->unk78 */
    CAM.unkCC = 0x28000;        /* g->unk84 */
    CAM.unkD6 = 0xd555;         /* g->unk8E */
}

/* BossFPartD8_CheckKnockOffRider (copied) */
static BOOL BossFPartD8_CheckKnockOffRider(BossFPartD8 *s) {
    DcPlayer *p = gPlayers;
    s32 x;
    s32 y;
    switch (p->anim) {
    case 0xf:
    case 0x11:
    case 0x12:
        set_player_draw(gBossF->unk21C); /* p->unk70 */
        gPlayers->disp_flags |= 0x40;
        return FALSE;
    }
    if (!BossFPartD8_IsPlayerCrouching()) {
        if (BossFPartD8_IsPlayerRiding() || gPlayers->x > 0x2c000) {
            BossFPartD8_GetRidePosition(s, &x, &y, 0);
            gPlayers->x = x;
            gPlayers->y = y;
            Player_Action_Knockback(gPlayers, (0x1a000 - gPlayers->x) >> 7, -0x800);
            BossFPartD8_GetRidePosition(s, &gPlayers->x, &gPlayers->y, 0);
            set_player_draw(gBossF->unk21C);
            gPlayers->disp_flags |= 0x40;
            s->unk1B8 = NULL;
            BossFPartD8_SetCameraFollowPlayer();
            s_st.knock_offs++;
            boss_log("knock_off", gPlayers->x >> 8, gPlayers->y >> 8);
            return TRUE;
        }
    }
    return FALSE;
}

/* BossFPartD8_BlinkPalette (copied) */
static void BossFPartD8_BlinkPalette(BossFPartD8 *p0) {
    p0->unk440 = (u16)(p0->unk440 + 1);
    if ((p0->unk440 & 3) != 0)
        return;
    if ((p0->unk440 & 0x8000) != 0)
        BossF_LoadPalette(gGameState, 0);
    else
        BossF_LoadPalette(gGameState, 1);
    p0->unk440 &= ~3;
    p0->unk440 ^= 0x8000;
}

/* BossFPartD8_StepFlashFade (modified: BLDY state) */
static void BossFPartD8_StepFlashFade(BossFPartD8 *w, u16 n) {
    if (n >= ++w->unk452)
        return;
    w->unk452 = 0;
    if (s_bld_evy == 0)
        return;
    bld_evy_add(-1); /* data_02089250.b0 -= 1 */
    if (s_bld_evy == 0)
        s_bld_mode = 0; /* gDisplayMain.unk20 &= ~0xc0 */
}

/* BossFPartD8_CreateHitEffect (copied) */
static void BossFPartD8_CreateHitEffect(s32 a, s32 b, s32 c) {
    MtxFx33 mtx;
    BossEffect *obj = BossEffect_CreateHit(1, a, b, c);
    u16 ang = (u16)((Rand() & 0xfff) + 0xaaa);
    MTX_RotX33(&obj->unk134.rot, SIN(ang), COS(ang)); /* obj + 0x158 */
    ang = Rand();
    MTX_RotY33(&mtx, SIN(ang), COS(ang));
    MTX_Concat33(&obj->unk134.rot, &mtx, &obj->unk134.rot);
}

/* BossFPartD8_RollThrowVariant (copied) */
static u8 BossFPartD8_RollThrowVariant(void) {
    u8 ret = 0;
    u16 hp = gBossStatus.unk8;
    if (hp >= 7)
        ret = data_ov000_02363e20[GetLevel()][Rand() & 3];
    else if (hp >= 6)
        ret = data_ov000_02363e00[GetLevel()][Rand() & 3];
    else if (hp >= 4)
        ret = data_ov000_02363e40[GetLevel()][Rand() & 3];
    else if (hp >= 2)
        ret = data_ov000_02363e08[GetLevel()][Rand() & 3];
    return ret;
}

/* BossFPartD8_InitPartDCPath (copied) */
static void BossFPartD8_InitPartDCPath(BossFPartDC *p, s32 a, s32 b, s16 c) {
    p->unk220 = a;
    p->unk224 = 0x30000;
    p->unk228 = 0;
    p->unk22C = b;
    p->unk230 = 0x30000;
    p->unk234 = 0;
    if (p->unk20C & 2) {
        p->unk218 = c;
        p->h.vel_x = 0;
        return;
    }
    if (a < b)
        p->h.vel_x = c;
    else
        p->h.vel_x = (s16)-c;
    p->unk214 = p->h.vel_x;
}

/* BossFPartD8_SpawnPartD9 (copied) */
static void BossFPartD8_SpawnPartD9(void) {
    MapObject_Spawn(0xd9, 0x52600, 0x1a000, 1);
    Sound_PlaySeqArc(0, 0, 0xc7);
}

/* ============================================================ BossFPartD8: actions */
/* BossFPartD8_Action_Defeat (modified: Task_DestroyByTag(4) = the parts with tag 4 (none of the
 * translated objects use it); NNS_SndPlayerStopSeqAll -> music_stop) */
static void BossFPartD8_Action_Defeat(BossFPartD8 *s) {
    BossF *b = gBossF;
    gBossStatus.unk4 = 0x3b;
    BossFPartD8_InitHitboxes(s);
    BossFPartD8_DisableHitboxes(s);
    s->unk1B8 = NULL;
    b->h.obj_flags |= 0x20;
    s->h.obj_flags |= 0x20;
    b->unk244.speed = 0; /* b->unk358 */
    s->unk584 = 0;
    StageObj_SetPause();
    BossF_LoadPalette(gGameState, 0);
    g_dcp_world.flags &= ~1u; /* gGameState.unk18: the stage timer stops */
    Player_Action_Blank(gPlayers);
    Player_SetAnimation(gPlayers, 0x30);
    gPlayers->invuln = 0;
    gPlayers->x = 0x19e00;
    gPlayers->y = 0x2b100;
    gPlayers->z = 0x2000;
    s->h.z += 0x1400;
    RingManager_SetFlags1();
    gBossF->unk20C &= ~0xe;
    /* Task_DestroyByTag(4) */
    s_bld_mode = 0; /* gDisplayMain.unk20.bits.b6 = 0 */
    b->unk20C &= ~0x10;
    NNS_SndPlayerStopSeqAll();
    s->func = BossFPartD8_State_DefeatCamera1;
    boss_log("defeat", 0, 0);
}

/* BossFPartD8_Action_DoublePressHurt / HandSlamHurt / PartDAThrowHurt (copied) */
static void BossFPartD8_Action_DoublePressHurt(BossFPartD8 *p0) {
    p0->h.vel_x = 0;
    BossFPartD8_DisableHitboxes(p0);
    BossFPartD8_LoadModelPalettesB(gGameState, 0);
    BossFPartD8_LoadModelPalettesA(gGameState, 0);
    gBossStatus.unk4 = 0x39;
    p0->func = BossFPartD8_State_DoublePressHurt;
}
static void BossFPartD8_Action_HandSlamHurt(BossFPartD8 *p0) {
    p0->h.vel_x = 0;
    BossFPartD8_DisableHitboxes(p0);
    BossFPartD8_LoadModelPalettesB(gGameState, 0);
    BossFPartD8_LoadModelPalettesA(gGameState, 0);
    gBossStatus.unk4 = 0x39;
    p0->func = BossFPartD8_State_HandSlamHurt;
}
static void BossFPartD8_Action_PartDAThrowHurt(BossFPartD8 *p0) {
    BossFPartD8_DisableHitboxes(p0);
    BossFPartD8_LoadModelPalettesA(gGameState, 0);
    gBossStatus.unk4 = 0x39;
    p0->func = BossFPartD8_State_PartDAThrowHurt;
}

/* BossFPartD8_Action_Hurt (modified: gPlayers->unk70 -> draw_player hook) */
static void BossFPartD8_Action_Hurt(BossFPartD8 *s) {
    BossF *b = gBossF;
    DcPlayer *p;
    BossFPartD8_SetAnimation(s, 0x23, 0x1000, 0);
    BossFPartD8_InitHitboxes(s);
    BossFPartD8_DisableHitboxes(s);
    s->unk1B8 = NULL;
    CAM.t[1].oy = NULL;          /* c->unk40 */
    CAM.t[1].tgt.y -= 0x64000;   /* c->unk58 */
    CAM.t[1].offz = 0;           /* c->unk50 */
    CAM.unkCC = 0x28000;         /* c->unk84 */
    CAM.unkC0 = 0x258000;        /* c->unk78 */
    p = gPlayers;
    if (!(p->pf & 0x400)) {
        set_player_draw(gBossF->unk21C); /* p->base.unk70 */
        Player_Action_Blank(gPlayers);
        Player_SetAnimation(gPlayers, 2);
        gPlayers->gnd_vel = 0;
        gPlayers->vel_x = 0;
        gPlayers->vel_y = 0;
        gPlayers->move_flags |= 0x10;
        gPlayers->move_flags &= ~0x80;
    }
    s->unk434 = 0;
    BossHelp_Destroy_F();
    b->unk20C &= ~0x40;
    gBossStatus.unk4 = 0x3a;
    s->func = BossFPartD8_State_Hurt;
}

/* BossFPartD8_Action_HurtInSky (copied) */
static void BossFPartD8_Action_HurtInSky(BossFPartD8 *p0) {
    BossFPartD8_SetAnimation(p0, 0x24, 0x1000, 0);
    BossFPartD8_InitHitboxes(p0);
    BossFPartD8_DisableHitboxes(p0);
    p0->unk1B8 = NULL;
    BossF_InitCamera();
    p0->unk434 = 0;
    gBossStatus.unk4 = 0x3a;
    p0->func = BossFPartD8_State_HurtInSky;
}

/* BossFPartD8_Action_ReturnHome (copied) */
static void BossFPartD8_Action_ReturnHome(BossFPartD8 *s) {
    BossF_InitCamera();
    s->func = BossFPartD8_State_ReturnHome;
}

/* BossFPartD8_Action_StartDoublePress (copied) */
static void BossFPartD8_Action_StartDoublePress(BossFPartD8 *p0) {
    p0->h.vel_x = 0;
    p0->unk436 = 0;
    BossFPartD8_InitHitboxes(p0);
    rect_set_box3d(&p0->coll[3], -120, -48, 0x10, 0x38, -0x7d00, 0x7d00); /* p0 + 0x290 */
    rect_set_box3d(&p0->coll[6], -20, -48, 0x74, 0x38, -0x7d00, 0x7d00);  /* p0 + 0x314 */
    BossFPartD8_DisableHitboxes(p0);
    gBossStatus.unk4 = 0x31;
    p0->func = BossFPartD8_State_DoublePressStart;
    s_st.presses++;
    g_boss_stats.attacks++;
    boss_log("attack_press", p0->unk458, gBossStatus.unk8);
}

/* BossFPartD8_Action_StartHandRide (modified: EffEnemyRoomFix_Create -> help hook) */
static void BossFPartD8_Action_StartHandRide(BossFPartD8 *s) {
    ObjBox *c;
    s->unk1B8 = s;
    BossFPartD8_DisableHitboxes(s);
    c = &s->coll[0]; /* coll20C */
    c->flags |= 4;
    rect_set_box3d(c, -0x60, -0x100, 0, 0, -0x7d00, 0x7d00);
    s->unk434 = 0;
    s->unk436 = 0;
    gStageMapCollision_unk1C = 0;
    gStageMapCollision_unk24 = gStageMapCollision_unk18 << 6;
    EffEnemyRoomFix_Create(0xb4, 0x30, 0, 0xb4);
    s->unk440 = 0;
    gBossStatus.unk4 = 0x33;
    s->func = BossFPartD8_State_HandRideStartWait;
    s_st.rides++;
    boss_log("attack_ride", 0, gBossStatus.unk8);
}

/* BossFPartD8_Action_StartHandSlam (copied) */
static void BossFPartD8_Action_StartHandSlam(BossFPartD8 *s) {
    s->h.vel_x = 0;
    BossFPartD8_InitHitboxes(s);
    BossFPartD8_DisableHitboxes(s);
    s->unk434w = (Rand() & 1) ? 1 : 0;
    s->unk438 = 0;
    s->unk43A = 0;
    Sound_PlayVoice(0, 0, 0xd0);
    gBossStatus.unk4 = 0x30;
    s->func = BossFPartD8_State_HandSlamRaise;
    s_st.hand_slams++;
    g_boss_stats.attacks++;
    boss_log("attack_slam", s->unk458, gBossStatus.unk8);
}

/* BossFPartD8_Action_StartPartDAThrow (copied) */
static void BossFPartD8_Action_StartPartDAThrow(BossFPartD8 *s) {
    s->h.vel_x = 0;
    s->unk434 = 0;
    s->unk436 = 0;
    s->unk438 = 0;
    BossFPartD8_DisableHitboxes(s);
    s->h.disp_flags &= ~1;
    CAM.t[1].offx = 0x64000;  /* cam->unk48 */
    CAM.t[1].offy = 0x50000;  /* cam->unk4C */
    CAM.t[1].offz = 0x50000;  /* cam->unk50 */
    CAM.unkC0 = 0x1f4000;     /* cam->unk78 */
    CAM.unkCC = -0x28000;     /* cam->unk84 */
    CAM.unkD6 = 0xd555;       /* cam->unk8E */
    gBossStatus.unk4 = 0x32;
    s->func = BossFPartD8_State_PartDAThrowMoveRight;
    s_st.throws++;
    boss_log("attack_throw", s->unk458, gBossStatus.unk8);
}

/* BossFPartD8_Action_StartPartDCVolley (copied) */
static void BossFPartD8_Action_StartPartDCVolley(BossFPartD8 *p) {
    p->unk43Cw = (Rand() & 1) ? 1 : 0;
    p->unk440 = 0;
    p->unk434w = 0;
    gBossStatus.unk4 = 0x36;
    p->func = BossFPartD8_State_PartDCVolleyStart;
    s_st.lasers++;
    g_boss_stats.attacks++;
    boss_log("attack_laser", p->unk458, gBossStatus.unk8);
}

/* BossFPartD8_Action_StartPartDEAttack (copied) */
static void BossFPartD8_Action_StartPartDEAttack(BossFPartD8 *s) {
    s->unk438 = 0;
    s->unk434w = 0;
    gBossStatus.unk4 = 0x37;
    s->func = BossFPartD8_State_PartDEAttackStart;
    s_st.beams++;
    g_boss_stats.attacks++;
    boss_log("attack_beam", s->unk458, gBossStatus.unk8);
}

/* BossFPartD8_Action_StartSkyCharge (copied) */
static void BossFPartD8_Action_StartSkyCharge(BossFPartD8 *s) {
    BossF *p = gBossF;
    BOOL b;
    s32 i;
    s->h.vel_x = 0;
    b = TRUE;
    if (g_dcp_world.mode != 2 && g_dcp_world.mode != 3)
        b = FALSE;
    if (b)
        i = 1;
    else
        i = g_boss_game.unk14;
    s->unk44E = data_ov000_02363dd8[i];
    g_dcp_world.flags &= ~1u; /* gGameState.unk18 */
    p->unk20C |= 0x10;
    gBossStatus.unk4 = 0x38;
    s->func = BossFPartD8_State_SkyChargeStartWait;
    s_st.sky_charges++;
    g_boss_stats.attacks++;
    boss_log("attack_sky", 0, gBossStatus.unk8);
}

/* ============================================================ BossFPartD8: triggers */
static void BossFPartD8_TriggerDefeat(void) { BossFPartD8_Action_Defeat(gBossF->unk218); }
static void BossFPartD8_TriggerDoublePress(s32 a) {
    gBossF->unk218->unk458 = a;
    BossFPartD8_Action_StartDoublePress(gBossF->unk218);
}
static void BossFPartD8_TriggerDoublePressHurt(void) { BossFPartD8_Action_DoublePressHurt(gBossF->unk218); }
static void BossFPartD8_TriggerHandRide(void) { BossFPartD8_Action_StartHandRide(gBossF->unk218); }
static void BossFPartD8_TriggerHandSlam(s32 a) {
    gBossF->unk218->unk458 = a;
    BossFPartD8_Action_StartHandSlam(gBossF->unk218);
}
static void BossFPartD8_TriggerHandSlamHurt(void) { BossFPartD8_Action_HandSlamHurt(gBossF->unk218); }
static void BossFPartD8_TriggerHurt(void) { BossFPartD8_Action_Hurt(gBossF->unk218); }
static void BossFPartD8_TriggerHurtInSky(void) { BossFPartD8_Action_HurtInSky(gBossF->unk218); }
static void BossFPartD8_TriggerPartDAThrow(s32 a) {
    gBossF->unk218->unk458 = a;
    BossFPartD8_Action_StartPartDAThrow(gBossF->unk218);
}
static void BossFPartD8_TriggerPartDAThrowHurt(void) { BossFPartD8_Action_PartDAThrowHurt(gBossF->unk218); }
static void BossFPartD8_TriggerPartDCVolley(s32 a) {
    gBossF->unk218->unk458 = a;
    BossFPartD8_Action_StartPartDCVolley(gBossF->unk218);
}
static void BossFPartD8_TriggerPartDEAttack(s32 a) {
    gBossF->unk218->unk458 = a;
    BossFPartD8_Action_StartPartDEAttack(gBossF->unk218);
}
static void BossFPartD8_TriggerReturnHome(void) { BossFPartD8_Action_ReturnHome(gBossF->unk218); }
static void BossFPartD8_TriggerSkyCharge(s32 a) {
    gBossF->unk218->unk458 = a;
    BossFPartD8_Action_StartSkyCharge(gBossF->unk218);
}

/* BossFPartD8_ChooseAttack (copied) */
static void BossFPartD8_ChooseAttack(void) {
    BossF *w = gBossF;
    u16 hp = gBossStatus.unk8;
    if (hp >= 7) {
        switch (data_ov000_02363eac[GetLevel()][w->unk214][w->unk212]) {
        case 0:
            BossFPartD8_TriggerHandSlam(data_ov000_02363da4[Rand() & 1]);
            break;
        case 1:
            BossFPartD8_TriggerDoublePress(1);
            break;
        }
    } else if (hp >= 6) {
        switch (data_ov000_02363ebc[GetLevel()][w->unk214][w->unk212]) {
        case 0:
            BossFPartD8_TriggerHandSlam(data_ov000_02363e38[GetLevel()][Rand() & 3]);
            break;
        case 1:
            BossFPartD8_TriggerDoublePress(data_ov000_02363dc4[GetLevel()][Rand() & 1]);
            break;
        }
    } else if (hp >= 4) {
        switch (data_ov000_02363ecc[GetLevel()][w->unk214][w->unk212]) {
        case 0:
            BossFPartD8_TriggerHandSlam(data_ov000_02363e10[GetLevel()][Rand() & 3]);
            break;
        case 1:
            BossFPartD8_TriggerDoublePress(data_ov000_02363e28[GetLevel()][Rand() & 3]);
            break;
        case 3:
            BossFPartD8_TriggerPartDCVolley(data_ov000_02363db4[Rand() & 1]);
            break;
        }
    } else if (hp >= 2) {
        switch (data_ov000_02363eec[GetLevel()][w->unk214][w->unk212]) {
        case 0:
            BossFPartD8_TriggerHandSlam(data_ov000_02363e30[GetLevel()][Rand() & 3]);
            break;
        case 1:
            BossFPartD8_TriggerDoublePress(data_ov000_02363e18[GetLevel()][Rand() & 3]);
            break;
        case 3:
            BossFPartD8_TriggerPartDCVolley(data_ov000_02363de0[Rand() & 3]);
            break;
        case 4:
            BossFPartD8_TriggerPartDEAttack(data_ov000_02363da8[Rand() & 1]);
            break;
        }
    } else {
        BossFPartD8_TriggerSkyCharge(0);
    }
    w->unk212++;
    if (w->unk212 >= 4) {
        w->unk212 = 0;
        w->unk214 ^= 1;
    }
}

/* ============================================================ BossFPartD8: hit callbacks */
/* BossFPartD8_OnHitBody (modified: owner type -> obj_box_player) */
static void BossFPartD8_OnHitBody(ObjBox *a, ObjBox *b) {
    DcPlayer *pl = obj_box_player(b);
    DcPlayer *w = (DcPlayer *)a->owner;
    u16 hp = gBossStatus.unk8;
    s16 e1;
    s16 e2;
    s32 v;
    if (pl == NULL)
        return;
    if (hp == 0)
        return;
    if (gBossStatus.unk4 == 0x38) {
        gBossStatus.unk8 = hp - 1;
        g_boss_stats.hits++;
        s_st.sky_hits++;
        boss_log("hit", gBossStatus.unk8, 0x38);
        if (gBossStatus.unk8 != 0) {
            Sound_PlaySeqArc(0, 0, 0x7b);
            Sound_PlayVoice(0, 0, (Rand() & 1) ? 0xce : 0xcf);
            BossFPartD8_CreateHitEffect(b->x << 8, b->y << 8, 0);
            BossFPartD8_TriggerHurtInSky();
            return;
        }
        BossFPartD8_TriggerDefeat();
        return;
    }
    if (gPlayers->anim == 0x30) {
        gBossStatus.unk8 = hp - 1;
        g_boss_stats.hits++;
        s_st.body_hits++;
        boss_log("hit", gBossStatus.unk8, gBossStatus.unk4);
        Sound_PlaySeqArc(0, 0, 0x7b);
        Sound_PlayVoice(0, 0, (Rand() & 1) ? 0xce : 0xcf);
        BossFPartD8_CreateHitEffect(b->x << 8, b->y << 8, 0);
        BossFPartD8_TriggerHurt();
        pl->push_x = (s16)((((a->x + ((w->disp_flags & 1) ? a->l : (s16)-a->r)) - b->r) << 8) - pl->x);
        pl->move_flags |= 4;
        if (gBossStatus.unk8 <= 1) {
            if (gBossF->unk20C & 1)
                return;
            NaviMsg_SetFaceAnim(0xb);
            NaviMsg_ShowMessage(0x12, 0xf0);
            return;
        }
        NaviMsg_SetFaceAnim(0xb);
        NaviMsg_ShowMessage(0x10, 0xf0);
        return;
    }
    if (w->disp_flags & 1) {
        e1 = a->l;
        e2 = a->r;
    } else {
        e1 = (s16)-a->r;
        e2 = (s16)-a->l;
    }
    v = (pl->move_flags & 0x8000) ? pl->vel_x : pl->gnd_vel;
    if ((a->x + ((e1 + e2) >> 1)) << 8 < pl->x) {
        if (v > 0)
            return;
        pl->push_x = (s16)((((a->x + e2) - b->l) << 8) - pl->x);
        pl->move_flags |= 4;
        pl->vel_x = 0;
        pl->gnd_vel = 0;
    } else {
        if (v < 0)
            return;
        pl->push_x = (s16)((((a->x + e1) - b->r) << 8) - pl->x);
        pl->move_flags |= 4;
        pl->vel_x = 0;
        pl->gnd_vel = 0;
    }
}

/* BossFPartD8_OnHitHand (modified: owner type -> obj_box_player; branches on the character:
 * during the throw the bounce pushes Blaze (unk324 1) further back than Sonic) */
static void BossFPartD8_OnHitHand(ObjBox *a, ObjBox *b) {
    DcPlayer *pl = obj_box_player(b);
    DcPlayer *w = (DcPlayer *)a->owner;
    s16 e1;
    s16 e2;
    s32 v;
    s32 mx, my, mz;
    if (pl == NULL)
        return;
    if (pl->anim == 0x30) {
        Player_Action_AttackBounce(pl);
        if (gBossStatus.unk4 == 0x32) {
            if (gPlayers->chara == 0)
                pl->vel_x = -0x800;
            else
                pl->vel_x = -0xb00;
        }
        player_model_pos(&mx, &my, &mz); /* pl->unk17C / unk180 / unk184 */
        BossEffect_CreateDmgHand(1, mx >> 4, -my >> 4, mz >> 4);
        Sound_PlaySeqArc(0, 0, 0x7b);
        s_st.hand_hits++;
        boss_log("hand_hit", gBossStatus.unk4, pl->chara);
        switch (gBossStatus.unk4) {
        case 0x30:
            BossFPartD8_TriggerHandSlamHurt();
            break;
        case 0x31:
            BossFPartD8_TriggerDoublePressHurt();
            break;
        case 0x32:
            BossFPartD8_TriggerPartDAThrowHurt();
            break;
        }
        return;
    }
    if (w->disp_flags & 1) {
        e1 = a->l;
        e2 = a->r;
    } else {
        e1 = (s16)-a->r;
        e2 = (s16)-a->l;
    }
    v = (pl->move_flags & 0x8000) ? pl->vel_x : pl->gnd_vel;
    if (a->x << 8 < pl->x) {
        if (v > 0)
            return;
        pl->push_x = (s16)((((a->x + e2) - b->l) << 8) - pl->x);
        pl->move_flags |= 4;
        pl->vel_x = 0;
        pl->gnd_vel = 0;
    } else {
        if (v < 0)
            return;
        pl->push_x = (s16)((((a->x + e1) - b->r) << 8) - pl->x);
        pl->move_flags |= 4;
        pl->vel_x = 0;
        pl->gnd_vel = 0;
    }
}

/* ============================================================ BossFPartD8: task */
/* BossFPartD8_NodeCallback_Body (substantially rewritten: the DS stores the matrices of body_core,
 * body_sh_l, body_sh_r, body_fist_l, body_fist_r in GE slots 0x1e..0x1a during the draw and
 * BossFPartD8_Draw reads them back with MTX_RESTORE + NNS_G3dGetCurrentMtx; here the joint world
 * matrices are read after the pose) */
static void BossFPartD8_NodeCallback_Body(BossFPartD8 *obj) {
    fx32 *dst[5];
    dcm_mtx_t m;
    int i;
    dst[0] = obj->unk340;
    dst[1] = obj->unk370;
    dst[2] = obj->unk3A0;
    dst[3] = obj->unk3D0;
    dst[4] = obj->unk400;
    for (i = 0; i < 5; i++) {
        if (s_joint[i] < 0)
            continue;
        boss_model_joint_world(&obj->unk470, s_joint[i], &m);
        mtx_to_fx43(&m, dst[i]);
    }
}

/* BossFPartD8_Draw (modified: node matrices from BossFPartD8_NodeCallback_Body) */
static void BossFPartD8_Draw(void *p_) {
    BossFPartD8 *obj = (BossFPartD8 *)p_;
    BossModel *cam = &obj->unk470;
    if (obj->h.disp_flags & 0x20)
        return;
    cam->trans[0] = obj->h.x << 4;
    cam->trans[1] = -(obj->h.y << 4);
    cam->trans[2] = obj->h.z << 4;
    MTX_RotY33(&cam->rot, SIN(obj->unk45E), COS(obj->unk45E));
    Model_Animate(cam);
    Model_Draw(cam);
    BossFPartD8_NodeCallback_Body(obj);
}

/* BossFPartD8_UpdateColliderFromNode (copied) */
static void BossFPartD8_UpdateColliderFromNode(BossFPartD8 *p0, ObjBox *p1, const fx32 *p2) {
    if (p1->owner == NULL)
        return;
    obj_handle_collider(&p0->h, p1->owner, OBJ_OWNER_OBJECT, p1, 0);
    if (p2 == NULL)
        return;
    p1->x = (s32)((u32)p2[9] << 4) >> 0x10;
    p1->y = (s32)((u32)-p2[10] << 4) >> 0x10;
    p1->z = (s16)(p2[11] >> 0xc);
}

/* the collision map owner follows the part (obj_manager.c obj_update_ref + the StageObj header the
 * ObjCollisionMap code reads) */
static void d8_sync_cmap(BossFPartD8 *s) {
    Obj *o = &s->cm;
    o->h.x = s->h.x;
    o->h.y = s->h.y;
    o->h.z = s->h.z;
    o->h.px = s->h.px;
    o->h.py = s->h.py;
    o->h.pz = s->h.pz;
    o->h.move_x = s->h.move_x;
    o->h.move_y = s->h.move_y;
    o->h.move_z = s->h.move_z;
    o->h.angle = s->h.angle;
    o->h.disp_flags = s->h.disp_flags;
    o->h.obj_flags = s->h.obj_flags;
    o->h.move_flags = s->h.move_flags;
    o->ref.x = s->h.x;
    o->ref.y = s->h.y;
    o->ref.z = s->h.z;
    o->ref.px = s->h.px;
    o->ref.py = s->h.py;
    o->ref.pz = s->h.pz;
    o->ref.vx = s->h.move_x;
    o->ref.vy = s->h.move_y;
    o->ref.vz = s->h.move_z;
    o->ref.max = s->h.ride_max;
    o->ref.obj_flags = s->h.obj_flags;
    o->ref.move_flags = s->h.move_flags;
}

/* BossFPartD8_Collide (modified: ObjCollisionMap_Register -> the objects module's colmap list) */
static void BossFPartD8_Collide(void *p_) {
    BossFPartD8 *t0 = (BossFPartD8 *)p_;
    if ((t0->h.obj_flags & 0xc) != 0)
        return;
    BossFPartD8_UpdateColliderFromNode(t0, &t0->coll[0], t0->unk340);
    BossFPartD8_UpdateColliderFromNode(t0, &t0->coll[1], t0->unk3D0);
    BossFPartD8_UpdateColliderFromNode(t0, &t0->coll[2], t0->unk3D0);
    BossFPartD8_UpdateColliderFromNode(t0, &t0->coll[3], t0->unk3D0);
    BossFPartD8_UpdateColliderFromNode(t0, &t0->coll[4], t0->unk400);
    BossFPartD8_UpdateColliderFromNode(t0, &t0->coll[5], t0->unk400);
    BossFPartD8_UpdateColliderFromNode(t0, &t0->coll[6], t0->unk400);
    d8_sync_cmap(t0);
    if (t0->unk1B8 == NULL)
        return;
    colmap_register(&t0->cm.cmap);
}

/* BossFPartD8_Destructor (MapObject_Destructor) */
static void BossFPartD8_Destructor(void *p_) {
    BossFPartD8 *s = (BossFPartD8 *)p_;
    boss_model_release(&s->unk470);
    boxes_forget(&s->unk134, 3);
    boxes_forget(s->coll, 7);
    colmap_forget(&s->cm.cmap);
    if (gPlayers != NULL && gPlayers->ride == &s->cm.ref)
        gPlayers->ride = NULL;
    if (gPlayers != NULL && gPlayers->touch == &s->cm.ref)
        gPlayers->touch = NULL;
}

/* BossFPartD8_Create (modified: archive files from BossStageF; the collision map owner is s->cm) */
static BossFPartD8 *BossFPartD8_Create(const ObjParam *p, s32 b, s32 cc) {
    BossF *mgr = gBossF;
    BossFPartD8 *w = &s_d8;
    ObjBox *c;
    BossModel *sub;
    boss_model_release(&w->unk470);
    memset(w, 0, sizeof(*w)); /* MIi_CpuClear16 */
    w->alive = 1;
    w->func = BossFPartD8_State_Idle;
    w->unk46C = &s_bsf_ca;               /* "bsf" file 8 */
    w->unk460 = s_st_r_df;               /* 0xc */
    w->unk464 = s_st_r_di;               /* 0xd */
    w->unk468 = &data_ov000_02369b88[1]; /* bsf_body */
    MapObject_Init(&w->h, &w->unk134, p, b, cc);
    w->h.obj_flags |= 0x10;
    w->h.move_flags |= 0x8300;
    w->h.disp_flags |= 0x20;
    w->unk78 = BossFPartD8_Collide;
    w->h.x = mgr->h.x;
    w->h.y = mgr->h.y;
    w->h.z = -0x1f400;
    c = &w->coll[0];
    *c = w->unk134;
    rect_set_atkdef(c, 0x11, 0);
    c->flags |= 0x40;
    c->owner = w;
    c->cb = BossFPartD8_OnHitBody;
    c->flags &= ~4;
    c = &w->coll[1];
    *c = w->unk160;
    c->owner = w;
    c->flags &= ~4;
    c = &w->coll[2];
    *c = w->unk160;
    c->atk |= 0x20;
    c->owner = w;
    c->flags &= ~4;
    c = &w->coll[3];
    *c = w->unk134;
    rect_set_atkdef(c, 0x11, 0);
    c->flags |= 0x40;
    c->owner = w;
    c->cb = BossFPartD8_OnHitHand;
    c->flags &= ~4;
    c = &w->coll[4];
    *c = w->unk160;
    c->owner = w;
    c->flags &= ~4;
    c = &w->coll[5];
    *c = w->unk160;
    c->atk |= 0x20;
    c->owner = w;
    c->flags &= ~4;
    c = &w->coll[6];
    *c = w->unk134;
    rect_set_atkdef(c, 0x11, 0);
    c->flags |= 0x40;
    c->owner = w;
    c->cb = BossFPartD8_OnHitHand;
    c->flags &= ~4;
    BossFPartD8_InitHitboxes(w);
    /* StageObj @0x1B8..@0x1DA: the arm's collision map (bsf_st_r) */
    w->cm.cmap.obj = &w->cm;
    w->cm.cmap.map = (const u8(*)[8])w->unk460;   /* unk1C8 */
    w->cm.cmap.angles = w->unk464;                /* unk1CC */
    w->cm.cmap.w = 0x56;                          /* unk1D8 */
    w->cm.cmap.h = 0x28;                          /* unk1DA */
    w->cm.cmap.ox = -0x2e4;                       /* unk1D4 */
    w->cm.cmap.oy = -0x144;                       /* unk1D6 */
    w->cm.has_cmap = 1;
    w->cm.used = 1;
    w->unk1B8 = NULL;
    d8_sync_cmap(w);
    sub = &w->unk470;
    Model_Init(sub, 0);
    Model_SetResource(sub, w->unk468);
    Model_SetAnimation(sub, 0, w->unk46C, 6);
    sub->flags[0] |= 4; /* unk10C */
    /* NNS_G3dRenderObjSetCallBack(BossFPartD8_NodeCallback_Body): run after the pose */
    sub->scale[0] = sub->scale[1] = sub->scale[2] = 0x34cc;
    sub->cb[0] = MOP_FLUSHVP; /* unkA = 4 */
    return w;
}

/* ============================================================ BossFPartD8: states */
/* BossFPartD8_State_Idle (copied) */
STATE(BossFPartD8_State_Idle) { (void)p_; }

/* BossFPartD8_State_WaitNextAttack (copied) */
STATE(BossFPartD8_State_WaitNextAttack) {
    D8;
    BossFPartD8_TrackPlayerX_Slow(s);
    s->unk434--;
    if (s->unk434 != 0)
        return;
    BossFPartD8_ChooseAttack();
}

/* ------------------------------------------------ hand slam (0x30) */
STATE(BossFPartD8_State_HandSlamRaise) {
    D8;
    BossFPartD8_SetAnimation(s, 7, 0x1000, 0);
    s->func = BossFPartD8_State_HandSlamRaiseWait;
}
STATE(BossFPartD8_State_HandSlamRaiseWait) {
    D8;
    BossFPartD8_TrackPlayerX_Fast(s);
    if (s->unk57C & 2)
        s->func = BossFPartD8_State_HandSlamNext;
}
STATE(BossFPartD8_State_HandSlamNext) {
    BossFPartD8 *p = (BossFPartD8 *)p_;
    p->unk43C = 0;
    if (data_ov000_02364248[p->unk458][p->unk438] != 0)
        p->func = BossFPartD8_State_HandSlamTrack;
    else
        p->func = BossFPartD8_State_HandSlamStrike;
    p->func(p);
}
STATE(BossFPartD8_State_HandSlamTrack) {
    D8;
    u16 t;
    BossF_UpdateCameraShake(&s->unk43A, 2, 6);
    t = data_ov000_02364248[s->unk458][s->unk438];
    if (s->unk458 >= 6 && s->unk458 <= 8)
        BossFPartD8_TrackPlayerX_Fast(s);
    else
        BossFPartD8_TrackPlayerX_Offset(s, s->unk434w);
    if (t < s->unk43C++ || (t > 0x2d && s->h.vel_x == 0))
        s->func = BossFPartD8_State_HandSlamStrike;
}
STATE(BossFPartD8_State_HandSlamStrike) {
    D8;
    s32 a;
    s->h.vel_x = 0;
    if (s->unk458 >= 6 && s->unk458 <= 8 && s->unk438 != 0) {
        if (s->unk434w != 0)
            a = 0xa;
        else
            a = 0xb;
    } else {
        if (s->unk434w != 0)
            a = 8;
        else
            a = 9;
    }
    BossFPartD8_SetAnimation(s, a, data_ov000_02364020[GetIdxB()][s->unk458], 0);
    s->func = BossFPartD8_State_HandSlamImpact;
    s->func(s);
}
STATE(BossFPartD8_State_HandSlamImpact) {
    D8;
    const fx32 *p;
    ObjBox *h;
    s32 d;
    BOOL b;
    s32 i;
    u16 fl;
    BossF_UpdateCameraShake(&s->unk43A, 2, 6);
    p = s->unk434w ? s->unk3D0 : s->unk400;
    h = s->unk434w ? &s->coll[1] : &s->coll[4];
    if (p[10] < -0x200000) /* p->unk28: the fist is down */
        h->flags |= 4;
    else
        h->flags &= ~4;
    if (!(s->unk57C & 2))
        return;
    d = s->unk434w ? -0x1000 : 0x1000;
    b = TRUE;
    if (g_dcp_world.mode != 2 && g_dcp_world.mode != 3)
        b = FALSE;
    if (b)
        i = 1;
    else
        i = g_boss_game.unk14;
    if (data_ov000_02363e9c[i][gBossStatus.unk8 - 1] != 0) {
        fl = 0;
        if (s->unk458 >= 6 && s->unk434w == 0)
            fl |= 4;
        MapObject_Spawn(0xdb, (p[9] >> 4) + 0x3000, 0x30000, fl);
        if (s->unk458 >= 6)
            fl ^= 4;
        MapObject_Spawn(0xdb, (p[9] >> 4) - 0x3000, 0x30000, fl | 1);
    }
    BossEffect_CreateImpactSl(1, d + (p[9] >> 4), 0x30000, 0);
    s->unk43A = 0x40;
    Sound_PlaySeqArc(0, 0, 0xc0);
    g_boss_stats.slams++;
    s->func = BossFPartD8_State_HandSlamStartRecover;
}
STATE(BossFPartD8_State_HandSlamStartRecover) {
    BossFPartD8 *w = (BossFPartD8 *)p_;
    w->unk43E = 0;
    if (w->unk438 + 1 < data_ov000_02363e48[w->unk458])
        w->unk43C = data_ov000_02363ffc[GetLevel()][w->unk458];
    else
        w->unk43C = data_ov000_02363f7c[GetLevel()][gBossStatus.unk8 - 1];
    if (w->unk43C != 0) {
        if (w->unk434w != 0) {
            w->coll[1].flags &= ~4; /* unk242 */
            w->coll[3].flags |= 4;  /* unk29A */
        } else {
            w->coll[4].flags &= ~4; /* unk2C6 */
            w->coll[6].flags |= 4;  /* unk31E */
        }
        w->func = BossFPartD8_State_HandSlamVulnerable;
    } else {
        w->func = BossFPartD8_State_HandSlamLift;
    }
    w->func(w);
}
STATE(BossFPartD8_State_HandSlamVulnerable) {
    D8;
    BossF_UpdateCameraShake(&s->unk43A, 2, 6);
    s->unk43E++;
    if ((s->unk43E & 3) == 0) {
        if (s->unk43E & 0x8000) {
            if (s->unk434w != 0)
                BossFPartD8_LoadModelPalettesB(gGameState, 0);
            else
                BossFPartD8_LoadModelPalettesA(gGameState, 0);
        } else {
            if (s->unk434w != 0)
                BossFPartD8_LoadModelPalettesB(gGameState, 1);
            else
                BossFPartD8_LoadModelPalettesA(gGameState, 1);
        }
        s->unk43E &= ~3;
        s->unk43E ^= 0x8000;
    }
    if (s->unk43C != 0)
        s->unk43C--;
    if (s->unk43C != 0)
        return;
    if (s->unk43E & 0x8000)
        return;
    BossFPartD8_DisableHitboxes(s);
    s->func = BossFPartD8_State_HandSlamLift;
}
STATE(BossFPartD8_State_HandSlamLift) {
    D8;
    s32 t0, v1;
    BossFPartD8_DisableHitboxes(s);
    s->unk438 = (u16)(s->unk438 + 1);
    t0 = s->unk458;
    if (s->unk438 < data_ov000_02363e48[s->unk458]) {
        if (t0 >= 6) {
            s->unk434w = (s->unk434w == 0);
            s->func = BossFPartD8_State_HandSlamStrike;
            return;
        }
        v1 = (s->unk434w != 0) ? 0xe : 0xf;
    } else {
        v1 = (s->unk434w != 0) ? 0xc : 0xd;
    }
    BossFPartD8_SetAnimation(s, v1, 0x1000, 0);
    s->func = BossFPartD8_State_HandSlamLiftWait;
    s->func(s);
}
STATE(BossFPartD8_State_HandSlamLiftWait) {
    D8;
    BossF_UpdateCameraShake(&s->unk43A, 2, 6);
    if (s->unk438 < data_ov000_02363e48[s->unk458])
        BossFPartD8_TrackPlayerX_Offset(s, s->unk434w == 0);
    if (!(s->unk57C & 2))
        return;
    if (s->unk438 < data_ov000_02363e48[s->unk458]) {
        s->unk434w = s->unk434w == 0;
        s->func = BossFPartD8_State_HandSlamNext;
        return;
    }
    BossF_StartNextAttack();
}
STATE(BossFPartD8_State_HandSlamHurt) {
    D8;
    s->unk57C |= 0x40;
    BossFPartD8_SetAnimation(s, 0x10, 0x1000, 0);
    s->func = BossFPartD8_State_HandSlamHurtWait;
    s->func(s);
}
STATE(BossFPartD8_State_HandSlamHurtWait) {
    D8;
    BossF_UpdateCameraShake(&s->unk43A, 2, 6);
    if ((s->unk57C & 2) == 0)
        return;
    BossFPartD8_TriggerPartDAThrow(BossFPartD8_RollThrowVariant()); /* r0 passes through */
}

/* ------------------------------------------------ double press (0x31) */
STATE(BossFPartD8_State_DoublePressStart) {
    D8;
    BossFPartD8_SetAnimation(s, 0x11, 0x1000, 0);
    CAM.unkC0 = 0x12c000;  /* +0x78 */
    CAM.unkC8 = 0x40;      /* +0x80 */
    CAM.unkCC = -0x50000;  /* +0x84 */
    CAM.unkD4 = 0x40;      /* +0x8c */
    Sound_PlayVoice(0, 0, 0xd2);
    s->func = BossFPartD8_State_DoublePressRaise;
}
STATE(BossFPartD8_State_DoublePressRaise) {
    D8;
    BossFPartD8_TrackPlayerX_Far(s);
    if (s->unk57C & 2)
        s->func = BossFPartD8_State_DoublePressSetTrack;
}
STATE(BossFPartD8_State_DoublePressSetTrack) {
    D8;
    s->unk434 = data_ov000_02363f4c[s->unk458];
    s->func = BossFPartD8_State_DoublePressTrack;
    s->func(s);
}
STATE(BossFPartD8_State_DoublePressTrack) {
    D8;
    if (GetIdxB() == 1) {
        BossFPartD8_TrackPlayerX_Far(s);
    } else if (GetIdxS() == 0) {
        if (s->unk434 <= 0x3c)
            s->h.vel_x = 0;
        else
            BossFPartD8_TrackPlayerX_Far(s);
    }
    if (s->unk434-- == 0)
        s->func = BossFPartD8_State_DoublePressStrike;
}
STATE(BossFPartD8_State_DoublePressStrike) {
    D8;
    s->h.vel_x = 0;
    BossFPartD8_SetAnimation(s, 0x12, data_ov000_02363e54[GetIdxB()][s->unk458], 0);
    BossF_InitCamera();
    s->func = BossFPartD8_State_DoublePressImpact;
}
STATE(BossFPartD8_State_DoublePressImpact) {
    D8;
    const fx32 *p;
    BOOL b;
    s32 i;
    p = s->unk434w ? s->unk3D0 : s->unk400;
    if (p[10] < -0x200000) {
        s->coll[2].flags |= 4; /* unk26E */
        s->coll[5].flags |= 4; /* unk2F2 */
    } else {
        s->coll[2].flags &= ~4;
        s->coll[5].flags &= ~4;
    }
    if (!(s->unk57C & 2))
        return;
    s->unk436 = 0x80;
    b = TRUE;
    if (g_dcp_world.mode != 2 && g_dcp_world.mode != 3)
        b = FALSE;
    if (b)
        i = 1;
    else
        i = g_boss_game.unk14;
    if (data_ov000_02363e9c[i][gBossStatus.unk8 - 1] != 0) {
        MapObject_Spawn(0xdb, (s->unk424 >> 4) + 0xa000, 0x30000, 2);
        MapObject_Spawn(0xdb, (s->unk3F4 >> 4) - 0xa000, 0x30000, 3);
    }
    BossEffect_CreateImpactPr(1, (s->unk3F4 + s->unk424) >> 5, 0x30000, 0);
    s->coll[2].flags &= ~4;
    s->coll[5].flags &= ~4;
    s->coll[3].flags |= 4; /* unk29A */
    s->coll[6].flags |= 4; /* unk31E */
    Sound_PlaySeqArc(0, 0, 0xbf);
    g_boss_stats.big_slams++;
    s->func = BossFPartD8_State_DoublePressStartVulnerable;
}
STATE(BossFPartD8_State_DoublePressStartVulnerable) {
    BossFPartD8 *p = (BossFPartD8 *)p_;
    BOOL b;
    s32 idx;
    b = g_dcp_world.mode == 2 || g_dcp_world.mode == 3;
    if (b)
        idx = 1;
    else
        idx = g_boss_game.unk14;
    p->unk438 = data_ov000_023642b4[idx][gBossStatus.unk8 - 1];
    p->unk43A = 0;
    p->func = BossFPartD8_State_DoublePressVulnerable;
}
/* BossFPartD8_State_DoublePressVulnerable (copied: the blink period counts @0x43E, the blink phase
 * toggles @0x43A, as in the original) */
STATE(BossFPartD8_State_DoublePressVulnerable) {
    D8;
    BossF_UpdateCameraShake(&s->unk436, 3, 7);
    s->unk43E = (u16)(s->unk43E + 1);
    if ((s->unk43E & 3) == 0) {
        if ((s->unk43A & 0x8000) != 0) {
            BossFPartD8_LoadModelPalettesB(gGameState, 0);
            BossFPartD8_LoadModelPalettesA(gGameState, 0);
        } else {
            BossFPartD8_LoadModelPalettesB(gGameState, 1);
            BossFPartD8_LoadModelPalettesA(gGameState, 1);
        }
        s->unk43A &= ~3;
        s->unk43A ^= 0x8000;
    }
    if (s->unk438 != 0)
        s->unk438 = (u16)(s->unk438 - 1);
    if (s->unk438 != 0)
        return;
    if ((s->unk43A & 0x8000) != 0)
        return;
    BossFPartD8_DisableHitboxes(s);
    s->func = BossFPartD8_State_DoublePressEnd;
}
STATE(BossFPartD8_State_DoublePressEnd) {
    D8;
    BossFPartD8_SetAnimation(s, 0x13, 0x1000, 0);
    s->func = BossFPartD8_State_DoublePressEndWait;
}
STATE(BossFPartD8_State_DoublePressEndWait) {
    D8;
    if (!(s->unk57C & 2))
        return;
    BossF_StartNextAttack();
}
STATE(BossFPartD8_State_DoublePressHurt) {
    D8;
    BossFPartD8_SetAnimation(s, 0x14, 0x1000, 0);
    s->func = BossFPartD8_State_DoublePressHurtWait;
    s->func(s);
}
STATE(BossFPartD8_State_DoublePressHurtWait) {
    D8;
    BossF_UpdateCameraShake(&s->unk436, 3, 7);
    if ((s->unk57C & 2) == 0)
        return;
    BossFPartD8_TriggerPartDAThrow(BossFPartD8_RollThrowVariant());
}

/* ------------------------------------------------ throw (0x32): the arm rolls shock waves */
STATE(BossFPartD8_State_PartDAThrowMoveRight) {
    D8;
    if (s->unk45E != 0x4000) {
        s->unk45E = (u16)(s->unk45E + 0x200);
        if (s->unk45E > 0x4000)
            s->unk45E = 0x4000;
    }
    s->h.vel_x = (s16)(s->h.vel_x + 0x80);
    if (s->h.vel_x > 0x1000)
        s->h.vel_x = 0x1000;
    if (s->h.x > 0x3b800)
        s->func = BossFPartD8_State_PartDAThrowMoveToPos;
}
STATE(BossFPartD8_State_PartDAThrowMoveToPos) {
    D8;
    if (s->unk45E != 0xc000)
        s->unk45E = (u16)(s->unk45E - 0x200);
    s->h.vel_x = (s16)(s->h.vel_x + 0x80);
    if (s->h.vel_x > 0x1000)
        s->h.vel_x = 0x1000;
    if (s->h.x >= 0x5e800) {
        s->h.vel_x = 0;
        s->h.x = 0x5e800;
    }
    s->h.vel_z = (s16)(s->h.vel_z + 0x80);
    if (s->h.vel_z > 0x1000)
        s->h.vel_z = 0x1000;
    if (s->h.z >= 0) {
        s->h.vel_z = 0;
        s->h.z = 0;
    }
    if (s->h.x != 0x5e800)
        return;
    if (s->h.z != 0)
        return;
    if (s->unk45E == 0xc000)
        s->func = BossFPartD8_State_PartDAThrowStart;
}
STATE(BossFPartD8_State_PartDAThrowStart) {
    D8;
    BossFPartD8_SetAnimation(s, 0x15, 0x1000, 0);
    Sound_PlaySeqArc(0, 0, 0xc4);
    Sound_PlayVoice(0, 0, 0xd2);
    s->func = BossFPartD8_State_PartDAThrowStartWait;
}
STATE(BossFPartD8_State_PartDAThrowStartWait) {
    D8;
    if (s->unk57C & 2)
        s->func = BossFPartD8_State_PartDAThrowSetDelay;
}
STATE(BossFPartD8_State_PartDAThrowSetDelay) {
    BossFPartD8 *p = (BossFPartD8 *)p_;
    p->unk438 = data_ov000_02364120[p->unk458][p->unk434];
    p->func = BossFPartD8_State_PartDAThrowDelay;
    p->func(p);
}
STATE(BossFPartD8_State_PartDAThrowDelay) {
    D8;
    s32 t0;
    BossF_UpdateCameraShake(&s->unk436, 2, 6);
    t0 = s->unk438;
    s->unk438 = (u16)(s->unk438 - 1);
    if (t0 == 0)
        s->func = BossFPartD8_State_PartDAThrowWindup;
}
STATE(BossFPartD8_State_PartDAThrowWindup) {
    D8;
    BossFPartD8_SetAnimation(s, 0x16, data_ov000_0236415c[s->unk458][s->unk434], 0);
    s->coll[4].flags |= 4; /* coll2BC */
    rect_set_box3d(&s->coll[4], 0x10, -0x30, 0x70, 0x38, -0x7d00, 0x7d00);
    s->func = BossFPartD8_State_PartDAThrowRelease;
}
STATE(BossFPartD8_State_PartDAThrowRelease) {
    D8;
    s32 x;
    DcPlayer *o;
    if (!(s->unk57C & 2))
        return;
    s->unk436 = 0x40;
    x = (s->unk424 >> 4) - 0x7800;
    o = (DcPlayer *)MapObject_Spawn(0xda, x, 0x30000, 1);
    if (o != NULL)
        o->vel_x = (s16)((o->vel_x * (s32)data_ov000_0236415c[s->unk458][s->unk434]) >> 12);
    Sound_PlaySeqArc(0, 0, 0xbe);
    if (s->unk434 + 1 != data_ov000_02363df0[s->unk458]) {
        s->func = BossFPartD8_State_PartDAThrowFollowThrough;
        return;
    }
    BossEffect_CreateImpactSt(1, x, 0x30000, 0);
    s->func = BossFPartD8_State_PartDAThrowStartVulnerable;
}
STATE(BossFPartD8_State_PartDAThrowFollowThrough) {
    D8;
    BossF *m = gBossF;
    s32 v;
    BossFPartD8_DisableHitboxes(s);
    if (s->unk434 + 1 < data_ov000_02363df0[s->unk458])
        v = data_ov000_0236415c[s->unk458][s->unk434];
    else
        v = 0x1000;
    BossFPartD8_SetAnimation(s, 0x17, v, 0);
    m->unk20C &= ~0x40;
    s->unk1B8 = NULL;
    s->func = BossFPartD8_State_PartDAThrowFollowThroughWait;
    s->func(s);
}
STATE(BossFPartD8_State_PartDAThrowFollowThroughWait) {
    BossFPartD8 *p = (BossFPartD8 *)p_;
    BossF_UpdateCameraShake(&p->unk436, 2, 6);
    if (!(p->unk57C & 2))
        return;
    p->unk434++;
    if (p->unk434 < data_ov000_02363df0[p->unk458])
        p->func = BossFPartD8_State_PartDAThrowSetDelay;
    else
        p->func = BossFPartD8_State_PartDAThrowEnd;
}
STATE(BossFPartD8_State_PartDAThrowStartVulnerable) {
    D8;
    BossF *m = gBossF;
    BossFPartD8_DisableHitboxes(s);
    s->coll[6].flags |= 4; /* coll314 */
    rect_set_box3d(&s->coll[6], 0x10, -0x80, 0x70, 0x38, -0x7d00, 0x7d00);
    s->unk438 = data_ov000_02364344[GetIdxB()][gBossStatus.unk8 - 1];
    s->unk43A = 0;
    s->unk430 = 0;
    m->unk20C |= 0x40;
    s->unk1B8 = s;
    s->func = BossFPartD8_State_PartDAThrowVulnerable;
    s->func(s);
}
STATE(BossFPartD8_State_PartDAThrowVulnerable) {
    D8;
    BossF_UpdateCameraShake(&s->unk436, 2, 6);
    s->unk43A++;
    if (!(s->unk43A & 3)) {
        if (s->unk43A & 0x8000)
            BossFPartD8_LoadModelPalettesA(gGameState, 0);
        else
            BossFPartD8_LoadModelPalettesA(gGameState, 1);
        s->unk43A &= ~3;
        s->unk43A ^= 0x8000;
    }
    if (data_ov000_02363edc[GetIdxB()][gBossStatus.unk8 - 1] != 0) {
        if (s->unk430-- == 0) {
            BossFPartD8_SpawnPartD9();
            s->unk430 = data_ov000_02363fdc[GetIdxB()][gBossStatus.unk8 - 1];
        }
    }
    if (s->unk438 != 0)
        s->unk438--;
    if (s->unk438 != 0)
        return;
    if (!(s->unk43A & 0x8000))
        s->func = BossFPartD8_State_PartDAThrowFollowThrough;
}
STATE(BossFPartD8_State_PartDAThrowEnd) {
    D8;
    BossFPartD8_SetAnimation(s, 0x18, 0x1000, 0);
    s->func = BossFPartD8_State_PartDAThrowEndWait;
    s->func(s);
}
STATE(BossFPartD8_State_PartDAThrowEndWait) {
    D8;
    BossF_UpdateCameraShake(&s->unk436, 2, 6);
    if ((s->unk57C & 2) == 0)
        return;
    BossFPartD8_TriggerReturnHome();
}
STATE(BossFPartD8_State_PartDAThrowHurt) {
    D8;
    BossFPartD8_SetAnimation(s, 0x19, 0x1000, 0);
    s->func = BossFPartD8_State_PartDAThrowHurtWait;
}
STATE(BossFPartD8_State_PartDAThrowHurtWait) {
    D8;
    if (!(s->unk57C & 2))
        return;
    BossFPartD8_TriggerHandRide();
}

/* ------------------------------------------------ hand ride (0x33 / 0x34 / 0x35) */
STATE(BossFPartD8_State_HandRideStartWait) {
    D8;
    BossF *m = gBossF;
    s->unk434 = data_ov000_023641e8[GetIdxB()][gBossStatus.unk8 - 1][s->unk436];
    BossFPartD8_SetCameraFollowPlayer();
    m->unk20C |= 0x40;
    gBossStatus.unk4 = 0x33;
    s->func = BossFPartD8_State_HandRideWait;
}
STATE(BossFPartD8_State_HandRideWait) {
    D8;
    BossFPartD8_BlinkPalette(s);
    if (data_ov000_02363edc[GetIdxB()][gBossStatus.unk8 - 1] != 0) {
        if (s->unk430-- == 0) {
            BossFPartD8_SpawnPartD9();
            s->unk430 = data_ov000_02363fdc[GetIdxB()][gBossStatus.unk8 - 1];
        }
    }
    s->unk434--;
    if (s->unk434 == 0)
        s->func = BossFPartD8_State_HandRideStartWarn;
}
/* BossFPartD8_State_HandRideStartWarn (modified: BossHelp_Create -> help hook) */
STATE(BossFPartD8_State_HandRideStartWarn) {
    D8;
    BOOL b;
    s32 i;
    BossFPartD8_SetAnimation(s, 0x1d, 0x1000, 1);
    b = TRUE;
    if (g_dcp_world.mode != 2 && g_dcp_world.mode != 3)
        b = FALSE;
    if (b)
        i = 1;
    else
        i = g_boss_game.unk14;
    s->unk434 = data_ov000_02363f9c[i][gBossStatus.unk8 - 1];
    BossHelp_Create_F(1);
    s->func = BossFPartD8_State_HandRideWarn;
    s->func(s);
}
STATE(BossFPartD8_State_HandRideWarn) {
    D8;
    BossF *m = gBossF;
    BossFPartD8_BlinkPalette(s);
    s->unk434--;
    if (s->unk434 != 0)
        return;
    s->unk436++;
    if (s->unk436 < 3)
        s->func = BossFPartD8_State_StartHandRideMove;
    else
        s->func = BossFPartD8_State_HandRideEnd;
    m->unk20C &= ~0x40;
}
/* BossFPartD8_State_StartHandRideMove (modified: gPlayers->unk70 -> draw_player hook) */
STATE(BossFPartD8_State_StartHandRideMove) {
    D8;
    s32 dx;
    s32 dy;
    BossFPartD8_SetAnimation(s, 0x1a, 0x1000, 0);
    BossF_LoadPalette(gGameState, 0);
    dx = s->unk3C4 - s->unk424;
    dy = s->unk3C8 - s->unk428;
    s->unk438 = (u16)(FX_Atan2(dy, dx) + 0x8000);
    dx = s->unk3C4 - (gPlayers->x << 4);
    dy = s->unk3C8 - (-gPlayers->y << 4);
    s->unk43A = (u16)(FX_Atan2(dy, dx) + 0x8000);
    dx = s->unk3C4 - (gPlayers->x << 4);
    dy = s->unk3C8 - (-gPlayers->y << 4);
    s->unk43Cw = FX_Sqrt(FX_Mul(dx, dx) + FX_Mul(dy, dy));
    CAM.t[1].accel = 0x2000; /* data_ov000_0236a48c.unk74 */
    CAM.t[0].accel = 0x2000; /* .unk38 */
    if (!BossFPartD8_IsPlayerRiding()) {
        s->unk1B8 = NULL;
    } else {
        set_player_draw(BossFPartD8_Draw_RidingPlayer); /* gPlayers->base.unk70 */
        gPlayers->disp_flags &= ~0x40;
    }
    gBossStatus.unk4 = 0x34;
    s->func = BossFPartD8_State_HandRideMove;
    s_st.ride_moves++;
    boss_log("ride_move", BossFPartD8_IsPlayerRiding(), s->unk436);
}
STATE(BossFPartD8_State_HandRideMove) {
    D8;
    s32 x;
    s32 y;
    BossFPartD8_CheckKnockOffRider(s);
    if (BossFPartD8_IsPlayerRiding() && BossFPartD8_IsPlayerCrouching()) {
        BossFPartD8_GetRidePosition(s, &x, &y, 0);
        CAM.t[1].offx = ((x - gPlayers->x) << 4) + 0x14000; /* cam->unk48 */
        CAM.t[1].offy = 0x32000 - ((y - gPlayers->y) << 4); /* cam->unk4C */
    }
    if (s->unk57C & 2)
        s->func = BossFPartD8_State_HandRideStartHold;
}
STATE(BossFPartD8_State_HandRideStartHold) {
    D8;
    BossFPartD8_SetAnimation(s, 0x1b, 0x1000, 1);
    s->unk434 = (u16)((Rand() & 0x3f) + 0x3c);
    s->func = BossFPartD8_State_HandRideHold;
    s->func(s);
}
STATE(BossFPartD8_State_HandRideHold) {
    D8;
    s32 x;
    s32 y;
    BossFPartD8_CheckKnockOffRider(s);
    if (s->unk434 != 0)
        s->unk434--;
    if (BossFPartD8_IsPlayerRiding() && BossFPartD8_IsPlayerCrouching()) {
        BossFPartD8_GetRidePosition(s, &x, &y, 0);
        CAM.t[1].offx = ((x - gPlayers->x) << 4) + 0x14000;
        CAM.t[1].offy = 0x32000 - ((y - gPlayers->y) << 4);
    }
    if ((s->unk434 & 0x3f) == 0)
        Sound_PlaySeqArc(0, 0, 0xbe);
    if (!(s->unk57C & 2))
        return;
    if (s->unk434 != 0)
        return;
    s->func = BossFPartD8_State_HandRideStartReturn;
    s->func(s);
}
STATE(BossFPartD8_State_HandRideStartReturn) {
    D8;
    BossFPartD8_SetAnimation(s, 0x1c, 0x1000, 0);
    CAM.t[1].offx = 0x14000; /* +0x48 */
    CAM.t[1].offy = 0x32000; /* +0x4c */
    CAM.t[1].lerp = 0x80;    /* +0x6c */
    CAM.t[0].lerp = 0x80;    /* +0x30 */
    CAM.t[1].accel = 0;      /* +0x74 */
    CAM.t[0].accel = 0;      /* +0x38 */
    s->func = BossFPartD8_State_HandRideReturn;
    s->func(s);
}
STATE(BossFPartD8_State_HandRideReturn) {
    D8;
    BossFPartD8_CheckKnockOffRider(s);
    if ((s->unk57C & 2) == 0)
        return;
    set_player_draw(gBossF->unk21C); /* gPlayers->unk70 */
    gPlayers->disp_flags |= 0x40;
    s->unk1B8 = s;
    BossHelp_Destroy_F();
    s->func = BossFPartD8_State_HandRideStartWait;
}
STATE(BossFPartD8_State_HandRideEnd) {
    D8;
    s32 dx;
    s32 dy;
    BossFPartD8_SetAnimation(s, 0x1e, 0x1000, 0);
    BossF_LoadPalette(gGameState, 0);
    dx = s->unk3C4 - s->unk424;
    dy = s->unk3C8 - s->unk428;
    s->unk438 = (u16)(FX_Atan2(dy, dx) + 0x8000);
    dx = s->unk3C4 - (gPlayers->x << 4);
    dy = s->unk3C8 - (-gPlayers->y << 4);
    s->unk43A = (u16)(FX_Atan2(dy, dx) + 0x8000);
    dx = s->unk3C4 - (gPlayers->x << 4);
    dy = s->unk3C8 - (-gPlayers->y << 4);
    s->unk43Cw = FX_Sqrt(FX_Mul(dx, dx) + FX_Mul(dy, dy));
    if (BossFPartD8_Unk_ov000_022f8f78()) {
        if (gPlayers->anim != 0x11 && gPlayers->anim != 0x12) {
            Player_Action_Knockback(gPlayers, (0x1a000 - gPlayers->x) >> 7, -0x800);
            BossFPartD8_GetRidePosition(s, &gPlayers->x, &gPlayers->y, 0);
            set_player_draw(gBossF->unk21C);
            gPlayers->disp_flags |= 0x40;
        }
        BossFPartD8_SetCameraFollowPlayer();
    }
    s->unk1B8 = NULL;
    BossHelp_Destroy_F();
    gBossStatus.unk4 = 0x35;
    s->func = BossFPartD8_State_HandRideEndWait;
}
STATE(BossFPartD8_State_HandRideEndWait) {
    D8;
    BossFPartD8_Unk_ov000_022f89a8();
    if (s->unk57C & 2)
        s->func = BossFPartD8_State_HandRideRetract;
}
STATE(BossFPartD8_State_HandRideRetract) {
    D8;
    BossFPartD8_SetAnimation(s, 0x1f, 0x1000, 0);
    s->func = BossFPartD8_State_HandRideRetractWait;
}
STATE(BossFPartD8_State_HandRideRetractWait) {
    D8;
    s32 t0 = BossFPartD8_Unk_ov000_022f89a8();
    if ((s->unk57C & 2) == 0)
        return;
    if (t0 == 0) {
        gStageMapCollision_unk24 = 0x340;
        gPlayers->x = 0x33400;
    }
    BossFPartD8_TriggerReturnHome();
}
STATE(BossFPartD8_State_ReturnHome) {
    D8;
    s->h.vel_x = (s16)(s->h.vel_x - 0x80);
    if (s->h.vel_x < -0x1000)
        s->h.vel_x = -0x1000;
    if (s->h.x <= 0x1a000) {
        s->h.vel_x = 0;
        s->h.x = 0x1a000;
    }
    s->h.vel_z = (s16)(s->h.vel_z - 0x80);
    if (s->h.vel_z < -0x1000)
        s->h.vel_z = -0x1000;
    if (s->h.z <= -0x1f400) {
        s->h.vel_z = 0;
        s->h.z = -0x1f400;
    }
    if (s->h.vel_z != 0) {
        if (s->unk45E > 0xa000)
            s->unk45E = (u16)(s->unk45E - 0x200);
    } else {
        if (s->unk45E != 0)
            s->unk45E = (u16)(s->unk45E + 0x200);
    }
    if (s->unk45E != 0)
        return;
    if (s->h.x != 0x1a000)
        return;
    if (s->h.z != -0x1f400)
        return;
    BossF_StartNextAttack();
}

/* ------------------------------------------------ laser volley (0x36) */
STATE(BossFPartD8_State_PartDCVolleyStart) {
    D8;
    BossFPartD8_SetAnimation(s, 0x26, 0x1000, 0);
    Sound_PlayVoice(0, 0, 0xd0);
    s->func = BossFPartD8_State_PartDCVolleyRaise;
}
STATE(BossFPartD8_State_PartDCVolleyRaise) {
    D8;
    BossFPartD8_TrackPlayerX_NearSlow(s);
    if (s->unk57C & 2)
        s->func = BossFPartD8_State_PartDCVolleyStartDelay;
}
STATE(BossFPartD8_State_PartDCVolleyStartDelay) {
    D8;
    s->unk442 = 0;
    s->func = BossFPartD8_State_PartDCVolleyDelay;
}
STATE(BossFPartD8_State_PartDCVolleyDelay) {
    D8;
    s->unk442++;
    if (s->unk442 != 0)
        s->func = BossFPartD8_State_PartDCVolleyFire;
}
STATE(BossFPartD8_State_PartDCVolleyFire) {
    BossFPartD8 *w = (BossFPartD8 *)p_;
    u16 p;
    BossFPartDC *o;
    s32 x, y, wd;
    BossFPartD8_SetAnimation(w, 0x27, 0x1000, 1);
    w->unk442 = 0;
    p = 0;
    if (w->unk458 == 2) {
        p |= 1;
        p |= data_ov000_02365e20[GetLevel()][0] << 1;
    }
    o = (BossFPartDC *)MapObject_Spawn(0xdc, 0, 0, p);
    wd = data_ov000_02365e60[GetLevel()][w->unk458][w->unk440];
    x = gPlayers->x - 0x2000 + ((Rand() & 0x3f) << 8);
    if (w->unk43Cw != 0)
        x += wd >> 1;
    else
        x -= wd >> 1;
    if (w->unk43Cw != 0)
        y = x - wd;
    else
        y = x + wd;
    if (x < 0x1800)
        x = 0x1800;
    else if (x > 0x32800)
        x = 0x32800;
    if (y < 0x1800)
        y = 0x1800;
    else if (y > 0x32800)
        y = 0x32800;
    if (o != NULL)
        BossFPartD8_InitPartDCPath(o, x, y, data_ov000_02365da0[GetLevel()][w->unk458][w->unk440]);
    w->func = BossFPartD8_State_PartDCVolleySetInterval;
}
STATE(BossFPartD8_State_PartDCVolleySetInterval) {
    BossFPartD8 *w = (BossFPartD8 *)p_;
    if (w->unk458 == 2) {
        w->unk442 = data_ov000_02365e20[GetLevel()][0];
        w->unk442 += 0x3c;
    } else {
        w->unk442 = (u16)FX_DivS32(data_ov000_02365e60[GetLevel()][w->unk458][w->unk440],
                                   data_ov000_02365da0[GetLevel()][w->unk458][w->unk440]);
        w->unk442 += data_ov000_02365e00[GetLevel()][w->unk458][w->unk440];
        if (w->unk440 + 1 == data_ov000_02365c5c[GetLevel()][w->unk458])
            w->unk442 += 0x3c;
    }
    w->func = BossFPartD8_State_PartDCVolleyInterval;
    w->func(w);
}
STATE(BossFPartD8_State_PartDCVolleyInterval) {
    BossFPartD8 *w = (BossFPartD8 *)p_;
    BossFPartD8_TrackPlayerX_NearSlow(w);
    if ((u16)(s16)(gPlayers->anim - 0xf) <= 1)
        w->unk434w = 1;
    if (w->unk442-- != 0)
        return;
    s_bld_mode = 0; /* gDisplayMain.unk20 &= ~0xc0 */
    w->unk440++;
    w->unk43Cw = Rand() & 1;
    if (w->unk440 < data_ov000_02365c5c[GetLevel()][w->unk458])
        w->func = BossFPartD8_State_PartDCVolleyFire;
    else
        w->func = BossFPartD8_State_PartDCVolleyEnd;
}
STATE(BossFPartD8_State_PartDCVolleyEnd) {
    D8;
    BossFPartD8_SetAnimation(s, 0x28, 0x1000, 0);
    s->func = BossFPartD8_State_PartDCVolleyEndWait;
    s->func(s);
}
STATE(BossFPartD8_State_PartDCVolleyEndWait) {
    D8;
    if ((u16)(s16)(gPlayers->anim - 0xf) <= 1)
        s->unk434w = 1;
    BossFPartD8_TrackPlayerX_NearSlow(s);
    if (!(s->unk57C & 2))
        return;
    if (s->unk434w != 0) {
        BossF_StartNextAttack();
        return;
    }
    BossFPartD8_TriggerPartDAThrow(BossFPartD8_RollThrowVariant());
}

/* ------------------------------------------------ beam balls (0x37) */
STATE(BossFPartD8_State_PartDEAttackStart) {
    D8;
    BossFPartD8_SetAnimation(s, 0x29, 0x1000, 0);
    s->func = BossFPartD8_State_PartDEAttackWindup;
}
STATE(BossFPartD8_State_PartDEAttackWindup) {
    D8;
    BossFPartD8_TrackPlayerX_Near(s);
    if (s->unk470.anm[0] != NULL && s->unk470.anm[0]->frame == 0x24000) /* *unk554 */
        Sound_PlaySeqArc(0, 0, 0xbf);
    if (s->unk470.anm[0] != NULL && s->unk470.anm[0]->frame == 0x3c000)
        Sound_PlaySeqArc(0, 0, 0xbf);
    if (s->unk57C & 2)
        s->func = BossFPartD8_State_PartDEAttackStartDelay;
}
STATE(BossFPartD8_State_PartDEAttackStartDelay) {
    D8;
    s->h.vel_x = 0;
    s->unk43A = 0;
    s->func = BossFPartD8_State_PartDEAttackDelay;
}
STATE(BossFPartD8_State_PartDEAttackDelay) {
    D8;
    s->unk43A++;
    if (s->unk43A != 0)
        s->func = BossFPartD8_State_PartDEAttackSpawn;
}
STATE(BossFPartD8_State_PartDEAttackSpawn) {
    D8;
    BossFPartD8_SetAnimation(s, 0x2a, 0x1000, 1);
    MapObject_Spawn(0xde, gPlayers->x, gPlayers->y - 0x8000, (u16)s->unk458);
    Sound_PlayVoice(0, 0, 0xd1);
    s->func = BossFPartD8_State_PartDEAttackWait;
}
STATE(BossFPartD8_State_PartDEAttackWait) {
    BossFPartD8 *p = (BossFPartD8 *)p_;
    if ((u16)(s16)(gPlayers->anim - 0xf) <= 1)
        p->unk434w = 1;
    if (p->unk438 == 0)
        p->func = BossFPartD8_State_PartDEAttackEnd;
}
STATE(BossFPartD8_State_PartDEAttackEnd) {
    D8;
    BossFPartD8_SetAnimation(s, 0x2b, 0x1000, 0);
    s->func = BossFPartD8_State_PartDEAttackEndWait;
}
STATE(BossFPartD8_State_PartDEAttackEndWait) {
    D8;
    if ((s->unk57C & 2) == 0)
        return;
    if (s->unk434w != 0) {
        BossF_StartNextAttack();
        return;
    }
    BossFPartD8_TriggerPartDAThrow(BossFPartD8_RollThrowVariant());
}

/* ------------------------------------------------ hurt (0x3a) */
STATE(BossFPartD8_State_Hurt) {
    D8;
    s32 t0;
    s->unk434 = (u16)(s->unk434 + 1);
    t0 = s->unk434;
    if ((s->unk434 & 1) == 0) {
        if ((t0 & 0x8000) != 0)
            BossF_LoadModelPalettesAll(gGameState, 0);
        else
            BossF_LoadModelPalettesAll(gGameState, 1);
        s->unk434 &= ~1;
        s->unk434 ^= 0x8000;
    }
    if ((gPlayers->pf & 0x400) == 0) {
        if (gPlayers->y > -0x6400)
            gPlayers->y -= 0x800;
    }
    if ((s->unk57C & 2) == 0)
        return;
    if ((s->unk434 & 0x8000) == 0)
        s->func = BossFPartD8_State_HurtStartRecover;
}
STATE(BossFPartD8_State_HurtStartRecover) {
    D8;
    BossFPartD8_SetAnimation(s, 6, 0x1000, 1);
    if ((gPlayers->pf & 0x400) == 0) {
        gPlayers->x = 0x1a000;
        gPlayers->y = 0x13e00;
    }
    BossF_InitCamera();
    s->unk434 = 0;
    s->func = BossFPartD8_State_HurtRecover;
}
STATE(BossFPartD8_State_HurtRecover) {
    D8;
    s->unk434 = (u16)(s->unk434 + 1);
    if ((u32)s->unk434 <= 0x3c)
        return;
    if ((gPlayers->pf & 0x400) == 0) {
        gPlayers->move_flags |= 0x80;
        Player_Action_GroundIdle(gPlayers);
    }
    gStageMapCollision_unk24 = 0x340;
    BossFPartD8_Action_ReturnHome(s);
}
STATE(BossFPartD8_State_HurtInSky) {
    D8;
    s32 t0;
    s->unk434 = (u16)(s->unk434 + 1);
    t0 = s->unk434;
    if ((s->unk434 & 1) == 0) {
        if ((t0 & 0x8000) != 0)
            BossF_LoadModelPalettesAll(gGameState, 0);
        else
            BossF_LoadModelPalettesAll(gGameState, 1);
        s->unk434 &= ~1;
        s->unk434 ^= 0x8000;
    }
    s->h.z -= 0x200;
    if (s->h.z < -0x1f400)
        s->h.z = -0x1f400;
    if ((s->unk57C & 2) == 0)
        return;
    if ((s->unk434 & 0x8000) != 0)
        return;
    BossF_StartNextAttack();
}

/* ------------------------------------------------ the sky phase (0x38): last hit point */
STATE(BossFPartD8_State_SkyChargeStartWait) {
    D8;
    s->unk454 = 0;
    s->func = BossFPartD8_State_WaitTimer_ov000_022f0f04;
}
STATE(BossFPartD8_State_WaitTimer_ov000_022f0f04) {
    D8;
    if (s->unk454++ > 0xb4)
        s->func = BossFPartD8_State_SkyChargeStartFadeOut;
}
/* BossFPartD8_State_SkyChargeStartFadeOut (modified: BLDCNT/BLDY state; NNS_SndPlayerStopSeq(BGM, 60) ->
 * music_stop) */
STATE(BossFPartD8_State_SkyChargeStartFadeOut) {
    BossFPartD8 *p = (BossFPartD8 *)p_;
    bld_set(2, 0); /* unk20.raw = 0; bits.a (BG0) = 1; bits.b (mode) = 2; unk24.c = 0 */
    NNS_SndPlayerStopSeqAll();
    p->unk454 = 0;
    p->func = BossFPartD8_State_SkyChargeFadeOut;
    s_st.white_outs++;
}
STATE(BossFPartD8_State_SkyChargeFadeOut) {
    BossFPartD8 *p = (BossFPartD8 *)p_;
    bld_evy_add(2); /* data_02089250.c += 2 */
    if (s_bld_evy >= 0x10)
        p->func = BossFPartD8_State_LoadSkyModel;
}
/* BossFPartD8_State_LoadSkyModel (modified: the sky and passage resources stay loaded until bossf_unload;
 * the pointers are cleared as on the DS, so ZoneFx stops drawing the passage) */
STATE(BossFPartD8_State_LoadSkyModel) {
    D8;
    BossF *b = gBossF;
    BossModel *m = &CAM.unkE0; /* data_ov000_0236a524 */
    s->unk454 = 0;
    boss_model_release(m);
    b->unk238 = NULL; /* NNS_G3dResDefaultRelease + free */
    b->unk240 = NULL;
    b->unk23C = &s_sky_b; /* File_Load("/mod/bsf_sky_b.nsbmd") */
    Model_Init(m, 0);
    Model_SetResource(m, b->unk23C);
    m->scale[0] = m->scale[1] = m->scale[2] = 0x1355b50;
    s->func = BossFPartD8_State_WaitTimer_ov000_022f0c8c;
}
STATE(BossFPartD8_State_WaitTimer_ov000_022f0c8c) {
    D8;
    if (s->unk454++ > 0x78)
        s->func = BossFPartD8_State_SkyChargeStartFadeIn;
}
STATE(BossFPartD8_State_SkyChargeStartFadeIn) {
    D8;
    s->unk454 = 0;
    s->func = BossFPartD8_State_SkyChargeFadeIn;
}
STATE(BossFPartD8_State_SkyChargeFadeIn) {
    BossFPartD8 *w = (BossFPartD8 *)p_;
    w->unk454++;
    if (w->unk454 <= 6)
        return;
    w->unk454 = 0;
    bld_evy_add(-1);
    if (s_bld_evy == 0) {
        s_bld_mode = 0;
        w->func = BossFPartD8_State_StartWaitTalkDemo;
    }
}
STATE(BossFPartD8_State_StartWaitTalkDemo) {
    D8;
    s->unk454 = 0;
    s->func = BossFPartD8_State_WaitTalkDemo;
}
/* BossFPartD8_State_WaitTalkDemo (modified: Sound_PlaySeq -> music hook; TalkDemo_Create -> talk hook) */
STATE(BossFPartD8_State_WaitTalkDemo) {
    BossFPartD8 *p = (BossFPartD8 *)p_;
    s32 id;
    if (p->unk454++ <= 0x78)
        return;
    Sound_PlaySeq(0x13);
    gBossF->unk20C |= 1;
    if (gGameState == 0)
        id = 0xf; /* gTalkDemoId */
    else
        id = 0x10;
    TalkDemo_Create(id);
    p->func = BossFPartD8_State_SkyChargeStartRetreat;
}
STATE(BossFPartD8_State_SkyChargeStartRetreat) {
    D8;
    BossFPartD8_SetAnimation(s, 0x20, 0x1000, 0);
    s->unk440w = s->h.x << 4;
    s->unk444 = s->h.y << 4;
    s->unk448 = s->h.z << 4;
    s->unk44C = 0;
    CAM.unkC0 = 0x258000; /* c->unk78 */
    CAM.unkCC = 0x14000;  /* c->unk84 */
    s->func = BossFPartD8_State_SkyChargeRetreat;
}
STATE(BossFPartD8_State_SkyChargeRetreat) {
    BossFPartD8 *w = (BossFPartD8 *)p_;
    w->unk44C += 0x2d;
    if (w->unk44C >= 0x1000) {
        w->unk44C = 0x1000;
        w->func = BossFPartD8_State_SkyChargeStartWait1;
    }
    w->h.x = (w->unk440w + FX_Mul(0x1a0000 - w->unk440w, w->unk44C)) >> 4;
    w->h.y = (w->unk444 + FX_Mul(0x358000 - w->unk444, w->unk44C)) >> 4;
    w->h.z = (w->unk448 + FX_Mul(-0x7d0000 - w->unk448, w->unk44C)) >> 4;
}
STATE(BossFPartD8_State_SkyChargeStartWait1) {
    D8;
    s->unk454 = 0;
    s->func = BossFPartD8_State_WaitTimer_ov000_022f09b4;
}
STATE(BossFPartD8_State_WaitTimer_ov000_022f09b4) {
    D8;
    if (s->unk454++ > 0x3c) {
        s->unk454 = 0;
        s->func = BossFPartD8_State_SkyChargeStart1;
    }
}
STATE(BossFPartD8_State_SkyChargeStart1) {
    D8;
    BossFPartD8_SetAnimation(s, 0x21, 0x1000, 1);
    Sound_PlaySeqArc(0, 0, 0xc7);
    Sound_PlayVoice(0, 0, 0xd1);
    s->func = BossFPartD8_State_SkyCharge1;
}
STATE(BossFPartD8_State_SkyCharge1) {
    BossFPartD8 *w = (BossFPartD8 *)p_;
    s16 t;
    w->h.vel_z = 0x2000;
    if (w->h.z >= -0x12c00) {
        w->h.z = -0x12c00;
        w->func = BossFPartD8_State_SkyChargeImpact1;
    }
    t = (s16)FX_Div(((w->h.z >> 8) + 0x7d0) << 12, 0x6a4000);
    CAM.unkC0 = Mul64(t, -0x12c000) + 0x258000;
}
STATE(BossFPartD8_State_SkyChargeImpact1) {
    BossFPartD8 *w = (BossFPartD8 *)p_;
    w->h.vel_z = 0;
    w->unk57C |= 1;
    w->unk450 = 0x100;
    Sound_PlaySeqArc(0, 0, 0xc9);
    w->unk452 = 0;
    bld_set(2, 8); /* raw 0, b0 (BG0) 1, b6 (mode) 2, unk24.b0 8 */
    gBossF->unk20C |= 2;
    w->unk454 = 0;
    w->func = BossFPartD8_State_SkyChargeShake1;
}
STATE(BossFPartD8_State_SkyChargeShake1) {
    D8;
    s32 t0;
    BossF_UpdateCameraShake(&s->unk450, 4, 8);
    BossFPartD8_StepFlashFade(s, 4);
    t0 = s->unk454;
    s->unk454 = (u16)(s->unk454 + 1);
    if ((u32)t0 > (u32)0x1e)
        s->func = BossFPartD8_State_SkyChargeStartRetreat1;
}
STATE(BossFPartD8_State_SkyChargeStartRetreat1) {
    D8;
    BossFPartD8_SetAnimation(s, 0x20, 0x1000, 0);
    s->unk440w = s->h.x << 4;
    s->unk444 = s->h.y << 4;
    s->unk448 = s->h.z << 4;
    s->unk44C = 0;
    CAM.unkC0 = 0x258000;
    s->func = BossFPartD8_State_SkyChargeRetreat1;
    s->func(s);
}
STATE(BossFPartD8_State_SkyChargeRetreat1) {
    BossFPartD8 *w = (BossFPartD8 *)p_;
    BossF_UpdateCameraShake(&w->unk450, 4, 8);
    BossFPartD8_StepFlashFade(w, 4);
    w->unk44C += 0x2d;
    if (w->unk44C >= 0x1000) {
        w->unk44C = 0x1000;
        w->func = BossFPartD8_State_SkyChargeStartWait2;
    }
    w->h.x = (w->unk440w + FX_Mul(0x1a0000 - w->unk440w, w->unk44C)) >> 4;
    w->h.y = (w->unk444 + FX_Mul(0x358000 - w->unk444, w->unk44C)) >> 4;
    w->h.z = (w->unk448 + FX_Mul(-0x9c4000 - w->unk448, w->unk44C)) >> 4;
}
STATE(BossFPartD8_State_SkyChargeStartWait2) {
    D8;
    s->unk454 = 0;
    s->func = BossFPartD8_State_WaitTimer_ov000_022f061c;
}
STATE(BossFPartD8_State_WaitTimer_ov000_022f061c) {
    D8;
    if (s->unk454++ > 0x14)
        s->func = BossFPartD8_State_SkyChargeStart2;
}
STATE(BossFPartD8_State_SkyChargeStart2) {
    D8;
    BossFPartD8_SetAnimation(s, 0x21, 0x1000, 1);
    Sound_PlaySeqArc(0, 0, 0xc7);
    Sound_PlayVoice(0, 0, 0xd0);
    s->func = BossFPartD8_State_SkyCharge2;
}
STATE(BossFPartD8_State_SkyCharge2) {
    BossFPartD8 *w = (BossFPartD8 *)p_;
    s16 t;
    w->h.vel_z = 0x2800;
    if (w->h.z >= -0x12c00) {
        w->h.z = -0x12c00;
        w->func = BossFPartD8_State_SkyChargeImpact2;
    }
    t = (s16)FX_Div((0x9c4 + (w->h.z >> 8)) << 12, 0x898000);
    CAM.unkC0 = Mul64(t, -0x12c000) + 0x258000;
}
STATE(BossFPartD8_State_SkyChargeImpact2) {
    BossFPartD8 *w = (BossFPartD8 *)p_;
    w->h.vel_z = 0;
    w->unk450 = 0x100;
    w->unk57C |= 1;
    Sound_PlaySeqArc(0, 0, 0xc9);
    w->unk452 = 0;
    bld_set(2, 0xc);
    gBossF->unk20C |= 4;
    w->unk454 = 0;
    w->func = BossFPartD8_State_SkyChargeShake2;
}
STATE(BossFPartD8_State_SkyChargeShake2) {
    D8;
    s32 t0;
    BossF_UpdateCameraShake(&s->unk450, 4, 8);
    BossFPartD8_StepFlashFade(s, 8);
    t0 = s->unk454;
    s->unk454 = (u16)(s->unk454 + 1);
    if ((u32)t0 > (u32)0x1e)
        s->func = BossFPartD8_State_SkyChargeStartRetreat2;
}
STATE(BossFPartD8_State_SkyChargeStartRetreat2) {
    D8;
    BossFPartD8_SetAnimation(s, 0x20, 0x1000, 0);
    s->unk440w = s->h.x << 4;
    s->unk444 = s->h.y << 4;
    s->unk448 = s->h.z << 4;
    s->unk44C = 0;
    CAM.unkC0 = 0x258000;
    s->func = BossFPartD8_State_SkyChargeRetreat2;
    s->func(s);
}
STATE(BossFPartD8_State_SkyChargeRetreat2) {
    BossFPartD8 *w = (BossFPartD8 *)p_;
    BossF_UpdateCameraShake(&w->unk450, 4, 8);
    BossFPartD8_StepFlashFade(w, 8);
    w->unk44C += 0x2d;
    if (w->unk44C >= 0x1000) {
        w->unk44C = 0x1000;
        w->func = BossFPartD8_State_SkyChargeStartWait3;
    }
    w->h.x = (w->unk440w + FX_Mul(0x1a0000 - w->unk440w, w->unk44C)) >> 4;
    w->h.y = (w->unk444 + FX_Mul(0x358000 - w->unk444, w->unk44C)) >> 4;
    w->h.z = (w->unk448 + FX_Mul(-0xfa0000 - w->unk448, w->unk44C)) >> 4;
}
STATE(BossFPartD8_State_SkyChargeStartWait3) {
    D8;
    s->unk454 = 0;
    s->func = BossFPartD8_State_WaitTimer_ov000_022f0280;
}
STATE(BossFPartD8_State_WaitTimer_ov000_022f0280) {
    D8;
    if (s->unk454++ > 0x3c)
        s->func = BossFPartD8_State_SkyChargeStart3;
}
/* BossFPartD8_State_SkyChargeStart3 (modified: the effect task pointer kept natively, see s->tackle) */
STATE(BossFPartD8_State_SkyChargeStart3) {
    D8;
    s->tackle = BossEffect_CreateTackle(0, 0x1a000, 0x30000, 0);
    s->unk434w = 1; /* *(u32 *)(p0 + 0x434) = the task */
    s->tackle->h.disp_flags |= 4;
    s->tackle->unkC = 0x270f;
    Sound_PlayVoice(0, 0, 0xd2);
    BossFPartD8_SetAnimation(s, 0x21, 0x1000, 1);
    Sound_PlaySeqArc(0, 0, 0xc7);
    s->func = BossFPartD8_State_SkyCharge3;
}
STATE(BossFPartD8_State_SkyCharge3) {
    BossFPartD8 *w = (BossFPartD8 *)p_;
    s16 t;
    w->h.vel_z = 0x3000;
    if (w->h.z >= -0x12c00) {
        w->h.z = -0x12c00;
        w->func = BossFPartD8_State_SkyChargeImpact3;
    }
    t = (s16)FX_Div(((w->h.z >> 8) + 0xfa0) << 12, 0xe74000);
    CAM.unkC0 = Mul64(t, -0x12c000) + 0x258000;
}
STATE(BossFPartD8_State_SkyChargeImpact3) {
    BossFPartD8 *w = (BossFPartD8 *)p_;
    w->h.vel_z = 0;
    w->unk454 = 0;
    w->unk456 = 0;
    BossF_InitCamera();
    if (w->tackle != NULL)
        w->tackle->h.obj_flags |= 8; /* w->unk434->unk54 |= 8 */
    w->tackle = NULL;
    w->unk57C |= 1;
    w->unk450 = 0x100;
    Sound_PlaySeqArc(0, 0, 0xc9);
    w->unk452 = 0;
    bld_set(2, 0x10);
    gBossF->unk20C |= 8;
    g_dcp_world.flags |= 1; /* gGameState.unk18: the stage timer runs again */
    w->func = BossFPartD8_State_SkyChargeShake3;
}
STATE(BossFPartD8_State_SkyChargeShake3) {
    D8;
    BossFPartD8_StepFlashFade(s, 0x10);
    s->unk450 = 0x100;
    BossF_UpdateCameraShake(&s->unk450, 4, 8);
    if (s->unk454 > 0xb4) {
        if (!(s->coll[0].flags & 4)) { /* unk216 */
            BossFPartD8_InitHitboxes(s);
            BossFPartD8_DisableHitboxes(s);
            s->coll[0].flags |= 4;
        }
        s->unk456++;
        if (!(s->unk456 & 3)) {
            if (s->unk456 & 0x8000)
                BossF_LoadPalette(gGameState, 0);
            else
                BossF_LoadPalette(gGameState, 1);
            s->unk456 &= ~3;
            s->unk456 ^= 0x8000;
        }
    }
    if (data_ov000_02363dd4[GetIdxB()] >= s->unk454++)
        return;
    if (!(s->unk456 & 0x8000))
        s->func = BossFPartD8_State_SkyChargeDestroyBase;
}
/* BossFPartD8_State_SkyChargeDestroyBase (modified: BLDCNT state) */
STATE(BossFPartD8_State_SkyChargeDestroyBase) {
    D8;
    BossF *b = gBossF;
    BossFPartD8_InitHitboxes(s);
    BossFPartD8_DisableHitboxes(s);
    BossF_LoadPalette(gGameState, 0);
    Model_SetAnimation(&b->unk244, 0, b->unk224, 2);
    Model_SetAnimation(&b->unk244, 4, b->unk228, 1);
    BossEffect_CreateBaseDest(0, 0x1a000, 0x30000, 0);
    s->h.vel_z = 0x800;
    gPlayers->obj_flags &= ~1u; /* the player leaves collision layer 1: the base floor is gone */
    bld_set(2, 0x10);
    s->unk438w = gPlayers->z;
    s->unk43C = 0x1000;
    b->unk20C &= ~0x10;
    CAM.t[1].oz = NULL; /* data_ov000_0236a444.unk8C */
    Sound_PlaySeqArc(0, 0, 0xca);
    s->unk454 = 0;
    s->func = BossFPartD8_State_SkyChargeAftermath;
    boss_log("base_destroyed", 0, 0);
}
STATE(BossFPartD8_State_SkyChargeAftermath) {
    BossFPartD8 *w = (BossFPartD8 *)p_;
    if (s_bld_evy != 0 && !(w->unk454++ & 1)) {
        bld_evy_add(-1);
        if (s_bld_evy == 0)
            s_bld_mode = 0;
    }
    if ((s16)w->unk43C > 0)
        w->unk43C = (u16)((s16)w->unk43C - 0x60);
    if ((s16)w->unk43C < 0)
        w->unk43C = 0;
    w->unk438w += (s16)w->unk43C;
    gPlayers->z = w->unk438w;
    w->h.vel_z = (s16)(w->h.vel_z - 0x60);
    if (w->h.vel_z < 0)
        w->h.vel_z = 0;
}

/* ------------------------------------------------ defeat (0x3b) */
/* BossFPartD8_State_DefeatCamera1 (modified: BG2 cut-in -> Boss_DrawExcBackground; brightness state) */
STATE(BossFPartD8_State_DefeatCamera1) {
    D8;
    CAM.unkDC = 2;
    CAM.t[0].lerp = 0x1000;                          /* c->unk30 */
    CAM.t[0].accel = 0;                              /* c->unk38 */
    CAM.t[1].ox = CAM.t[1].oy = CAM.t[1].oz = NULL;  /* c->unk3C..44 */
    CAM.t[1].offx = CAM.t[1].offy = CAM.t[1].offz = 0; /* c->unk48..50 */
    CAM.t[1].tgt.x = gPlayers->x << 4;               /* c->unk54 */
    CAM.t[1].tgt.y = -gPlayers->y << 4;
    CAM.t[1].tgt.z = gPlayers->z << 4;
    CAM.t[1].lerp = 0x1000;                          /* c->unk6C */
    CAM.t[1].accel = 0;                              /* c->unk74 */
    CAM.unkC0 = 0x60000;                             /* c->unk78 */
    CAM.unkC8 = 0x1000;                              /* c->unk80 */
    CAM.unkCC = 0x20000;                             /* c->unk84 */
    CAM.unkD4 = 0x1000;                              /* c->unk8C */
    CAM.unkD6 = 0x2000;                              /* c->unk8E */
    CAM.unkDA = 0x1000;                              /* c->unk92 */
    Boss_DrawExcBackground(6);
    g_boss_game.bright = 0x10;
    Sound_PlaySeqArc(0, 0, 0x7c);
    s->unk434 = 0;
    s->func = BossFPartD8_State_DefeatFade_ov000_022ef664;
}
STATE(BossFPartD8_State_DefeatCamera2) {
    D8;
    CAM.unkC0 = 0xa0000;
    CAM.unkC8 = 0x1000;
    CAM.unkCC = -0x10000;
    CAM.unkD4 = 0x1000;
    CAM.unkD6 = 0xe000;
    CAM.unkDA = 0x1000;
    Boss_DrawExcBackground(7);
    g_boss_game.bright = 0x10;
    Sound_PlaySeqArc(0, 0, 0x7c);
    s->unk434 = 0;
    s->func = BossFPartD8_State_DefeatFade_ov000_022ef5a4;
}
STATE(BossFPartD8_State_DefeatCamera3) {
    D8;
    CAM.unkC0 = 0x60000;
    CAM.unkC8 = 0x1000;
    CAM.unkCC = 0;
    CAM.unkD4 = 0x1000;
    CAM.unkD6 = 0;
    CAM.unkDA = 0x1000;
    Boss_DrawExcBackground(6);
    g_boss_game.bright = 0x10;
    Sound_PlaySeqArc(0, 0, 0x7c);
    s->unk434 = 0;
    s->func = BossFPartD8_State_DefeatFade_ov000_022ef4ec;
}
/* BossFPartD8_State_DefeatFade_* (copied) */
static void defeat_fade(BossFPartD8 *s, BossFn next) {
    s32 t1;
    if (g_boss_game.bright > 0)
        g_boss_game.bright = (s16)(g_boss_game.bright - 2);
    t1 = (s16)s->unk434;
    s->unk434 = (u16)(s->unk434 + 1);
    if (t1 > 0x1e)
        s->func = next;
}
STATE(BossFPartD8_State_DefeatFade_ov000_022ef4ec) { defeat_fade((BossFPartD8 *)p_, BossFPartD8_State_DefeatExplode); }
STATE(BossFPartD8_State_DefeatFade_ov000_022ef5a4) { defeat_fade((BossFPartD8 *)p_, BossFPartD8_State_DefeatCamera3); }
STATE(BossFPartD8_State_DefeatFade_ov000_022ef664) { defeat_fade((BossFPartD8 *)p_, BossFPartD8_State_DefeatCamera2); }
/* BossFPartD8_State_DefeatExplode (modified: DISPCNT BG2 off -> exc_bg -1) */
STATE(BossFPartD8_State_DefeatExplode) {
    D8;
    BossF *b = gBossF;
    BossFPartD8_SetAnimation(s, 0x25, 0x1000, 0);
    CAM.t[1].tgt.x = 0x19e000;        /* c->unk54 */
    CAM.t[1].tgt.y = (s32)0xffd77000; /* c->unk58 */
    CAM.t[1].tgt.z = 0;               /* c->unk5C */
    CAM.t[1].lerp = 0x1000;           /* c->unk6C */
    CAM.unkC0 = 0x320000;             /* c->unk78 */
    CAM.unkC8 = 0x1000;               /* c->unk80 */
    CAM.unkCC = -0x20000;             /* c->unk84 */
    CAM.unkD4 = 0x1000;               /* c->unk8C */
    CAM.unkD6 = 0xfaab;               /* c->unk8E */
    CAM.unkDA = 0x1000;               /* c->unk92 */
    Boss_DrawExcBackground((u32)-1);  /* GX_SetVisiblePlane(.. & ~BG2) */
    g_boss_game.exc_bg = -1;
    b->h.obj_flags &= ~0x20;
    s->h.obj_flags &= ~0x20;
    b->unk244.speed = 0x1000; /* b->unk358 */
    s->unk584 = 0x1000;
    StageObj_ClearPause();
    gPlayers->disp_flags |= 0x20;
    gPlayers->pf |= 0x200000;
    gPlayers->released = 0;
    gPlayers->pressed = gPlayers->released;
    gPlayers->held = gPlayers->pressed;
    BossEffect_CreateBlastF(0, s->h.x, s->h.y, s->h.z);
    NaviMsg_SetFaceAnim(0xd);
    NaviMsg_ShowMessage(0x13, 0xf0);
    g_boss_game.bright = 0x10;
    Sound_PlaySeqArc(0, 0, 0x7d);
    s->unk434 = 0;
    s->func = BossFPartD8_State_DefeatFinish;
}
/* BossFPartD8_State_DefeatFinish (modified: StageShutdown_Create(Stage_OnBossCleared) -> cleared hook, once) */
STATE(BossFPartD8_State_DefeatFinish) {
    BossFPartD8 *w = (BossFPartD8 *)p_;
    w->unk434 = (u16)(w->unk434 + 1);
    if ((s16)w->unk434 >= 0 && (w->unk434 & 3) == 0) {
        if (g_boss_game.bright > 0)
            g_boss_game.bright--;
    }
    if (!(w->unk57C & 2))
        return;
    if ((s16)w->unk434 <= 0x12c)
        return;
    w->h.obj_flags |= 4;
    g_obj_stage.ring_bonus = gPlayers->rings * 1000; /* data_022c4590.unk10 */
    if (!g_boss_game.cleared) {
        g_boss_game.cleared = 1;
        boss_log("cleared", gPlayers->rings, 0);
        if (g_boss_hooks.cleared)
            g_boss_hooks.cleared(g_boss_hooks.ctx);
    }
}

/* ============================================================ BossFPartD9: bomb ball */
STATE(BossFPartD9_State_StartDelay);
STATE(BossFPartD9_State_Delay);
STATE(BossFPartD9_State_StartFly);
STATE(BossFPartD9_State_Fly);
STATE(BossFPartD9_State_Roll);

/* BossFPartD9_State_Init (copied) */
STATE(BossFPartD9_State_Init) {
    BossFPartD9 *s = (BossFPartD9 *)p_;
    BossEffect_CreateProduce(1, s->h.x, s->h.y, s->h.z + 0x800);
    s->func = BossFPartD9_State_StartDelay;
}
/* BossFPartD9_State_StartDelay / Delay / StartFly (copied) */
STATE(BossFPartD9_State_StartDelay) {
    BossFPartD9 *s = (BossFPartD9 *)p_;
    s->h.timer = 0; /* unkC */
    s->func = BossFPartD9_State_Delay;
}
STATE(BossFPartD9_State_Delay) {
    BossFPartD9 *s = (BossFPartD9 *)p_;
    s->h.timer++;
    if (s->h.timer > 10)
        s->func = BossFPartD9_State_StartFly;
}
STATE(BossFPartD9_State_StartFly) {
    BossFPartD9 *s = (BossFPartD9 *)p_;
    s->unk21C = 0;
    s->func = BossFPartD9_State_Fly;
}
/* BossFPartD9_State_Fly (copied: an arc from the core to the arm, then it rolls) */
STATE(BossFPartD9_State_Fly) {
    BossFPartD9 *s = (BossFPartD9 *)p_;
    const fx32 *m = gBossF->unk218->unk340;
    s32 v;
    s32 k;
    s32 cur;
    s32 tgt;
    s32 i;
    s->unk20C = (u16)(s->unk20C - (s->h.vel_x << 1));
    v = s->h.vel_x;
    if (v < 0)
        v = -v;
    s->unk21C = (u16)(s->unk21C + FX_Mul(v << 4, 0x20));
    if (s->unk21C >= 0x1000) {
        s->unk21C = 0x1000;
        s->h.move_flags |= 0x90;
        s->h.move_flags &= ~0x2200;
        s->func = BossFPartD9_State_Roll;
    }
    s->unk20E = (u16)((FX_Mul(s->unk21C, 0x2000000) - 0x2000000) >> 12);
    k = s->unk21C;
    cur = s->unk210;
    tgt = m[9] - 0x50000;
    i = 2;
    do {
        cur += Mul64(tgt - cur, k);
    } while (i-- != 0);
    s->h.x = cur >> 4;
    s->h.y = -(s->unk214 + FX_Mul(m[10] + 0x40000 - s->unk214, s->unk21C)) >> 4;
    s->h.z = (s->unk218 + FX_Mul(-s->unk218, s->unk21C)) >> 4;
}
/* BossFPartD9_State_Roll (copied: rolls while in the air or on the arm, and while the arm lies there) */
STATE(BossFPartD9_State_Roll) {
    BossFPartD9 *w = (BossFPartD9 *)p_;
    BossF *m = gBossF;
    w->unk20C = (u16)(w->unk20C - w->h.vel_x * 2);
    if (w->h.move_flags & 1)
        w->h.move_flags &= ~0x10;
    else
        w->h.move_flags |= 0x10;
    if ((!(w->h.move_flags & 1) || w->h.ride != NULL) && (m->unk20C & 0x40) && !(w->h.obj_flags & 8))
        return;
    w->h.obj_flags |= 4;
    BossEffect_CreateBombBall(1, w->h.x, w->h.y, w->h.z);
}
/* BossFPartD9_OnHit (modified: owner type -> obj_box_player) */
static void BossFPartD9_OnHit(ObjBox *a, ObjBox *b) {
    DcPlayer *p;
    DcPlayer *w;
    s16 v;
    p = obj_box_player(b);
    w = (DcPlayer *)a->owner;
    if (p == NULL || p->type != 1)
        return;
    if (p->anim == 0x30) {
        w->obj_flags |= 8;
        Player_Action_AttackBounce(p);
        a->flags &= ~4; /* w->coll0.unkA */
        return;
    }
    if (p->anim == 0x31) {
        if (w->x < p->x) {
            p->push_x = (s16)(((a->x + a->r - b->l) << 8) - p->x);
            v = 0x300;
        } else {
            p->push_x = (s16)(((a->x + a->l - b->r) << 8) - p->x);
            v = -0x300;
        }
        p->move_flags |= 4;
        p->vel_x = v;
        p->gnd_vel = v;
        return;
    }
    if (p->invuln == 0)
        Player_OnHurt(b, a);
}
/* BossFPartD9_Draw (copied) */
static void BossFPartD9_Draw(void *p_) {
    BossFPartD9 *w = (BossFPartD9 *)p_;
    BossModel *m = &w->unk228;
    MtxFx33 mtx;
    if (w->h.disp_flags & 0x20)
        return;
    m->trans[0] = w->h.x << 4;
    m->trans[1] = -(w->h.y << 4);
    m->trans[2] = w->h.z << 4;
    if (w->h.disp_flags & 1)
        MTX_RotY33(&m->rot, SIN(0xc000), COS(0xc000)); /* data_0206e470[0x1800] */
    else
        MTX_RotY33(&m->rot, SIN(0x4000), COS(0x4000)); /* [0x800] */
    MTX_RotZ33(&mtx, SIN(w->unk20C), COS(w->unk20C));
    MTX_Concat33(&m->rot, &mtx, &m->rot);
    MTX_RotY33(&mtx, SIN(w->unk20E), COS(w->unk20E));
    MTX_Concat33(&m->rot, &mtx, &m->rot);
    Model_Animate(m);
    Model_Draw(m);
}
static void BossFPartD9_Destructor(void *p_) {
    BossFPartD9 *s = (BossFPartD9 *)p_;
    boss_model_release(&s->unk228);
    boxes_forget(s->coll, 3);
}
static void BossFPartD9_Collide(void *p_) { collide_default(&((BossFPartD9 *)p_)->h, ((BossFPartD9 *)p_)->coll); }
/* BossFPartD9_Create (modified: resources from BossStageF) */
static BossFPartD9 *BossFPartD9_Create(const ObjParam *p, s32 b, s32 c) {
    BossFPartD9 *w = NULL;
    const fx32 *r = gBossF->unk218->unk340;
    ObjBox *coll;
    BossModel *sub;
    s16 v;
    int i;
    for (i = 0; i < BF_D9; i++)
        if (!s_d9[i].alive) {
            w = &s_d9[i];
            break;
        }
    if (w == NULL)
        return NULL;
    boss_model_release(&w->unk228);
    memset(w, 0, sizeof(*w));
    w->alive = 1;
    w->func = BossFPartD9_State_Init;
    w->unk20E = 0xe000;
    w->unk210 = r[9] - 0x30000;
    w->unk214 = r[10] + 0x18000;
    w->unk218 = r[11] + 0x38000;
    w->unk224 = &s_bsf_ca;
    w->unk220 = &data_ov000_02369b88[2];
    MapObject_Init(&w->h, w->coll, p, b, c);
    w->h.obj_flags |= 0x10;
    w->h.move_flags |= 0xa000;
    w->h.move_flags |= 0x200;
    v = data_ov000_02363fbc[GetLevel()][gBossStatus.unk8 - 1];
    v += (Rand() & 7) << 4;
    if (p->flags & 1) {
        w->h.disp_flags |= 1;
        w->h.vel_x = (s16)-v;
    } else {
        w->h.disp_flags &= ~1;
        w->h.vel_x = v;
    }
    w->h.x = w->unk210 >> 4;
    w->h.y = -w->unk214 >> 4;
    w->h.z = w->unk218 >> 4;
    w->h.box_l = -16;
    w->h.box_t = -16;
    w->h.box_r = 16;
    w->h.box_b = 16;
    coll = &w->coll[0];
    rect_set_box2d(coll, -16, -16, 16, 16);
    rect_set_atkdef(coll, 0x11, 0);
    coll->owner = w;
    coll->cb = BossFPartD9_OnHit;
    coll->flags |= 4;
    coll = &w->coll[1];
    rect_set_box2d(coll, -16, -16, 16, 16);
    coll->flags &= ~4;
    sub = &w->unk228;
    Model_Init(sub, 0);
    Model_SetResource(sub, w->unk220);
    Model_SetAnimation(sub, 0, w->unk224, 3);
    sub->scale[0] = sub->scale[1] = sub->scale[2] = 0x34cc;
    w->func(w);
    s_st.balls++;
    return w;
}

/* ============================================================ BossFPartDA / DB: shock wave, slam waves */
/* BossFPartDA_State_Move / BossFPartDB_State_Move (copied) */
STATE(BossFPartDA_State_Move) {
    BossFPartDA *p0 = (BossFPartDA *)p_;
    s32 t0 = p0->h.timer;
    p0->h.timer++;
    if (t0 == 0xf)
        Sound_PlaySeqArc(0, 0, 0xc3);
    if (p0->h.x >= 0 && p0->h.x <= 0x34000)
        return;
    p0->h.obj_flags |= 4;
}
STATE(BossFPartDB_State_Move) {
    BossFPartDA *p0 = (BossFPartDA *)p_;
    s32 t0 = p0->h.timer;
    p0->h.timer++;
    if (t0 == 0xf)
        Sound_PlaySeqArc(0, 0, 0xc2);
    if (p0->h.x >= 0 && p0->h.x <= 0x34000)
        return;
    p0->h.obj_flags |= 4;
}
/* BossFPartDA_Draw / BossFPartDB_Draw (copied) */
static void BossFPartDA_Draw(void *p_) {
    BossFPartDA *obj = (BossFPartDA *)p_;
    BossModel *cam = &obj->unk214;
    if (obj->h.disp_flags & 0x20)
        return;
    cam->trans[0] = obj->h.x << 4;
    cam->trans[1] = -(obj->h.y << 4);
    cam->trans[2] = obj->h.z << 4;
    if (obj->h.disp_flags & 1)
        MTX_RotY33(&cam->rot, SIN(0xc000), COS(0xc000)); /* data_02071470 */
    else
        MTX_RotY33(&cam->rot, SIN(0x4000), COS(0x4000)); /* data_0206f470 */
    Model_Animate(cam);
    Model_Draw(cam);
}
#define BossFPartDB_Draw BossFPartDA_Draw
static void BossFPartDA_Destructor(void *p_) {
    BossFPartDA *s = (BossFPartDA *)p_;
    boss_model_release(&s->unk214);
    boxes_forget(s->coll, 3);
}
#define BossFPartDB_Destructor BossFPartDA_Destructor
static void BossFPartDA_Collide(void *p_) { collide_default(&((BossFPartDA *)p_)->h, ((BossFPartDA *)p_)->coll); }
static BossFPartDA *da_alloc(BossFPartDA *pool, int n) {
    int i;
    for (i = 0; i < n; i++)
        if (!pool[i].alive) {
            boss_model_release(&pool[i].unk214);
            memset(&pool[i], 0, sizeof(pool[i]));
            pool[i].alive = 1;
            return &pool[i];
        }
    return NULL;
}
/* BossFPartDA_Create (modified: resources from BossStageF) */
static BossFPartDA *BossFPartDA_Create(const ObjParam *p, s32 b, s32 c) {
    BossFPartDA *w = da_alloc(s_da, BF_DA);
    ObjBox *coll;
    BossModel *sub;
    if (w == NULL)
        return NULL;
    w->func = BossFPartDA_State_Move;
    w->unk210 = &s_bsf_ca;
    w->unk20C = &data_ov000_02369b88[3];
    MapObject_Init(&w->h, w->coll, p, b, c);
    w->h.obj_flags |= 0x10;
    w->h.move_flags |= 0x1300;
    if (p->flags & 1) {
        w->h.disp_flags |= 1;
        w->h.vel_x = -0x300;
    } else {
        w->h.disp_flags &= ~1;
        w->h.vel_x = 0x300;
    }
    w->h.x = b;
    w->h.y = c;
    w->h.z = 0;
    coll = &w->coll[0];
    coll->flags &= ~4;
    coll = &w->coll[1];
    rect_set_box2d(coll, 0x10, -0x20, 0x40, 0);
    coll->owner = w;
    coll->flags |= 4;
    sub = &w->unk214;
    Model_Init(sub, 0);
    Model_SetResource(sub, w->unk20C);
    Model_SetAnimation(sub, 0, w->unk210, 4);
    sub->flags[0] |= 4;
    sub->scale[0] = sub->scale[1] = sub->scale[2] = 0x34cc;
    s_st.shocks++;
    return w;
}
/* BossFPartDB_Create (modified: resources from BossStageF) */
static BossFPartDA *BossFPartDB_Create(const ObjParam *p, s32 b, s32 c) {
    BossF *boss = gBossF;
    BossFPartDA *w = da_alloc(s_db, BF_DB);
    ObjBox *coll;
    BossModel *sub;
    if (w == NULL)
        return NULL;
    w->func = BossFPartDB_State_Move;
    w->unk210 = &s_bsf_ca;
    w->unk20C = &data_ov000_02369b88[4];
    MapObject_Init(&w->h, w->coll, p, b, c);
    w->h.obj_flags |= 0x10;
    w->h.move_flags |= 0x1300;
    if (p->flags & 2)
        w->h.vel_x = data_ov000_02363e60[GetLevel()][boss->unk218->unk458];
    else
        w->h.vel_x = data_ov000_02364044[GetLevel()][boss->unk218->unk458];
    if (p->flags & 1) {
        w->h.disp_flags |= 1;
        w->h.vel_x = (s16)-w->h.vel_x;
    } else {
        w->h.disp_flags &= ~1;
        w->h.vel_x = (s16)(w->h.vel_x * 1);
    }
    if (p->flags & 4)
        w->h.vel_x >>= 1;
    w->h.x = b;
    w->h.y = c;
    w->h.z = 0;
    coll = &w->coll[0];
    coll->flags &= ~4;
    coll = &w->coll[1];
    rect_set_box2d(coll, 0x10, -0x20, 0x40, 0);
    coll->owner = w;
    coll->flags |= 4;
    sub = &w->unk214;
    Model_Init(sub, 0);
    Model_SetResource(sub, w->unk20C);
    Model_SetAnimation(sub, 0, w->unk210, 5);
    sub->flags[0] |= 4;
    sub->scale[0] = sub->scale[1] = sub->scale[2] = 0x34cc;
    s_st.waves++;
    return w;
}

/* ============================================================ BossFPartDC: laser */
/* NNS_G3dWorldPosToScrPos (modified: through the boss camera, DS screen x 0..255; 0 = on screen) */
static s32 NNS_G3dWorldPosToScrPos(const BossVec *v, s32 *sx, s32 *sy) {
    dcm_camera_t cam;
    float p[3], e[3], o[4];
    int c;
    boss_camera(&cam);
    p[0] = v->x / 4096.0f;
    p[1] = v->y / 4096.0f;
    p[2] = v->z / 4096.0f;
    for (c = 0; c < 3; c++)
        e[c] = p[0] * cam.view.m[0][c] + p[1] * cam.view.m[1][c] + p[2] * cam.view.m[2][c] + cam.view.m[3][c];
    for (c = 0; c < 4; c++)
        o[c] = e[0] * cam.proj[0][c] + e[1] * cam.proj[1][c] + e[2] * cam.proj[2][c] + cam.proj[3][c];
    if (o[3] <= 0)
        return -1;
    *sx = (s32)((o[0] / o[3] * 0.5f + 0.5f) * 256.0f);
    *sy = (s32)((0.5f - o[1] / o[3] * 0.5f) * 192.0f);
    if (o[0] < -o[3] || o[0] > o[3] || o[1] < -o[3] || o[1] > o[3])
        return -1;
    return 0;
}
/* BossFPartDC_FollowBossNode (copied) */
static void BossFPartDC_FollowBossNode(BossFPartDC *p0) {
    const fx32 *t0 = gBossF->unk218->unk340;
    p0->h.x = t0[9] >> 4;
    p0->h.y = -(t0[10] - 0xa000) >> 4;
    p0->h.z = (t0[11] + 0x46000) >> 4;
}
/* BossFPartDC_State_ChasePlayer (modified: BLDY state) */
STATE(BossFPartDC_State_ChasePlayer) {
    BossFPartDC *w = (BossFPartDC *)p_;
    BOOL b;
    s32 i;
    s32 d;
    s16 v;
    s32 x;
    u16 n;
    u16 cur;
    BossVec vec;
    s32 ang;
    s32 tmp;
    s32 k;
    u16 r;
    BossFPartDC_FollowBossNode(w);
    if (w->unk21A-- == 0) {
        b = TRUE;
        if (g_dcp_world.mode != 2 && g_dcp_world.mode != 3)
            b = FALSE;
        if (b)
            i = 1;
        else
            i = g_boss_game.unk14;
        r = Rand();
        w->unk21A = (u16)((r & 0xf) + data_ov000_02365c54[i]);
        if (gPlayers->x < w->unk220)
            w->unk20C |= 4;
        else
            w->unk20C &= ~4;
    }
    k = w->unk218;
    d = w->unk220 - gPlayers->x;
    if (d < 0)
        d = -d;
    v = (s16)(k + (k * d) / 0x8000);
    if (w->unk20C & 4)
        w->h.vel_x = (s16)(w->h.vel_x - v);
    else
        w->h.vel_x = (s16)(w->h.vel_x + v);
    x = w->h.vel_x;
    if (x < -0x800)
        x = -0x800;
    else if (x > 0x800)
        x = 0x800;
    w->h.vel_x = (s16)x;
    w->unk220 += w->h.vel_x;
    x = w->unk220;
    if (x < 0x1800)
        x = 0x1800;
    else if (x > 0x32800)
        x = 0x32800;
    w->unk220 = x;
    if (w->h.timer != 0) {
        w->h.timer--;
        if (w->h.timer == 0)
            w->unk214 = w->h.vel_x;
    }
    if (w->h.timer == 0) {
        w->unk20C |= 8;
        w->unk216 += 0x88;
        if (w->unk216 >= 0x1000) {
            w->unk216 = 0x1000;
            if (s_bld_evy == 0)
                w->h.obj_flags |= 4;
        }
        w->h.vel_x = (s16)(w->unk214 + FX_MUL(-w->unk214, w->unk216));
        w->unk23C.scale[0] = FX_MUL(w->unk216, -0x3333) + 0x34cc; /* unk254 */
        if (s_bld_evy != 0 && !(w->unk210 & 1))
            bld_evy_add(-1);
    } else if (!(w->unk210 & 1)) {
        vec.x = w->unk220 << 4;
        vec.y = -0x300000;
        vec.z = 0;
        if (NNS_G3dWorldPosToScrPos(&vec, &ang, &tmp) == 0) {
            ang = 0x80 - ang;
            ang = 0x40 - (ang < 0 ? -ang : ang);
            if (ang < 0)
                x = 0;
            else if (ang > 0x40)
                x = 0x40;
            else
                x = ang;
            n = (u16)((u32)x >> 2);
            n = n < 2 ? 2 : (n > 7 ? 7 : n);
        } else {
            n = 2;
        }
        cur = (u16)s_bld_evy;
        if (cur < n)
            bld_evy_add(1);
        else if (cur > n)
            bld_evy_add(-1);
    }
    x = w->h.vel_x;
    if (x < 0)
        x = -x;
    w->unk21C = (s16)(w->unk21C + x);
    if (w->unk21C >= 0x2000) {
        w->unk21C -= 0x2000;
        MapObject_Spawn(0xdd, w->unk220, 0x30000, 0xf);
    }
    w->unk210++;
}
/* BossFPartDC_State_SweepPath (modified: BLDY state) */
STATE(BossFPartDC_State_SweepPath) {
    BossFPartDC *w = (BossFPartDC *)p_;
    s32 x;
    u32 f;
    s32 v;
    if (!(w->unk20C & 0x10) && s_bld_evy != 0 && !(w->unk210 & 3)) {
        bld_evy_add(-1);
        if (s_bld_evy == 0)
            w->unk20C |= 0x10;
    }
    BossFPartDC_FollowBossNode(w);
    w->unk220 += w->h.vel_x;
    x = w->unk220;
    if (x < 0x1800)
        x = 0x1800;
    else if (x > 0x32800)
        x = 0x32800;
    w->unk220 = x;
    f = w->unk20C;
    if (!(f & 1)) {
        if (w->h.vel_x > 0 && w->unk220 >= w->unk22C)
            w->unk20C = f | 1;
        else if (w->h.vel_x < 0 && w->unk220 <= w->unk22C)
            w->unk20C |= 1;
        v = w->h.vel_x;
        if (v < 0)
            v = -v;
        w->unk21C = (s16)(w->unk21C + v);
        if (w->unk21C >= 0x2000) {
            w->unk21C -= 0x2000;
            MapObject_Spawn(0xdd, w->unk220, 0x30000, 0xf);
        }
    } else {
        w->unk20C = f | 8;
        w->unk216 += 0x88;
        if (w->unk216 >= 0x1000) {
            w->unk216 = 0x1000;
            w->h.obj_flags |= 4;
        }
        w->h.vel_x = (s16)(w->unk214 + FX_MUL(-w->unk214, w->unk216));
        w->unk23C.scale[0] = FX_MUL(w->unk216, -0x3333) + 0x34cc;
    }
    w->unk210++;
}
/* BossFPartDC_Draw (modified: the stretched laser faces the camera; NNSi_G3dModifyPolygonAttrMask(alpha 0)
 * draws it as a wireframe on the DS while it fades (unk20C & 8): drawn at a low alpha here) */
static void BossFPartDC_Draw(void *p_) {
    BossFPartDC *w = (BossFPartDC *)p_;
    BossModel *m = &w->unk23C;
    BossVec v, t, r0, r1, r2, tmp;
    if (w->h.disp_flags & 0x20)
        return;
    v.x = w->unk220 << 4;
    v.y = -w->unk224 << 4;
    v.z = w->unk228 << 4;
    m->trans[0] = w->h.x << 4;
    m->trans[1] = -(w->h.y << 4);
    m->trans[2] = w->h.z << 4;
    t.x = m->trans[0];
    t.y = m->trans[1];
    t.z = m->trans[2];
    m->scale[2] = VEC_Distance(&v, &t); /* m->unk20 */
    m->scale[2] = Mul64(m->scale[2], 0x1199);
    VEC_Subtract(&v, &t, &r2); /* m->unk3C */
    VEC_Normalize(&r2, &r2);
    VEC_Subtract(&CAM.unk18, &t, &tmp); /* data_ov000_0236a45c: the camera eye */
    VEC_CrossProduct(&tmp, &r2, &r0);   /* m->unk24 */
    VEC_Normalize(&r0, &r0);
    VEC_CrossProduct(&r2, &r0, &r1);    /* m->unk30 */
    VEC_Normalize(&r1, &r1);
    m->rot.m[0] = r0.x; m->rot.m[1] = r0.y; m->rot.m[2] = r0.z;
    m->rot.m[3] = r1.x; m->rot.m[4] = r1.y; m->rot.m[5] = r1.z;
    m->rot.m[6] = r2.x; m->rot.m[7] = r2.y; m->rot.m[8] = r2.z;
    Model_Draw_inst(m, NULL, (w->unk20C & 8) ? 96 : 200);
}
static void BossFPartDC_Destructor(void *p_) {
    BossFPartDC *s = (BossFPartDC *)p_;
    boss_model_release(&s->unk23C);
    boxes_forget(s->coll, 3);
}
static void BossFPartDC_Collide(void *p_) { collide_default(&((BossFPartDC *)p_)->h, ((BossFPartDC *)p_)->coll); }
/* BossFPartDC_Create (modified: resources from the zone archive; NNS_G3dGlbPolygonAttr(alpha 0x10) -> the
 * laser's draw alpha; BLDCNT / BLDY state) */
static BossFPartDC *BossFPartDC_Create(const ObjParam *p, s32 b, s32 c) {
    BossFPartDC *w = NULL;
    BossModel *sub;
    int i;
    for (i = 0; i < BF_DC; i++)
        if (!s_dc[i].alive) {
            w = &s_dc[i];
            break;
        }
    if (w == NULL)
        return NULL;
    boss_model_release(&w->unk23C);
    memset(w, 0, sizeof(*w));
    w->alive = 1;
    w->unk238 = &s_laser_fin; /* "bsf" file 0x6e */
    MapObject_Init(&w->h, w->coll, p, b, c);
    w->h.obj_flags |= 0x10;
    w->h.move_flags |= 0x3300;
    if (p->flags & 1) {
        w->unk20C = 2;
        w->h.timer = p->flags >> 1; /* unkC */
        w->func = BossFPartDC_State_ChasePlayer;
        Sound_PlaySeqArc(0, 0, 0xe0);
    } else {
        w->func = BossFPartDC_State_SweepPath;
        Sound_PlaySeqArc(0, 0, 0xe0);
    }
    w->h.x = b;
    w->h.y = c;
    w->h.z = 0;
    w->unk22C = w->h.x;
    w->unk230 = w->h.y + 0x64;
    w->unk234 = w->h.z;
    sub = &w->unk23C;
    Model_Init(sub, 0);
    Model_SetResource(sub, w->unk238);
    sub->scale[0] = sub->scale[1] = sub->scale[2] = 0x34cc;
    bld_set(2, 7); /* raw 0, b0 (BG0) 1, b6 (mode) 2, unk24.b0 7 */
    s_st.laser_objs++;
    return w;
}

/* ============================================================ BossFPartDD: laser impact */
/* BossFPartDD_State_HitThenVanish (copied) */
STATE(BossFPartDD_State_HitThenVanish) {
    BossFPartDD *p = (BossFPartDD *)p_;
    BossModel *cam = &p->unk218;
    p->h.timer++;
    if (p->h.timer > 8) {
        p->coll[1].flags &= ~4; /* unk16A */
    } else if (p->h.timer >= 0) {
        p->h.disp_flags &= ~0x20;
        p->coll[1].flags |= 4;
    }
    if (!(cam->flags[1] & 2)) /* unk10D */
        return;
    if (cam->flags[2] & 2) /* unk10E */
        p->h.obj_flags |= 4;
}
/* BossFPartDD_Draw (copied) */
static void BossFPartDD_Draw(void *p_) {
    BossFPartDD *o = (BossFPartDD *)p_;
    BossModel *p = &o->unk218;
    if (o->h.disp_flags & 0x20)
        return;
    p->trans[0] = o->h.x << 4;
    p->trans[1] = -(o->h.y << 4);
    p->trans[2] = o->h.z << 4;
    Model_Animate(p);
    Model_Draw(p);
}
static void BossFPartDD_Destructor(void *p_) {
    BossFPartDD *s = (BossFPartDD *)p_;
    boss_model_release(&s->unk218);
    boxes_forget(s->coll, 3);
}
static void BossFPartDD_Collide(void *p_) { collide_default(&((BossFPartDD *)p_)->h, ((BossFPartDD *)p_)->coll); }
/* BossFPartDD_Create (modified: resources from the zone archive; .nsbma / .nsbtp timing only) */
static BossFPartDD *BossFPartDD_Create(const ObjParam *p, s32 b, s32 c) {
    BossFPartDD *w = NULL;
    ObjBox *coll;
    BossModel *sub;
    int i;
    for (i = 0; i < BF_DD; i++)
        if (!s_dd[i].alive) {
            w = &s_dd[i];
            break;
        }
    if (w == NULL)
        return NULL;
    boss_model_release(&w->unk218);
    memset(w, 0, sizeof(*w));
    w->alive = 1;
    w->func = BossFPartDD_State_HitThenVanish;
    w->unk20C = &s_laser_fin_b; /* 0x6f */
    w->unk210 = &s_lfb_ma;      /* 0x70 */
    w->unk214 = &s_lfb_tp;      /* 0x71 */
    MapObject_Init(&w->h, w->coll, p, b, c);
    w->h.obj_flags |= 0x10;
    w->h.move_flags |= 0x3300;
    w->h.disp_flags |= 0x20;
    w->h.x = b;
    w->h.y = c;
    w->h.z = 0;
    w->h.timer = -(s32)p->flags; /* unkC = -p->unk4 */
    coll = &w->coll[0];
    coll->flags &= ~4;
    coll = &w->coll[1];
    rect_set_box2d(coll, -20, -20, 20, 0);
    coll->owner = w;
    coll->flags &= ~4;
    sub = &w->unk218;
    Model_Init(sub, 0);
    Model_SetResource(sub, w->unk20C);
    Model_SetAnimation(sub, 1, w->unk210, 0);
    Model_SetAnimation(sub, 2, w->unk214, 0);
    sub->speed = 0x1800; /* unk114 */
    sub->scale[0] = sub->scale[1] = sub->scale[2] = 0x34cc;
    sub->cb[0] = MOP_SETCAMERAROT; /* unkA = 0x13 */
    sub->cb[1] = MOP_FLUSHP;       /* unkB = 3 */
    sub->cb[2] = 0;
    Sound_PlaySeqArc(0, 0, 0xe5);
    s_st.flashes++;
    return w;
}

/* ============================================================ BossFPartDE: beam ball */
STATE(BossFPartDE_State_Start);
STATE(BossFPartDE_State_StartRise);
STATE(BossFPartDE_State_Rise);
STATE(BossFPartDE_State_Drop);
STATE(BossFPartDE_State_Fall);
STATE(BossFPartDE_State_Land);
STATE(BossFPartDE_State_StartTrack);
STATE(BossFPartDE_State_Track);
STATE(BossFPartDE_State_StartAim);
STATE(BossFPartDE_State_Aim);
STATE(BossFPartDE_State_Activate);
STATE(BossFPartDE_State_Patrol);
STATE(BossFPartDE_State_PickTarget);
STATE(BossFPartDE_State_ResizeToTarget);
STATE(BossFPartDE_State_StartPause);
STATE(BossFPartDE_State_Pause);
STATE(BossFPartDE_State_StartSweep);
STATE(BossFPartDE_State_Sweep);
STATE(BossFPartDE_State_Deactivate);
STATE(BossFPartDE_State_Leave);
STATE(BossFPartDE_StartChainRise);
STATE(BossFPartDE_State_ChainRise);
STATE(BossFPartDE_State_ChainSpawnNext);
STATE(BossFPartDE_State_ChainDrop);
STATE(BossFPartDE_State_ChainFall);
STATE(BossFPartDE_State_ChainLand);
STATE(BossFPartDE_State_ChainStartWait);
STATE(BossFPartDE_State_WaitTimer_ov000_022f5840);
STATE(BossFPartDE_State_ChainActivate);
STATE(BossFPartDE_State_ChainSpread);
#define DE BossFPartDE *s = (BossFPartDE *)p_

/* BossFPartDE_TrackPlayerX (copied) */
static void BossFPartDE_TrackPlayerX(BossFPartDE *s) {
    s32 t;
    s32 d;
    s16 a;
    t = gPlayers->x;
    if (t < 0x8000)
        t = 0x8000;
    else if (t > 0x2c000)
        t = 0x2c000;
    d = t - s->h.x;
    if ((d < 0 ? -d : d) < 0x3000) {
        a = s->h.vel_x;
        if (a < 0) {
            if (-a > 0x60)
                s->h.vel_x = (s16)(a + 0x60);
            else
                s->h.vel_x = 0;
        } else if (a > 0) {
            if (a > 0x60)
                s->h.vel_x = (s16)(a - 0x60);
            else
                s->h.vel_x = 0;
        }
    } else if (d > 0) {
        a = s->h.vel_x;
        if (a < 0x800) {
            if (0x800 - a > 0x60)
                s->h.vel_x = (s16)(a + 0x60);
            else
                s->h.vel_x = 0x800;
        } else if (a > 0x800) {
            if (a - 0x800 > 0x60)
                s->h.vel_x = (s16)(a - 0x60);
            else
                s->h.vel_x = 0x800;
        }
    } else {
        a = s->h.vel_x;
        if (a < -0x800) {
            if (-0x800 - a > 0x60)
                s->h.vel_x = (s16)(a + 0x60);
            else
                s->h.vel_x = -0x800;
        } else if (a > -0x800) {
            if (a + 0x800 > 0x60)
                s->h.vel_x = (s16)(a - 0x60);
            else
                s->h.vel_x = -0x800;
        }
    }
}
/* BossFPartDE_UpdatePatrolSpeed (copied) */
static BOOL BossFPartDE_UpdatePatrolSpeed(BossFPartDE *s) {
    s16 a;
    if (s->h.x < 0x8000) {
        a = s->h.vel_x;
        if (a < 0x800) {
            if (0x800 - a > 0x40)
                s->h.vel_x = (s16)(a + 0x40);
            else
                s->h.vel_x = 0x800;
        } else if (a > 0x800) {
            if (a - 0x800 > 0x40)
                s->h.vel_x = (s16)(a - 0x40);
            else
                s->h.vel_x = 0x800;
        }
        return FALSE;
    }
    if (s->h.x > 0x2c000) {
        a = s->h.vel_x;
        if (a < -0x800) {
            if (-0x800 - a > 0x40)
                s->h.vel_x = (s16)(a + 0x40);
            else
                s->h.vel_x = -0x800;
        } else if (a > -0x800) {
            if (a + 0x800 > 0x40)
                s->h.vel_x = (s16)(a - 0x40);
            else
                s->h.vel_x = -0x800;
        }
        return FALSE;
    }
    a = s->h.vel_x;
    if (a < 0) {
        if (-a > 0x80)
            s->h.vel_x = (s16)(a + 0x80);
        else
            s->h.vel_x = 0;
    } else if (a > 0) {
        if (a > 0x80)
            s->h.vel_x = (s16)(a - 0x80);
        else
            s->h.vel_x = 0;
    }
    return s->h.vel_x == 0;
}
/* BossFPartDE_NarrowEnds (copied) */
static void BossFPartDE_NarrowEnds(BossFPartDE *s) {
    s32 t;
    if (s->h.vel_x > 0x2000) {
        s->unk35Cx = s->unk35Cx + (((0x8000 - (s->unk35Cx - s->h.x)) << 14) >> 16);
        s->unk218x = s->unk218x - (((0x8000 - (s->h.x - s->unk218x)) << 12) >> 16);
    } else if (s->h.vel_x < -0x2000) {
        t = s->h.x - s->unk218x;
        s->unk218x -= ((0x8000 - t) << 14) >> 16;
        s->unk35Cx = s->unk35Cx + (((0x8000 - (s->unk35Cx - s->h.x)) << 12) >> 16);
    } else {
        t = s->h.x - s->unk218x;
        s->unk218x -= ((0x8000 - t) << 13) >> 16;
        s->unk35Cx = s->unk35Cx + (((0x8000 - (s->unk35Cx - s->h.x)) << 13) >> 16);
    }
    t = s->unk218x;
    if (t < 0)
        t = 0;
    else if (t > 0x34000)
        t = 0x34000;
    s->unk218x = t;
    t = s->unk35Cx;
    if (t < 0)
        t = 0;
    else if (t > 0x34000)
        t = 0x34000;
    s->unk35Cx = t;
}
/* BossFPartDE_ShiftEndsTowardX (copied) */
static void BossFPartDE_ShiftEndsTowardX(BossFPartDE *w, s32 maxv, s32 maxd) {
    s32 half = (w->unk35Cx - w->unk218x) >> 1;
    s32 mid = w->unk218x + half;
    s32 d = w->h.x - mid;
    s32 p;
    d >>= 4;
    d = d < -maxd ? -maxd : (d > maxd ? maxd : d);
    w->unk4C6 = (s16)(w->unk4C6 + (s16)d);
    w->unk4C6 = (s16)(w->unk4C6 < -maxv ? -maxv : (w->unk4C6 > maxv ? maxv : w->unk4C6));
    p = mid + w->unk4C6;
    p = p < half ? half : (p > 0x34000 - half ? 0x34000 - half : p);
    w->unk218x = p - half;
    w->unk35Cx = p + half;
}
/* BossFPartDE_SpawnBeamFinEffects (copied) */
static void BossFPartDE_SpawnBeamFinEffects(BossFPartDE *p) {
    s32 i;
    switch (p->unk4B0++ & 7) {
    case 0:
        i = 0;
        break;
    case 4:
        i = 1;
        break;
    default:
        return;
    }
    BossEffect_CreateBeamFinB(1, p->e[i].x, p->e[i].y, p->e[i].z);
}
/* BossFPartDE_OnHit (modified: owner -> obj_box_player) */
static void BossFPartDE_OnHit(ObjBox *a, ObjBox *b) {
    BossFPartDE *w;
    DcPlayer *p;
    BossFLine line;
    BossVec pp, wp, ep;
    u32 i;
    p = obj_box_player(b);
    w = (BossFPartDE *)a->owner;
    if (p == NULL || p->invuln > 0)
        return;
    for (i = 0; i < 2; i++) {
        pp.x = p->x << 4;
        pp.y = -p->y << 4;
        pp.z = p->z << 4;
        wp.x = w->h.x << 4;
        wp.y = -w->h.y << 4;
        wp.z = w->h.z << 4;
        ep.x = w->e[i].x << 4;
        ep.y = -w->e[i].y << 4;
        ep.z = w->e[i].z << 4;
        Math_LineFromPoints(&wp, &ep, &line);
        if (Math_SegmentHitsSphere(&line, &wp, &ep, 0xc000, &pp)) {
            Player_OnHurt(b, a);
            return;
        }
    }
}
/* BossFPartDE_Draw (modified: beams drawn see-through while unk4A4 (the DS draws them as wireframes:
 * NNSi_G3dModifyPolygonAttrMask alpha 0)) */
static void BossFPartDE_Draw(void *p_) {
    BossFPartDE *obj = (BossFPartDE *)p_;
    BossModel *cam = &obj->unk4D4;
    MtxFx33 mtx;
    BossVec v, d, u, t;
    u16 i;
    if (obj->h.disp_flags & 0x20)
        return;
    cam->trans[0] = obj->h.x << 4;
    cam->trans[1] = -(obj->h.y << 4);
    cam->trans[2] = obj->h.z << 4;
    obj->unk210 = (u16)(obj->unk210 + 0x400);
    MTX_RotX33(&cam->rot, SIN(obj->unk210), COS(obj->unk210));
    MTX_RotZ33(&mtx, SIN(obj->unk212), COS(obj->unk212));
    MTX_Concat33(&cam->rot, &mtx, &cam->rot);
    Model_Draw(cam);
    if (obj->unk4A0 == 0)
        return;
    for (i = 0; i < 2; i++) {
        BossFBeamEnd *e = &obj->e[i];
        BossModel *c = &e->m;
        v.x = e->x << 4;
        v.y = -(e->y << 4);
        v.z = e->z << 4;
        c->trans[0] = obj->h.x << 4;
        c->trans[1] = -(obj->h.y << 4);
        c->trans[2] = obj->h.z << 4;
        t.x = c->trans[0];
        t.y = c->trans[1];
        t.z = c->trans[2];
        c->scale[2] = VEC_Distance(&v, &t); /* c->unk20 */
        VEC_Subtract(&v, &t, &d);           /* c->unk3C */
        VEC_Normalize(&d, &d);
        u.x = 0;                            /* c->unk30 */
        u.y = 0;
        u.z = 0x1000;
        VEC_CrossProduct(&u, &d, &v);       /* c->unk24 */
        VEC_Normalize(&v, &v);
        c->rot.m[0] = v.x; c->rot.m[1] = v.y; c->rot.m[2] = v.z;
        c->rot.m[3] = u.x; c->rot.m[4] = u.y; c->rot.m[5] = u.z;
        c->rot.m[6] = d.x; c->rot.m[7] = d.y; c->rot.m[8] = d.z;
        Model_Animate(c);
        Model_Draw_inst(c, NULL, obj->unk4A4 ? 96 : 255);
    }
}
/* BossFPartDE_Destructor (modified: NNS_SndHandleReleaseSeq: the handle is released, the sound is not stopped) */
static void BossFPartDE_Destructor(void *p_) {
    BossFPartDE *s = (BossFPartDE *)p_;
    boss_model_release(&s->unk4D4);
    boss_model_release(&s->e[0].m);
    boss_model_release(&s->e[1].m);
    boxes_forget(s->coll, 3);
}
static void BossFPartDE_Collide(void *p_) { collide_default(&((BossFPartDE *)p_)->h, ((BossFPartDE *)p_)->coll); }

/* single ball (kind 0) */
STATE(BossFPartDE_State_Start) {
    DE;
    s->func = BossFPartDE_State_StartRise;
    s->func(s);
}
STATE(BossFPartDE_State_StartRise) {
    BossFPartDE *p = (BossFPartDE *)p_;
    BossFPartD8 *t = gBossF->unk218;
    p->h.x = t->h.x;
    p->h.y = -t->unk368 >> 4;
    p->h.z = t->h.z - 0x3200;
    p->h.vel_y = -0xc00;
    Sound_PlaySeqArc(0, 0, 0xe4);
    p->func = BossFPartDE_State_Rise;
}
STATE(BossFPartDE_State_Rise) {
    DE;
    if (s->h.y < -0x1f400)
        s->func = BossFPartDE_State_Drop;
}
STATE(BossFPartDE_State_Drop) {
    BossFPartDE *w = (BossFPartDE *)p_;
    w->h.x = gPlayers->x;
    w->h.y = 0x23800;
    w->h.z = gPlayers->z;
    w->h.vel_y = 0xa00;
    w->e[0].x = w->h.x - 0x8000; /* Obj_SetVec(&w->unk218, ..) */
    w->e[0].y = 0x30000;
    w->e[0].z = 0;
    w->e[0].x = w->e[0].x < 0 ? 0 : (w->e[0].x > 0x34000 ? 0x34000 : w->e[0].x);
    w->e[1].x = w->h.x + 0x8000;
    w->e[1].y = 0x30000;
    w->e[1].z = 0;
    w->e[1].x = w->e[1].x < 0 ? 0 : (w->e[1].x > 0x34000 ? 0x34000 : w->e[1].x);
    w->func = BossFPartDE_State_Fall;
}
STATE(BossFPartDE_State_Fall) {
    DE;
    if (s->h.y > 0x24c00)
        s->func = BossFPartDE_State_Land;
}
STATE(BossFPartDE_State_Land) {
    DE;
    s->h.vel_y = (s16)((s->h.vel_y * 0xd0) >> 8);
    if (s->h.vel_y <= 0x30) {
        s->h.vel_y = 0;
        s->func = BossFPartDE_State_StartTrack;
    }
}
STATE(BossFPartDE_State_StartTrack) {
    DE;
    s->h.timer = 0;
    s->func = BossFPartDE_State_Track;
}
STATE(BossFPartDE_State_Track) {
    DE;
    BossFPartDE_TrackPlayerX(s);
    if (s->h.timer++ > 0x1e)
        s->func = BossFPartDE_State_StartAim;
}
/* BossFPartDE_State_StartAim (modified: NNS_G3dGlbPolygonAttr -> draw alpha) */
STATE(BossFPartDE_State_StartAim) {
    DE;
    s->unk4A0 = 1;
    s->unk4A4 = 1;
    s->h.timer = 0;
    s->func = BossFPartDE_State_Aim;
}
STATE(BossFPartDE_State_Aim) {
    BossFPartDE *p = (BossFPartDE *)p_;
    DcPlayer *plr;
    BossFPartDE_TrackPlayerX(p);
    BossFPartDE_NarrowEnds(p);
    if (p->h.timer++ <= 0x3c)
        return;
    plr = gPlayers;
    if (!(plr->move_flags & 1))
        return;
    if (p->unk218x >= plr->x)
        return;
    if (plr->x < p->unk35Cx)
        p->func = BossFPartDE_State_Activate;
}
/* BossFPartDE_State_Activate (modified: the looping beam sound on the object's own handle) */
STATE(BossFPartDE_State_Activate) {
    DE;
    s->unk4A4 = 0;
    s->coll[0].flags |= 4; /* unk13E */
    boss_sfx_handle(s->unk4A8, 0xe3);
    s->func = BossFPartDE_State_Patrol;
}
STATE(BossFPartDE_State_Patrol) {
    DE;
    BossFPartDE_SpawnBeamFinEffects(s);
    s->unk210 = (u16)(s->unk210 + 0x800);
    if (BossFPartDE_UpdatePatrolSpeed(s))
        s->func = BossFPartDE_State_PickTarget;
}
STATE(BossFPartDE_State_PickTarget) {
    BossFPartDE *w = (BossFPartDE *)p_;
    w->unk4B4 = (((Rand() & 7) << 6) >> 3) + 0x4000;
    if (Rand() & 1)
        w->unk4B4 = -w->unk4B4;
    w->unk4B4 += w->h.x;
    w->unk4B8 = data_ov000_02363de4[GetLevel()];
    w->unk4BC = data_ov000_02363dc8[GetLevel()];
    w->unk4AC = 0;
    w->func = BossFPartDE_State_ResizeToTarget;
    w->func(w);
}
static inline s32 Lerp(s32 a, s32 b, s32 t) { return a + FX_Mul(b - a, t); }
STATE(BossFPartDE_State_ResizeToTarget) {
    BossFPartDE *w = (BossFPartDE *)p_;
    BossFPartDE_SpawnBeamFinEffects(w);
    w->unk210 = (u16)(w->unk210 + 0x800);
    w->unk4AC = (s16)(w->unk4AC + w->unk4BC);
    if (w->unk4AC >= 0x1000) {
        w->unk4AC = 0x1000;
        w->func = BossFPartDE_State_StartPause;
    }
    w->unk218x = Lerp((w->h.x - 0x8000) << 4, (w->unk4B4 - (w->unk4B8 >> 1)) << 4, w->unk4AC) >> 4;
    w->unk35Cx = Lerp((w->h.x + 0x8000) << 4, (w->unk4B4 + (w->unk4B8 >> 1)) << 4, w->unk4AC) >> 4;
}
STATE(BossFPartDE_State_StartPause) {
    DE;
    s->unk4AE = 0;
    s->func = BossFPartDE_State_Pause;
    s->func(s);
}
STATE(BossFPartDE_State_Pause) {
    DE;
    s32 t1;
    BossFPartDE_SpawnBeamFinEffects(s);
    s->unk210 = (u16)(s->unk210 + 0x800);
    t1 = s->unk4AE;
    s->unk4AE = (u16)(s->unk4AE + 1);
    if ((u32)t1 > 0x1e)
        s->func = BossFPartDE_State_StartSweep;
}
/* BossFPartDE_State_StartSweep (copied: the first test is overwritten by the second, as in the original) */
STATE(BossFPartDE_State_StartSweep) {
    BossFPartDE *w = (BossFPartDE *)p_;
    if (w->h.x < 0x10000)
        w->unk4C0 = 0;
    if (w->h.x > 0x24000)
        w->unk4C0 = 1;
    else
        w->unk4C0 = Rand() & 1;
    w->unk4C4 = data_ov000_02363db8[GetIdxB()];
    w->unk4BE = 0;
    w->unk4C6 = 0;
    w->func = BossFPartDE_State_Sweep;
    w->func(w);
}
STATE(BossFPartDE_State_Sweep) {
    DE;
    s32 px;
    s32 v;
    s32 i;
    if (s->unk4C4 != 0)
        s->unk4C4--;
    BossFPartDE_SpawnBeamFinEffects(s);
    s->unk210 = (u16)(s->unk210 + 0x800);
    s->unk212 = (u16)(-s->h.vel_x << 3);
    px = gPlayers->x;
    if (s->unk218x < px && px < s->unk35Cx) {
        if (s->unk4C4 != 0) {
            if (s->unk4C0)
                s->h.vel_x = (s16)(s->h.vel_x - 8);
            else
                s->h.vel_x = (s16)(s->h.vel_x + 8);
        } else {
            v = s->h.vel_x;
            if (v < 0) {
                if (-v > 8)
                    s->h.vel_x = (s16)(v + 8);
                else
                    s->h.vel_x = 0;
            } else if (v > 0) {
                if (v > 8)
                    s->h.vel_x = (s16)(v - 8);
                else
                    s->h.vel_x = 0;
            }
        }
        v = s->h.vel_x;
        if (v < -0x220)
            v = -0x220;
        else if (v > 0x220)
            v = 0x220;
        s->h.vel_x = (s16)v;
        i = GetIdxB();
        BossFPartDE_ShiftEndsTowardX(s, data_ov000_02363dc0[GetIdxS()], data_ov000_02363dcc[i]);
    } else {
        if (s->unk4C4 != 0) {
            if (px < s->h.x)
                s->h.vel_x = (s16)(s->h.vel_x - 0x40);
            else
                s->h.vel_x = (s16)(s->h.vel_x + 0x40);
        } else {
            v = s->h.vel_x;
            if (v < 0) {
                if (-v > 0x40)
                    s->h.vel_x = (s16)(v + 0x40);
                else
                    s->h.vel_x = 0;
            } else if (v > 0) {
                if (v > 0x40)
                    s->h.vel_x = (s16)(v - 0x40);
                else
                    s->h.vel_x = 0;
            }
        }
        v = s->h.vel_x;
        if (v < -0x600)
            v = -0x600;
        else if (v > 0x600)
            v = 0x600;
        s->h.vel_x = (s16)v;
        BossFPartDE_ShiftEndsTowardX(s, 0x500, 0x500);
    }
    if (s->unk4C4 != 0)
        return;
    if (s->h.vel_x != 0)
        return;
    s->unk4BE++;
    if (s->unk4BE < data_ov000_02363dbc[GetIdxB()]) {
        s->unk4C0 = s->unk4C0 == 0 ? 1 : 0;
        s->unk4C4 = data_ov000_02363db8[GetIdxB()];
        s->func = BossFPartDE_State_Sweep;
    } else {
        s->func = BossFPartDE_State_Deactivate;
    }
}
/* BossFPartDE_State_Deactivate (modified: NNS_SndPlayerStopSeq(handle, 60) -> sfx_stop) */
STATE(BossFPartDE_State_Deactivate) {
    DE;
    s->unk4A4 = 1;
    s->coll[0].flags &= ~4;
    boss_sfx_stop(s->unk4A8);
    s->h.vel_x = 0;
    s->func = BossFPartDE_State_Leave;
    s->func(s);
}
STATE(BossFPartDE_State_Leave) {
    BossFPartDE *w = (BossFPartDE *)p_;
    s16 v;
    w->h.vel_y = (s16)(w->h.vel_y - 0x40);
    w->unk210 = (u16)(w->unk210 + 0x800 - w->h.vel_y);
    v = (s16)w->unk212;
    if (v < 0) {
        if (-v > 0x800)
            v = (s16)(v + 0x800);
        else
            v = 0;
    } else if (v > 0) {
        if (v > 0x800)
            v = (s16)(v - 0x800);
        else
            v = 0;
    }
    w->unk212 = (u16)v;
    BossFPartDE_ShiftEndsTowardX(w, data_ov000_02363dc0[GetLevel()], data_ov000_02363dcc[GetLevel()]);
    if (w->h.y < 0x18000) {
        gBossF->unk218->unk438--;
        w->h.obj_flags |= 4;
    }
}

/* chain (kind 1): each ball spawns the next one */
STATE(BossFPartDE_StartChainRise) {
    BossFPartDE *p = (BossFPartDE *)p_;
    BossFPartD8 *t = gBossF->unk218;
    p->h.x = t->h.x;
    p->h.y = -t->unk368 >> 4;
    p->h.z = t->h.z - 0x3200;
    p->h.vel_y = -0x1000;
    Sound_PlaySeqArc(0, 0, 0xe4);
    p->func = BossFPartDE_State_ChainRise;
}
STATE(BossFPartDE_State_ChainRise) {
    BossFPartDE *p = (BossFPartDE *)p_;
    BOOL b;
    s32 idx;
    b = g_dcp_world.mode == 2 || g_dcp_world.mode == 3;
    if (b)
        idx = 1;
    else
        idx = g_boss_game.unk14;
    if (data_ov000_02364198[idx][p->unk214] > p->h.y)
        p->func = BossFPartDE_State_ChainSpawnNext;
}
STATE(BossFPartDE_State_ChainSpawnNext) {
    BossFPartDE *w = (BossFPartDE *)p_;
    s32 n;
    s32 i = GetIdxB();
    n = w->unk214 + 1;
    if (n < data_ov000_02363db0[i])
        MapObject_Spawn(0xde, 0, 0, (u16)((n << 4) | 1));
    w->func = BossFPartDE_State_ChainDrop;
}
/* BossFPartDE_State_ChainDrop (modified: NNS_G3dGlbPolygonAttr -> draw alpha) */
STATE(BossFPartDE_State_ChainDrop) {
    BossFPartDE *w = (BossFPartDE *)p_;
    s32 margin, off, v, a;
    BOOL dir;
    DcPlayer *pl;
    w->h.vel_y = 0;
    w->h.x = gPlayers->x;
    w->h.y = 0x17000;
    w->h.z = gPlayers->z;
    w->h.vel_y = 0xa00;
    margin = (data_ov000_02364068[GetLevel()][w->unk214] >> 8) + 0x20;
    off = (Rand() & 7) << 10;
    off += data_ov000_02364068[GetLevel()][w->unk214];
    pl = gPlayers;
    if (pl->x < margin << 8) {
        dir = FALSE;
    } else if ((0x300 - margin) << 8 < pl->x) {
        dir = TRUE;
    } else {
        v = pl->vel_x;
        a = v < 0 ? -v : v;
        if (a > 0x280) {
            if (v > 0)
                dir = (Rand() & 7) < 6;
            else
                dir = (Rand() & 7) >= 6;
        } else {
            dir = !(Rand() & 1);
        }
    }
    if (dir)
        w->h.x -= off;
    else
        w->h.x += off;
    w->e[0].x = w->h.x - 0x4000;
    w->e[0].y = 0x30000;
    w->e[0].z = 0;
    w->e[1].x = w->h.x + 0x4000;
    w->e[1].y = 0x30000;
    w->e[1].z = 0;
    if (w->e[0].x < 0)
        w->e[0].x = 0;
    if (w->e[1].x > 0x34000)
        w->e[1].x = 0x34000;
    w->unk4A0 = 1;
    w->unk4A4 = 1;
    w->func = BossFPartDE_State_ChainFall;
}
STATE(BossFPartDE_State_ChainFall) {
    DE;
    if (s->h.y > 0x25600)
        s->func = BossFPartDE_State_ChainLand;
}
STATE(BossFPartDE_State_ChainLand) {
    DE;
    s->h.vel_y = (s16)((s->h.vel_y * 0xd0) >> 8);
    if (s->h.vel_y <= 0x30) {
        s->h.vel_y = 0;
        s->func = BossFPartDE_State_ChainStartWait;
    }
}
STATE(BossFPartDE_State_ChainStartWait) {
    DE;
    s->h.timer = 0;
    s->func = BossFPartDE_State_WaitTimer_ov000_022f5840;
}
STATE(BossFPartDE_State_WaitTimer_ov000_022f5840) {
    DE;
    if (s->h.timer++ > 0)
        s->func = BossFPartDE_State_ChainActivate;
}
STATE(BossFPartDE_State_ChainActivate) {
    DE;
    s->unk4A4 = 0;
    s->coll[0].flags |= 4;
    boss_sfx_handle(s->unk4A8, 0xe3);
    s->func = BossFPartDE_State_ChainSpread;
}
STATE(BossFPartDE_State_ChainSpread) {
    BossFPartDE *w = (BossFPartDE *)p_;
    BOOL far = FALSE;
    w->h.vel_y = (s16)(w->h.vel_y - data_ov000_02364090[GetLevel()][w->unk214]);
    w->unk210 = (u16)(w->unk210 - (w->h.vel_y << 2));
    w->unk218x += w->h.vel_y;
    if (w->h.x - w->unk218x > 0xb400)
        far = TRUE;
    if (w->unk218x < 0)
        w->unk218x = 0;
    w->unk35Cx -= w->h.vel_y;
    if (w->unk35Cx - w->h.x > 0xb400)
        far = TRUE;
    if (w->unk35Cx > 0x34000)
        w->unk35Cx = 0x34000;
    if (far && w->unk4A0 != 0) {
        w->unk4A0 = 0;
        w->coll[0].flags &= ~4;
        boss_sfx_stop(w->unk4A8);
    }
    if (w->h.y < 0x1a000) {
        gBossF->unk218->unk438--;
        w->h.obj_flags |= 4;
    }
}
/* BossFPartDE_Create (modified: resources from the zone archive; a native sound handle 1..3) */
static BossFPartDE *BossFPartDE_Create(const ObjParam *p, s32 b, s32 c) {
    BossF *boss = gBossF;
    BossFPartDE *w = NULL;
    ObjBox *coll;
    BossModel *sub;
    u16 i;
    int k;
    for (k = 0; k < BF_DE; k++)
        if (!s_de[k].alive) {
            w = &s_de[k];
            break;
        }
    if (w == NULL)
        return NULL;
    boss_model_release(&w->unk4D4);
    boss_model_release(&w->e[0].m);
    boss_model_release(&w->e[1].m);
    memset(w, 0, sizeof(*w));
    w->alive = 1;
    w->unk20C = p->flags & 0xf;
    w->unk214 = (s16)(p->flags >> 4);
    switch (w->unk20C) {
    case 0:
        w->func = BossFPartDE_State_Start;
        break;
    case 1:
        w->func = BossFPartDE_StartChainRise;
        break;
    }
    boss->unk218->unk438++;
    w->unk4CC = &s_beam_fin; /* 0x72 */
    w->unk4D0 = &s_bf_tp;    /* 0x73 */
    w->unk4C8 = &data_ov000_02369b88[2];
    w->unk4A8 = 1 + (s_snd_next++ % 3);
    MapObject_Init(&w->h, w->coll, p, b, c);
    w->h.obj_flags |= 0x10;
    w->h.move_flags |= 0x8200;
    w->h.x = b;
    w->h.y = c;
    w->h.z = 0;
    coll = &w->coll[0];
    rect_set_box2d(coll, -0x400, -0x400, 0x400, 0x400);
    rect_set_atkdef(coll, 0x11, 0);
    coll->flags |= 0x40;
    coll->cb = BossFPartDE_OnHit;
    coll->owner = w;
    coll->flags &= ~4;
    sub = &w->unk4D4;
    Model_Init(sub, 0);
    Model_SetResource(sub, w->unk4C8);
    sub->scale[0] = sub->scale[1] = sub->scale[2] = 0x34cc;
    for (i = 0; i < 2; i++) {
        BossModel *s = &w->e[i].m;
        Model_Init(s, 0);
        Model_SetResource(s, w->unk4CC);
        Model_SetAnimation(s, 2, w->unk4D0, 0);
        s->flags[2] |= 4; /* unk10E */
        s->scale[0] = s->scale[1] = s->scale[2] = 0x34cc;
    }
    s_st.beam_objs++;
    return w;
}

/* ============================================================ MapObject_Spawn */
static void *MapObject_Spawn(u16 type, s32 x, s32 y, u16 flags) {
    ObjParam prm;
    memset(&prm, 0, sizeof(prm));
    prm.x0 = prm.y0 = 0xff;
    prm.type = type;
    prm.flags = flags;
    switch (type) {
    case 0xd8:
        return BossFPartD8_Create(&prm, x, y);
    case 0xd9:
        return BossFPartD9_Create(&prm, x, y);
    case 0xda:
        return BossFPartDA_Create(&prm, x, y);
    case 0xdb:
        return BossFPartDB_Create(&prm, x, y);
    case 0xdc:
        return BossFPartDC_Create(&prm, x, y);
    case 0xdd:
        return BossFPartDD_Create(&prm, x, y);
    case 0xde:
        return BossFPartDE_Create(&prm, x, y);
    }
    return NULL;
}

/* ============================================================ BossF_Create */
/* BossF_Create (modified: resources from BossStageF / the zone archive (bossf_load); the exc:eff_*
 * resources of the BossEffect wrappers are loaded by boss_effects_load) */
void *BossF_Create(const ObjParam *p, s32 b, s32 c) {
    BossF *w;
    BossModel *sub;
    u32 i;
    if (p->x0 == 0xff && p->y0 == 0xff)
        return NULL;
    w = &s_bf;
    boss_model_release(&w->unk244);
    boss_model_release(&w->unk85C);
    for (i = 0; i < 4; i++)
        boss_model_release(&w->unk37C[i]);
    memset(w, 0, sizeof(*w));
    w->alive = 1;
    gBossF = w;
    s_chara = gPlayers != NULL ? gPlayers->chara : 0; /* gGameState.unk0 = the story character */
    w->func = BossF_State_WaitForStart;
    w->unk214 = Rand() & 1;
    w->unk9BC = -0x300000;
    w->unk9C0 = (s32)0xfe978000;
    w->unk9A4 = -0x300000;
    w->unk9A8 = (s32)0xfe978000;
    w->unk9AC = (s32)0xfe978000;
    w->unk9B0 = (s32)0xfd5f0000;
    memset(&gBossStatus, 0, sizeof(gBossStatus));
    gBossStatus.unk4 = 0x2f;
    gBossStatus.unk8 = (u16)Boss_GetStartHealth();
    gBossStatus.unkA = (u16)Boss_GetStartHealth();
    w->unk224 = &s_bsf_ca; /* "bsf" files 8..11 */
    w->unk228 = &s_bsf_va;
    w->unk22C = &s_bsf_ta;
    w->unk230 = &s_bsf_ma;
    w->unk220 = &data_ov000_02369b88[0];
    w->unk234 = &data_ov000_02369b88[5];
    MapObject_Init(&w->h, &w->unk134, p, b, c);
    w->h.obj_flags |= 0x12;
    w->h.move_flags |= 0xa300;
    w->h.disp_flags |= 0x20;
    w->h.x = 0x1a000;
    w->h.y = 0x30000;
    w->h.z = 0;
    sub = &w->unk244;
    Model_Init(sub, 0);
    Model_SetResource(sub, w->unk220);
    Model_SetAnimation(sub, 0, w->unk224, 1);
    Model_SetAnimation(sub, 4, w->unk228, 0);
    sub->trans[0] = w->h.x << 4;
    sub->trans[1] = -(w->h.y << 4);
    sub->trans[2] = w->h.z << 4;
    sub->scale[0] = sub->scale[1] = sub->scale[2] = 0x34cc;
    w->unk218 = (BossFPartD8 *)MapObject_Spawn(0xd8, 0, 0, 0);
    BossF_LoadModelPalettesAll(gGameState, 0);
    /* NNS_G3dResDefaultSetup("exc:eff_hit / impact_sl / impact_st / impact_pr / dmg_hand / produce / bomb_ball /
     * laser_fin / laser_fin_b / beam_fin / beam_fin_b.nsbmd"): boss_effects_load + bossf_load */
    w->unk238 = &s_sky;     /* File_Load("/mod/bsf_sky.nsbmd") */
    w->unk240 = s_passage;  /* File_Load("/mod/bsf_passage.nsbmd") */
    sub = &w->unk85C;
    Model_Init(sub, 0);
    Model_SetResource(sub, w->unk234);
    sub = w->unk37C;
    for (i = 0; i < 4; i++) {
        Model_Init(sub, 0);
        Model_SetResource(sub, &w->unk240[i]);
        sub++;
    }
    sub = &w->unk37C[0];
    sub->trans[0] = 0xc8000;
    sub->trans[2] = -0x3e8000;
    sub->trans[1] = -0x3c8000;
    gStageMapCollision_unk1C = 0;
    gStageMapCollision_unk24 = 0x340;
    boss_log("create", gBossStatus.unk8, 0);
    return w;
}

/* ============================================================ module glue */
static int load_lengths(BossAnimRes *r, const char *name) {
    memset(r, 0, sizeof(*r));
    if (boss_anim_lengths(name, &r->nframes, &r->count) != 0) {
        printf("boss: %s frame counts missing in bossdata.bin\n", name);
        return -1;
    }
    return 0;
}

/* BossStageF_OnModelLoaded (modified: the converted packages; data_02074498 = bsf_base, bsf_body, bsf_ball,
 * bsf_shock, bsf_wave_sl, bsf_pas_dust01; NaviBg_Load / NaviB_Load after the last one: HUD, not here), plus
 * the /mod files BossF_Create loads and the zf3_act_lz7.narc ("bsf") files the parts read */
int bossf_load(void) {
    static const char *const files[6] = {"mod__bsf_base__bsf_base.dcm",     "mod__bsf_body__bsf_body.dcm",
                                         "mod__bsf_ball__bsf_ball.dcm",     "mod__bsf_shock__bsf_shock.dcm",
                                         "mod__bsf_wave_sl__bsf_wave_sl.dcm", "mod__bsf_pas_dust01__bsf_pas_dust01.dcm"};
    static const char *const passage[4] = {"mod__bsf_passage__bsf_pas_sate01.dcm", "mod__bsf_passage__bsf_pas_peel01.dcm",
                                           "mod__bsf_passage__bsf_pas_clo00.dcm", "mod__bsf_passage__bsf_pas_clo01.dcm"};
    static const char *const nodes[5] = {"body_core", "body_sh_l", "body_sh_r", "body_fist_l", "body_fist_r"};
    int i, r = 0;
    for (i = 0; i < 6; i++)
        if (!data_ov000_02369b88[i].loaded)
            r |= boss_res_load_model(&data_ov000_02369b88[i], files[i]);
    if (!s_sky.loaded)
        r |= boss_res_load_model(&s_sky, "mod__bsf_sky__bsf_sky.dcm");
    if (!s_sky_b.loaded)
        r |= boss_res_load_model(&s_sky_b, "mod__bsf_sky_b__bsf_sky_b.dcm");
    for (i = 0; i < 4; i++)
        if (!s_passage[i].loaded)
            r |= boss_res_load_model(&s_passage[i], passage[i]);
    if (!s_bsf_ca.loaded)
        r |= boss_res_load_anim(&s_bsf_ca, "narc__zf3_act_lz7__bsf.dca");
    if (!s_bsf_va.loaded)
        r |= boss_res_load_anim(&s_bsf_va, "narc__zf3_act_lz7__bsf.dcv");
    if (!s_laser_fin.loaded)
        r |= boss_res_load_model(&s_laser_fin, "narc__zf3_act_lz7__eff_laser_fin__eff_laser_fin.dcm");
    if (!s_laser_fin_b.loaded)
        r |= boss_res_load_model(&s_laser_fin_b, "narc__zf3_act_lz7__eff_laser_fin_b__eff_laser_fin_b.dcm");
    if (!s_beam_fin.loaded)
        r |= boss_res_load_model(&s_beam_fin, "narc__zf3_act_lz7__eff_beam_fin__eff_beam_fin.dcm");
    r |= load_lengths(&s_bsf_ta, "bsf.nsbta");
    r |= load_lengths(&s_bsf_ma, "bsf.nsbma");
    r |= load_lengths(&s_lfb_ma, "eff_laser_fin_b.nsbma");
    r |= load_lengths(&s_lfb_tp, "eff_laser_fin_b.nsbtp");
    r |= load_lengths(&s_bf_tp, "eff_beam_fin.nsbtp");
    /* "bsf" files 12 / 13: the arm's collision map (bossdata.bin archive chunks) */
    s_st_r_df = BOSS_DATA(0x7f000000, const u8 *);
    s_st_r_di = BOSS_DATA(0x7f010000, const u8 *);
    for (i = 0; i < 5; i++) {
        s_joint[i] = boss_joint_index("bsf_body", nodes[i]); /* data_ov000_02363efc .. 02363f3c */
        if (s_joint[i] < 0)
            r = -1;
    }
    /* gStageMapCollision.unk18: the level width in 64 px blocks (zf3: 26) */
    s_map_w_blocks = 26;
    return r ? -1 : 0;
}

/* BossStageF_FreeModels (+ the other packages) */
void bossf_unload(void) {
    int i;
    for (i = 0; i < 6; i++)
        boss_res_free_model(&data_ov000_02369b88[i]);
    boss_res_free_model(&s_sky);
    boss_res_free_model(&s_sky_b);
    for (i = 0; i < 4; i++)
        boss_res_free_model(&s_passage[i]);
    boss_res_free_anim(&s_bsf_ca);
    boss_res_free_anim(&s_bsf_va);
    boss_res_free_model(&s_laser_fin);
    boss_res_free_model(&s_laser_fin_b);
    boss_res_free_model(&s_beam_fin);
}

/* level width for gStageMapCollision.unk18 (BossFPartD8_Action_StartHandRide) */
void bossf_set_map_width(s32 blocks) {
    if (blocks > 0)
        s_map_w_blocks = blocks;
}

static void D8_collide(void *p) { BossFPartD8_Collide(p); }
static const BossObjOps s_ops_bf = {BossF_Draw, NULL, BossF_Destructor};
static const BossObjOps s_ops_d8 = {BossFPartD8_Draw, D8_collide, BossFPartD8_Destructor};
static const BossObjOps s_ops_d9 = {BossFPartD9_Draw, BossFPartD9_Collide, BossFPartD9_Destructor};
static const BossObjOps s_ops_da = {BossFPartDA_Draw, BossFPartDA_Collide, BossFPartDA_Destructor};
static const BossObjOps s_ops_db = {BossFPartDB_Draw, BossFPartDA_Collide, BossFPartDB_Destructor};
static const BossObjOps s_ops_dc = {BossFPartDC_Draw, BossFPartDC_Collide, BossFPartDC_Destructor};
static const BossObjOps s_ops_dd = {BossFPartDD_Draw, BossFPartDD_Collide, BossFPartDD_Destructor};
static const BossObjOps s_ops_de = {BossFPartDE_Draw, BossFPartDE_Collide, BossFPartDE_Destructor};

void bossf_init(void) {
    static s32 s_zonefx;
    ZoneFx_ZoneFBoss_Init(&s_zonefx); /* StageBackground: the zone F boss background state */
    memset(&s_st, 0, sizeof(s_st));
    memset(s_pal, 0, sizeof(s_pal));
    s_bld_mode = s_bld_evy = 0;
    s_ninst = 0;
    s_pre_ran = 0;
    s_snd_next = 0;
}

void bossf_destroy(void) {
    int i;
    for (i = 0; i < BF_D9; i++)
        if (s_d9[i].alive) {
            BossFPartD9_Destructor(&s_d9[i]);
            s_d9[i].alive = 0;
        }
    for (i = 0; i < BF_DA; i++)
        if (s_da[i].alive) {
            BossFPartDA_Destructor(&s_da[i]);
            s_da[i].alive = 0;
        }
    for (i = 0; i < BF_DB; i++)
        if (s_db[i].alive) {
            BossFPartDB_Destructor(&s_db[i]);
            s_db[i].alive = 0;
        }
    for (i = 0; i < BF_DC; i++)
        if (s_dc[i].alive) {
            BossFPartDC_Destructor(&s_dc[i]);
            s_dc[i].alive = 0;
        }
    for (i = 0; i < BF_DD; i++)
        if (s_dd[i].alive) {
            BossFPartDD_Destructor(&s_dd[i]);
            s_dd[i].alive = 0;
        }
    for (i = 0; i < BF_DE; i++)
        if (s_de[i].alive) {
            BossFPartDE_Destructor(&s_de[i]);
            s_de[i].alive = 0;
        }
    if (s_d8.alive)
        BossFPartD8_Destructor(&s_d8);
    s_d8.alive = 0;
    if (s_bf.alive)
        BossF_Destructor(&s_bf);
    s_bf.alive = 0;
    gBossF = NULL;
    s_ninst = 0;
    set_player_draw(NULL);
}

/* task 0xf0 (BossFPartD8) runs before the player (0x100): the arm's collision map and the hand the
 * player rides move first, as on the DS */
static void run_d8(void) {
    if (s_d8.alive)
        boss_obj_main(&s_d8.h, &s_d8.func, &s_ops_d8, &s_d8, &s_d8.alive);
}
void bossf_update_pre(void) {
    s_ninst = 0;
    run_d8();
    s_pre_ran = 1;
}
void bossf_update(void) {
    int i;
    if (!s_pre_ran) {
        s_ninst = 0;
        run_d8();
    }
    s_pre_ran = 0;
    if (s_bf.alive)
        boss_obj_main(&s_bf.h, &s_bf.func, &s_ops_bf, &s_bf, &s_bf.alive); /* 0x500 */
    for (i = 0; i < BF_D9; i++)                                            /* 0x501 */
        if (s_d9[i].alive)
            bf_obj_main_map(&s_d9[i].h, &s_d9[i].func, &s_ops_d9, &s_d9[i], &s_d9[i].alive);
    for (i = 0; i < BF_DA; i++)
        if (s_da[i].alive)
            boss_obj_main(&s_da[i].h, &s_da[i].func, &s_ops_da, &s_da[i], &s_da[i].alive);
    for (i = 0; i < BF_DB; i++)
        if (s_db[i].alive)
            boss_obj_main(&s_db[i].h, &s_db[i].func, &s_ops_db, &s_db[i], &s_db[i].alive);
    for (i = 0; i < BF_DC; i++)
        if (s_dc[i].alive)
            boss_obj_main(&s_dc[i].h, &s_dc[i].func, &s_ops_dc, &s_dc[i], &s_dc[i].alive);
    for (i = 0; i < BF_DD; i++)
        if (s_dd[i].alive)
            boss_obj_main(&s_dd[i].h, &s_dd[i].func, &s_ops_dd, &s_dd[i], &s_dd[i].alive);
    for (i = 0; i < BF_DE; i++)
        if (s_de[i].alive)
            bf_obj_main_map(&s_de[i].h, &s_de[i].func, &s_ops_de, &s_de[i], &s_de[i].alive);
    ZoneFx_ZoneFBoss_Update(); /* the stage background task (passage models) */
}
/* StageZoneScript (task 0x3eff, between ScreenShake 0x3e00 and BossCamera 0x3f00) */
void bossf_zone_script(void) { StageZoneScript_ZoneFBoss(); }

int bossf_draw(int list) {
    dcm_camera_t cam;
    int i, n = 0;
    if (!s_bf.alive)
        return 0;
    boss_camera(&cam);
    for (i = 0; i < s_ninst; i++) {
        const BossModel *m = s_inst[i].m;
        if (m->res == NULL || !m->res->loaded || (m->flag & 1))
            continue;
#ifdef _arch_dreamcast
        {
            dcm_draw_opts_t o;
            o.list = list;
            o.pt_list = PVR_LIST_PT_POLY;
            o.alpha_scale = s_inst[i].alpha;
            n += dcm_draw_ex(&m->res->mdl, m->palette, &s_inst[i].mtx, &cam, s_inst[i].vis, g_boss_lights, &o,
                             boss_model_matstates(m));
        }
#else
        if (list != 0)
            continue;
        {
            static float xyz[3 * 4096];
            int k, c;
            if (m->res->mdl.hdr->num_vertices > 4096)
                continue;
            (void)boss_model_matstates(m);
            c = dcm_transform_vertices(&m->res->mdl, m->palette, &s_inst[i].mtx, &cam, xyz);
            if (g_boss_debug_fb)
                for (k = 0; k < c; k++) {
                    int x = (int)xyz[3 * k], y = (int)xyz[3 * k + 1];
                    if (x >= 0 && x < 640 && y >= 0 && y < 480 && xyz[3 * k + 2] > 0) {
                        u8 *px = g_boss_debug_fb + 3 * (y * 640 + x);
                        px[0] = (u8)(g_boss_debug_color >> 16);
                        px[1] = (u8)(g_boss_debug_color >> 8);
                        px[2] = (u8)g_boss_debug_color;
                    }
                }
            n += c;
        }
#endif
    }
    return n;
}

void bossf_blend(s32 *mode, s32 *evy) {
    *mode = s_bld_mode;
    *evy = s_bld_evy > 16 ? 16 : s_bld_evy;
}

const BossFStats *bossf_stats(void) { return &s_st; }

int bossf_info(BossFInfo *o) {
    int i;
    memset(o, 0, sizeof(*o));
    if (!s_bf.alive)
        return 0;
    if (s_d8.alive) {
        o->x = s_d8.h.x;
        o->y = s_d8.h.y;
        o->z = s_d8.h.z;
        for (i = 0; i < 3; i++) {
            o->core[i] = s_d8.unk340[9 + i];
            o->fist_l[i] = s_d8.unk3D0[9 + i];
            o->fist_r[i] = s_d8.unk400[9 + i];
            o->sh_r[i] = s_d8.unk3A0[9 + i];
        }
        o->rot = s_d8.unk45E;
        for (i = 0; i < 7; i++) {
            o->box[i] = s_d8.coll[i];
            if (s_d8.coll[i].flags & 4)
                o->boxes_armed |= 1 << i;
        }
        o->cmap_on = s_d8.unk1B8 != NULL;
        o->riding = gPlayers != NULL && BossFPartD8_IsPlayerRiding();
        o->variant = s_d8.unk458;
        o->timer = s_d8.unk434;
    }
    o->ride_draw = g_boss_game.draw_player == BossFPartD8_Draw_RidingPlayer;
    for (i = 0; i < BF_D9; i++)
        o->balls += s_d9[i].alive;
    for (i = 0; i < BF_DA; i++)
        o->shocks += s_da[i].alive;
    for (i = 0; i < BF_DB; i++)
        o->waves += s_db[i].alive;
    for (i = 0; i < BF_DC; i++)
        o->lasers += s_dc[i].alive;
    for (i = 0; i < BF_DD; i++)
        o->flashes += s_dd[i].alive;
    for (i = 0; i < BF_DE; i++)
        o->beams += s_de[i].alive;
    o->sky_model = s_bf.unk23C != NULL;
    o->flags20C = s_bf.unk20C;
    o->map_right = gStageMapCollision_unk24;
    return 1;
}
void bossf_body_pos(s32 *x, s32 *y, s32 *z) {
    *x = s_d8.unk364 >> 4;
    *y = -s_d8.unk368 >> 4;
    *z = s_d8.unk36C >> 4;
}
int bossf_box_armed(int box) { return box >= 0 && box < 7 && s_d8.alive && (s_d8.coll[box].flags & 4); }
s32 bossf_palette(int set) { return set >= 0 && set < 4 ? s_pal[set][0] : -1; }
int bossf_palette_flash(int set) { return set >= 0 && set < 4 ? s_pal[set][1] : 0; }

#define N(f) {(const void *)f, #f}
static const struct {
    const void *f;
    const char *n;
} s_names[] = {
    N(BossF_State_WaitForStart), N(BossF_State_SpawnLaserFinEffects),
    N(BossFPartD8_State_Idle), N(BossFPartD8_State_WaitNextAttack), N(BossFPartD8_State_HandSlamRaise),
    N(BossFPartD8_State_HandSlamRaiseWait), N(BossFPartD8_State_HandSlamNext), N(BossFPartD8_State_HandSlamTrack),
    N(BossFPartD8_State_HandSlamStrike), N(BossFPartD8_State_HandSlamImpact), N(BossFPartD8_State_HandSlamStartRecover),
    N(BossFPartD8_State_HandSlamVulnerable), N(BossFPartD8_State_HandSlamLift), N(BossFPartD8_State_HandSlamLiftWait),
    N(BossFPartD8_State_HandSlamHurt), N(BossFPartD8_State_HandSlamHurtWait), N(BossFPartD8_State_DoublePressStart),
    N(BossFPartD8_State_DoublePressRaise), N(BossFPartD8_State_DoublePressSetTrack), N(BossFPartD8_State_DoublePressTrack),
    N(BossFPartD8_State_DoublePressStrike), N(BossFPartD8_State_DoublePressImpact),
    N(BossFPartD8_State_DoublePressStartVulnerable), N(BossFPartD8_State_DoublePressVulnerable),
    N(BossFPartD8_State_DoublePressEnd), N(BossFPartD8_State_DoublePressEndWait), N(BossFPartD8_State_DoublePressHurt),
    N(BossFPartD8_State_DoublePressHurtWait), N(BossFPartD8_State_PartDAThrowMoveRight),
    N(BossFPartD8_State_PartDAThrowMoveToPos), N(BossFPartD8_State_PartDAThrowStart),
    N(BossFPartD8_State_PartDAThrowStartWait), N(BossFPartD8_State_PartDAThrowSetDelay),
    N(BossFPartD8_State_PartDAThrowDelay), N(BossFPartD8_State_PartDAThrowWindup), N(BossFPartD8_State_PartDAThrowRelease),
    N(BossFPartD8_State_PartDAThrowFollowThrough), N(BossFPartD8_State_PartDAThrowFollowThroughWait),
    N(BossFPartD8_State_PartDAThrowStartVulnerable), N(BossFPartD8_State_PartDAThrowVulnerable),
    N(BossFPartD8_State_PartDAThrowEnd), N(BossFPartD8_State_PartDAThrowEndWait), N(BossFPartD8_State_PartDAThrowHurt),
    N(BossFPartD8_State_PartDAThrowHurtWait), N(BossFPartD8_State_HandRideStartWait), N(BossFPartD8_State_HandRideWait),
    N(BossFPartD8_State_HandRideStartWarn), N(BossFPartD8_State_HandRideWarn), N(BossFPartD8_State_StartHandRideMove),
    N(BossFPartD8_State_HandRideMove), N(BossFPartD8_State_HandRideStartHold), N(BossFPartD8_State_HandRideHold),
    N(BossFPartD8_State_HandRideStartReturn), N(BossFPartD8_State_HandRideReturn), N(BossFPartD8_State_HandRideEnd),
    N(BossFPartD8_State_HandRideEndWait), N(BossFPartD8_State_HandRideRetract), N(BossFPartD8_State_HandRideRetractWait),
    N(BossFPartD8_State_ReturnHome), N(BossFPartD8_State_PartDCVolleyStart), N(BossFPartD8_State_PartDCVolleyRaise),
    N(BossFPartD8_State_PartDCVolleyStartDelay), N(BossFPartD8_State_PartDCVolleyDelay),
    N(BossFPartD8_State_PartDCVolleyFire), N(BossFPartD8_State_PartDCVolleySetInterval),
    N(BossFPartD8_State_PartDCVolleyInterval), N(BossFPartD8_State_PartDCVolleyEnd),
    N(BossFPartD8_State_PartDCVolleyEndWait), N(BossFPartD8_State_PartDEAttackStart),
    N(BossFPartD8_State_PartDEAttackWindup), N(BossFPartD8_State_PartDEAttackStartDelay),
    N(BossFPartD8_State_PartDEAttackDelay), N(BossFPartD8_State_PartDEAttackSpawn), N(BossFPartD8_State_PartDEAttackWait),
    N(BossFPartD8_State_PartDEAttackEnd), N(BossFPartD8_State_PartDEAttackEndWait), N(BossFPartD8_State_Hurt),
    N(BossFPartD8_State_HurtStartRecover), N(BossFPartD8_State_HurtRecover), N(BossFPartD8_State_HurtInSky),
    N(BossFPartD8_State_SkyChargeStartWait), N(BossFPartD8_State_WaitTimer_ov000_022f0f04),
    N(BossFPartD8_State_SkyChargeStartFadeOut), N(BossFPartD8_State_SkyChargeFadeOut), N(BossFPartD8_State_LoadSkyModel),
    N(BossFPartD8_State_WaitTimer_ov000_022f0c8c), N(BossFPartD8_State_SkyChargeStartFadeIn),
    N(BossFPartD8_State_SkyChargeFadeIn), N(BossFPartD8_State_StartWaitTalkDemo), N(BossFPartD8_State_WaitTalkDemo),
    N(BossFPartD8_State_SkyChargeStartRetreat), N(BossFPartD8_State_SkyChargeRetreat),
    N(BossFPartD8_State_SkyChargeStartWait1), N(BossFPartD8_State_WaitTimer_ov000_022f09b4),
    N(BossFPartD8_State_SkyChargeStart1), N(BossFPartD8_State_SkyCharge1), N(BossFPartD8_State_SkyChargeImpact1),
    N(BossFPartD8_State_SkyChargeShake1), N(BossFPartD8_State_SkyChargeStartRetreat1),
    N(BossFPartD8_State_SkyChargeRetreat1), N(BossFPartD8_State_SkyChargeStartWait2),
    N(BossFPartD8_State_WaitTimer_ov000_022f061c), N(BossFPartD8_State_SkyChargeStart2), N(BossFPartD8_State_SkyCharge2),
    N(BossFPartD8_State_SkyChargeImpact2), N(BossFPartD8_State_SkyChargeShake2),
    N(BossFPartD8_State_SkyChargeStartRetreat2), N(BossFPartD8_State_SkyChargeRetreat2),
    N(BossFPartD8_State_SkyChargeStartWait3), N(BossFPartD8_State_WaitTimer_ov000_022f0280),
    N(BossFPartD8_State_SkyChargeStart3), N(BossFPartD8_State_SkyCharge3), N(BossFPartD8_State_SkyChargeImpact3),
    N(BossFPartD8_State_SkyChargeShake3), N(BossFPartD8_State_SkyChargeDestroyBase),
    N(BossFPartD8_State_SkyChargeAftermath), N(BossFPartD8_State_DefeatCamera1), N(BossFPartD8_State_DefeatCamera2),
    N(BossFPartD8_State_DefeatCamera3), N(BossFPartD8_State_DefeatFade_ov000_022ef4ec),
    N(BossFPartD8_State_DefeatFade_ov000_022ef5a4), N(BossFPartD8_State_DefeatFade_ov000_022ef664),
    N(BossFPartD8_State_DefeatExplode), N(BossFPartD8_State_DefeatFinish),
};
#undef N
static const char *name_of(const void *f) {
    unsigned i;
    if (f == NULL)
        return "-";
    for (i = 0; i < sizeof(s_names) / sizeof(s_names[0]); i++)
        if (s_names[i].f == f)
            return strchr(s_names[i].n, '_') + 1;
    return "?";
}
const char *bossf_state_name(int part) {
    if (!s_bf.alive)
        return "-";
    if (part == 0)
        return name_of((const void *)s_bf.func);
    if (part == 1)
        return s_d8.alive ? name_of((const void *)s_d8.func) : "-";
    return "-";
}

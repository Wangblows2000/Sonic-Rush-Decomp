/*
 * Boss7 (Dead Line act 3, stage z73 = stage id 20): native translation of
 * src/game/Zone/Dead_Line/Boss/ (all 167 functions: Boss7_*, Boss7FrameCounter,
 * Boss7PartD5_*, Boss7PartD6_*, Boss7PartD7_*, BossStage7_*), of the one
 * function kept as an ARM listing (src/asm_fallback/
 * Boss7PartD5_InputState_ClashMash.c, rewritten from the listing) and of the
 * stage camera director StageZoneScript_Zone7Boss (src/game/Zone/Dead_Line/Stage).
 *
 * The fight is against the rival, a player-like object driven by an AI that
 * "presses" a virtual pad (Pad_Update) every frame:
 *
 *   Boss7        (task 0x500, map object 0x39)  arena (bs7_base: Eggman's / Nega's pod by the
 *                                               story character), director of the fight and the clash
 *   Boss7PartD5  (0x501, MapObject_Spawn(0xd5)) the rival: Blaze (blz.nsbmd) when the story character
 *                                               is Sonic, Sonic (son.nsbmd) when it is Blaze.  Input
 *                                               states (unk43C) return DS keys, move states (unk440)
 *                                               are the rival's own copy of the player physics
 *   Boss7PartD6  (0x502, 0xd6)  the special's column: Sonic's tornado (eff_tornado, pulls the player)
 *                               or Blaze's fire pillar (eff_flame)
 *   Boss7PartD7  (0x502, 0xd7)  the clash aura around the player / the rival (eff_aura_son / _blz)
 *   Boss7FrameCounter (0x503)   gBoss7->unk220++
 *
 * Health: Boss_GetStartHealth (6 / 8); the rival is beaten at 1 (Boss7PartD5_OnHit),
 * then the defeat scene, the talk (TalkDemo 0xa / 0xb), and the clash: both sides
 * mash A/B (Boss7_UpdateMashPower); unk384 += player power - rival power, |unk384|
 * >= 0x4000 ends it.  Won: health 0, three win cut-ins, the rival jumps away and
 * Stage_OnBossCleared.  Lost: the player is knocked back and the fight stays
 * stopped (Boss7_State_None), as on the DS (the stage timer runs again).
 *
 * Every function keeps its original name and body; the per-file Unk structs are
 * merged with the original offsets in the field names.  DS services replaced:
 *   - Model_Draw -> boss_model_pose (+ the rival's Translate2 op and the
 *     visibility animations, slot 4, drawn by boss7_model_draw)
 *   - the rival's son.bac sprite (AnimatorSprite: hit boxes per frame, the "ended"
 *     bit) -> a native AnimatorSprite on the son.bac command-7 table that
 *     tools/bossdata.py writes to boss7bac.bin
 *   - NNS_G3dGlbLightColor (after-image) -> a second light set (g_boss_lights for
 *     everything else), NNS_G3dGlbMaterialColorDiffAmb + NNSi_G3dModifyMatFlag ->
 *     the bs7_base / bs7_sky material colours
 *   - gDisplayMain BLDCNT/BLDY -> boss7_blend(), master brightness -> boss_brightness(),
 *     BG2 cut-ins -> Boss_DrawExcBackground state, POWCNT / DISPCNT / gDisplaySub
 *     writes dropped (single screen)
 *   - BossHelp_* -> boss_core.c (g_boss_hooks.help), TalkDemo_Create -> boss7_set_talk_hook,
 *     NaviMsg -> g_boss_hooks.navi, sound -> g_boss_hooks.sfx / sfx_handle / music
 *   - 3D BAC sprite effects (EffTrail, EffBoostA(2)_3D, EffFire01_3D,
 *     EffKemuriSpin3D) are not translated: logged ("fx3d") and counted
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "boss_internal.h"
#include "boss7.h"

#ifdef _arch_dreamcast
#include <dc/pvr.h>
#endif

void rect_set_box2d(ObjBox *b, s16 l, s16 t, s16 r, s16 bb); /* ObjRect_SetBox2D */
void obj_player_on_hurt(ObjBox *body, ObjBox *attacker);     /* Player_OnHurt */

/* ------------------------------------------------------------ ROM data */
/* Boss7 read-only tables data_ov000_02363b18 .. 02363da4 (bossdata.bin) */
#define data_ov000_02363b18 BOSS_DATA(0x02363b18, const u16 *)
#define data_ov000_02363b1c BOSS_DATA(0x02363b1c, const u8 *)
#define data_ov000_02363b24 BOSS_DATA(0x02363b24, const u16 *)
#define data_ov000_02363b2c BOSS_DATA(0x02363b2c, const u8 *)
#define data_ov000_02363b34 BOSS_DATA(0x02363b34, const u8 *)
#define data_ov000_02363b3c BOSS_DATA(0x02363b3c, const u16 *)
#define data_ov000_02363b44 BOSS_DATA(0x02363b44, const u16 *)
#define data_ov000_02363b4c BOSS_DATA(0x02363b4c, const u8 *)
#define data_ov000_02363b54 BOSS_DATA(0x02363b54, const u8 *)
#define data_ov000_02363b5c BOSS_DATA(0x02363b5c, const u8 *)
#define data_ov000_02363b64 BOSS_DATA(0x02363b64, const s16 *)
#define data_ov000_02363b6c BOSS_DATA(0x02363b6c, const u16 *)
#define data_ov000_02363b74 BOSS_DATA(0x02363b74, const s32 *)
#define data_ov000_02363b84 BOSS_DATA(0x02363b84, const u16(*)[4])
#define data_ov000_02363b94 BOSS_DATA(0x02363b94, const u16(*)[4])
#define data_ov000_02363ba4 BOSS_DATA(0x02363ba4, const u16(*)[8])
#define data_ov000_02363bc4 BOSS_DATA(0x02363bc4, const u16(*)[8])
#define data_ov000_02363be4 BOSS_DATA(0x02363be4, const u16(*)[8])
#define data_ov000_02363c04 BOSS_DATA(0x02363c04, const u16(*)[4])
#define data_ov000_02363c24 BOSS_DATA(0x02363c24, const s16(*)[4])
#define data_ov000_02363c44 BOSS_DATA(0x02363c44, const u16(*)[8])
#define data_ov000_02363c64 BOSS_DATA(0x02363c64, const s16(*)[4])
#define data_ov000_02363c84 BOSS_DATA(0x02363c84, const u8(*)[8])
#define data_ov000_02363ca4 BOSS_DATA(0x02363ca4, const u8(*)[8])
#define data_ov000_02363cc4 BOSS_DATA(0x02363cc4, const u8(*)[8])
#define data_ov000_02363ce4 BOSS_DATA(0x02363ce4, const u8(*)[8])
#define data_ov000_02363d04 BOSS_DATA(0x02363d04, const u8(*)[8])
#define data_ov000_02363d24 BOSS_DATA(0x02363d24, const u8(*)[8])
#define data_ov000_02363d44 BOSS_DATA(0x02363d44, const u8(*)[8])
#define data_ov000_02363d64 BOSS_DATA(0x02363d64, const u8(*)[8])
#define data_ov000_02363d84 BOSS_DATA(0x02363d84, const u8(*)[8])
/* gPlayerPhysicsTable (0x02368b7c, 2 x 0x30 bytes: Sonic, Blaze) and fields of it */
#define gPlayerPhysicsTable BOSS_DATA(0x02368b7c, const s16(*)[0x18])
#define data_ov000_02368b7e BOSS_DATA(0x02368b7e, const s16(*)[0x18]) /* +0x02 top speed */
#define data_ov000_02368b80 BOSS_DATA(0x02368b80, const s16(*)[0x18]) /* +0x04 deceleration */
#define data_ov000_02368b86 BOSS_DATA(0x02368b86, const s16(*)[0x18]) /* +0x0A spin dash max */
#define data_ov000_02368ba2 BOSS_DATA(0x02368ba2, const s16(*)[0x18]) /* +0x26 jump */
#define data_ov000_02368ba4 BOSS_DATA(0x02368ba4, const s16(*)[0x18]) /* +0x28 gravity */
#define data_ov000_02368ba6 BOSS_DATA(0x02368ba6, const s16(*)[0x18]) /* +0x2A fall max */
/* Player_SetAnimation tables (0x68 entries): joint animation, model, visibility animation, son.bac animation */
#define data_ov000_02368bdc BOSS_DATA(0x02368bdc, const u8 *)
#define data_ov000_02368c44 BOSS_DATA(0x02368c44, const u8 *)
#define data_ov000_02368cac BOSS_DATA(0x02368cac, const u8 *)
#define data_ov000_02368d14 BOSS_DATA(0x02368d14, const u8 *)
/* zone lights: vectors data_020748bc[zone][3] (VecFx16), colours data_02074960[zone][act][3] */
#define data_020748bc BOSS_DATA(0x020748bc, const s16(*)[3][3])
#define data_02074960 BOSS_DATA(0x02074960, const u16(*)[3][3])

#define CAM data_ov000_0236a444
#define gGameState_unk0 (s_story)  /* gGameState.unk0: story character (see boss7_load) */

static inline s32 FX_DivS32(s32 a, s32 b) { return b != 0 ? a / b : 0; }
static inline s32 GetLevel(void) { /* IsMode23() ? 1 : gGameState.unk14 */
    if (g_dcp_world.mode == 2 || g_dcp_world.mode == 3)
        return 1;
    return g_boss_game.unk14;
}
#define GetIdxB GetLevel

/* ------------------------------------------------------------ types */
typedef struct Boss7 Boss7;
typedef struct Boss7PartD5 Boss7PartD5;
typedef struct Boss7PartD6 Boss7PartD6;
typedef struct Boss7PartD7 Boss7PartD7;
typedef u16 (*Boss7InputFn)(Boss7PartD5 *w);
typedef void (*Boss7MoveFn)(Boss7PartD5 *w);

/* Pad (Pad_Reset / Pad_Update, 0x52 bytes at @0x3D8) */
typedef struct Boss7Pad {
    u16 cur;                  /* +0x0 @0x3D8 held */
    u16 prev;                 /* +0x2 */
    u16 trg;                  /* +0x4 @0x3DC pressed */
    u16 rel;                  /* +0x6 */
    u16 rep;                  /* +0x8 */
    u16 cnt[12], delay[12], interval[12];
} Boss7Pad;

/* son.bac command-7 table (tools/bossdata.py -> boss7bac.bin "B7BC") */
typedef struct { u16 first_frame, frame_count; s16 loop_frame; u16 pad; } Boss7BacAnim;
typedef struct { u16 duration, first_event, count, pad; } Boss7BacFrame;
typedef struct { u16 type, len, slot, flags; s16 l, t, r, b; } Boss7BacEvent; /* the BAC command bytes */

/* AnimatorSprite (0x48-byte block) subset: the son.bac timing of the rival */
typedef struct Boss7Spr {
    u16 unk8;                 /* @0x08 animation */
    u16 unkA;                 /* @0x0A frame number (1-based) */
    u16 unkC;                 /* @0x0C frame count */
    s16 unkE;                 /* @0x0E elapsed ticks */
    u16 pos;                  /* @0x1C next frame (command position) */
    s16 wait;                 /* @0x28 */
    s16 unk2A;                /* @0x2A speed (0x10 = one tick per frame) */
    u32 unk2C;                /* @0x2C 1 stopped, 2 ended this frame, 4 loop */
    int has_res;
} Boss7Spr;

struct Boss7 {                /* 0x398 bytes (Boss7_Create) */
    DcPlayer h;
    BossFn func;              /* @0x68 */
    ObjBox coll0, coll1, coll2; /* @0x134 MapObject_Init boxes (no owner) */
    u16 unk1F0;
    u32 unk20C;               /* @0x20C 2 defeat scene done, 0x10 after-image, 0x20 tornado pull */
    s32 unk210;               /* @0x210 camera director case (StageZoneScript_Zone7Boss) */
    Boss7PartD5 *unk214;      /* @0x214 the rival */
    Boss7PartD7 *unk218;      /* @0x218 rival's aura */
    Boss7PartD7 *unk21C;      /* @0x21C player's aura */
    u32 unk220;               /* @0x220 frame counter (Boss7FrameCounter_Main) */
    s32 unk224;               /* @0x224 tornado centre x */
    s16 unk228;               /* @0x228 pull limit */
    s16 unk22A;               /* @0x22A pull speed */
    s16 unk22C;               /* @0x22C pull acceleration */
    u16 unk22E;               /* @0x22E random phase (ClashMash) */
    u16 unk230;               /* @0x230 timer */
    u16 unk232;               /* @0x232 white blend (x256) */
    s32 unk234;               /* @0x234 global material colour (x256) */
    const BossModelRes *unk23C; /* @0x23C bs7_base */
    const BossAnimRes *unk240;  /* @0x240 bs7.nsbca (file 8) */
    const BossAnimRes *unk244;  /* @0x244 bs7.nsbva (file 9) */
    const BossModelRes *unk248; /* @0x248 bs7_sky */
    BossModel unk24C;         /* @0x24C base model (speed @0x360, flags @0x358) */
    s32 unk384;               /* @0x384 clash balance */
    u32 unk388;               /* @0x388 sound handle */
    s32 unk38C;               /* @0x38C rival mash power */
    s32 unk390;               /* @0x390 player mash power */
    u16 unk394;               /* @0x394 clash timer */
    u8 vis[DCM_MAX_JOINTS];   /* native: visibility of the base (bs7.nsbva) */
    int alive;
};

struct Boss7PartD5 {          /* 0x4D8 bytes (Boss7PartD5_Create) */
    DcPlayer h;
    BossFn func;              /* @0x68 */
    ObjBox coll0, coll1, coll2; /* @0x134 / @0x160 / @0x18C StageObj boxes (copied below) */
    u16 unk1F0;               /* @0x1F0 hurt blink timer (MapObject_In_Default) */
    Boss7Spr spr0, spr1;      /* @0x94 / @0xDC (speed @0xBE / @0x106) */
    BossModel unk20C;         /* @0x20C model (flags @0x318, speed @0x320, rot @0x230, trans @0x254) */
    fx32 unk260[3];           /* @0x260 model +0x54: Translate2 (0, -0x10000, 0) */
    int mdl_idx;              /* native: model index set by Model_SetResource (unk138 file) */
    int vis_on;               /* native: slot 4 is attached to the render object */
    const BossAnimRes *unk348; /* @0x348 son.nsbca (unk4C4) */
    const BossAnimRes *unk358; /* @0x358 son.nsbva (unk4C8) */
    ObjBox unk378;            /* @0x378 body box: atk 0x11 def 0, Boss7PartD5_OnHit */
    ObjBox unk3A4;            /* @0x3A4 attack box: atk 0x13 def 0x13 */
    s32 unk3D0;               /* @0x3D0 character: 0 Sonic, 1 Blaze */
    s32 unk3D4;               /* @0x3D4 animation (Boss7_SetRivalAnimation) */
    Boss7Pad unk3D8;          /* @0x3D8 the AI's pad (unk3D8 held, unk3DC pressed) */
    u16 unk42C;               /* @0x42C air dash speed index */
    u16 unk42E;               /* @0x42E attack strength 0..3 */
    s16 unk430, unk432, unk434, unk436, unk438, unk43A; /* @0x430 top speed / 2 and run thresholds */
    Boss7InputFn unk43C;      /* @0x43C input state */
    Boss7MoveFn unk440;       /* @0x440 move state */
    s32 unk444;               /* @0x444 clash mash frames */
    u32 unk448;               /* @0x448 frame of the last clash bounce */
    u16 unk44C;               /* @0x44C attack timer */
    u16 unk44E;               /* @0x44E reposition timer */
    u16 unk450;               /* @0x450 attack table column */
    u16 unk452;               /* @0x452 attack table row */
    u16 unk454;               /* @0x454 input timer */
    s16 unk456;               /* @0x456 walk / roll frames (sign = direction) */
    u16 unk458;               /* @0x458 spin dash charge frames */
    /* @0x464.. used as u16/s32 fields by the specials and as s32 by the defeat scene */
    union {
        struct {
            u16 unk464;       /* @0x464 count (orbits / pillars), run turn timer */
            u16 unk466;       /* @0x466 orbit angle, hover angle, run turn taps */
            s16 unk468;       /* @0x468 orbit angular speed, pillar interval */
            u16 unk46A;       /* @0x46A hover delay */
            s32 unk46C;       /* @0x46C orbit radius */
            s32 unk470;       /* @0x470 orbit timer */
        } sp;
        struct {
            s32 unk464, unk468, unk46C, unk470, unk474, unk478; /* defeat scene arcs */
        } df;
    } u;
    s16 unk47C;               /* @0x47C defeat scene progress */
    void *unk480;             /* @0x480 EffTrail */
    void *unk484;             /* @0x484 EffKemuriSpin3D */
    void *unk488;             /* @0x488 EffBoostA_3D */
    fx32 unk48C[4][3];        /* @0x48C model translations of the last 4 draws (after-image) */
    u16 unk4BC;               /* @0x4BC ring index */
    const BossAnimRes *unk4CC;  /* @0x4CC bs7.nsbca */
    const BossAnimRes *unk4D0;  /* @0x4D0 bs7.nsbva */
    /* native: draw state */
    u8 vis[DCM_MAX_JOINTS];
    int after_posed;
    dcm_mtx_t after_mtx;
    u8 after_vis[DCM_MAX_JOINTS];
    int alive;
};

struct Boss7PartD6 {          /* 0x3A4 bytes (Boss7PartD6_Create) */
    DcPlayer h;
    BossFn func;
    ObjBox coll0, coll1, coll2; /* coll1: the column (2D box -12/-8..12/8 x -256..0) */
    u16 unk1F0;
    BossModel unk20C;         /* @0x20C (flags[1] @0x319, flags[2] @0x31A, speed @0x320) */
    s32 unk378;               /* @0x378 character of the rival */
    u16 unk37C;               /* @0x37C strength (unk42E) */
    BossEffect *unk380;       /* @0x380 eff_suction (tornado) */
    u32 unk384;               /* @0x384 sound handle */
    const BossModelRes *unk388; /* @0x388 eff_tornado / eff_flame */
    const BossAnimRes *unk38C, *unk390, *unk394, *unk398, *unk39C, *unk3A0; /* 01/02/03 .nsbma, 01/02/03 .nsbtp */
    u32 seq;
    int alive;
};

struct Boss7PartD7 {          /* 0x368 bytes (Boss7PartD7_Create) */
    DcPlayer h;
    BossFn func;
    ObjBox coll0, coll1, coll2;
    u16 unk1F0;
    BossModel unk20C;         /* @0x20C (flags[0] @0x318, flags[1] @0x319, speed @0x320) */
    u32 unk344;               /* @0x344 character (p->unk4) */
    DcPlayer *unk348;         /* @0x348 the object it follows */
    const BossModelRes *unk34C; /* @0x34C eff_aura_son / eff_aura_blz */
    const BossAnimRes *unk350, *unk354, *unk358; /* 01/02/03 .nsbca */
    const BossAnimRes *unk35C, *unk360, *unk364; /* 01/02/03 .nsbma */
    u32 seq;
    int alive;
};

/* 3D BAC sprite effects (not translated): the fields the Boss7 code writes */
typedef struct Boss7Fx {
    DcPlayer h;
    u32 unk210;
    Boss7Spr spr1C4;
} Boss7Fx;

#define B7_D6 6
#define B7_D7 2
static Boss7 s_boss7;
static Boss7PartD5 s_d5;
static Boss7PartD6 s_d6[B7_D6];
static Boss7PartD7 s_d7[B7_D7 * 2];
static Boss7 *gBoss7;
static u32 s_seq;
static s32 s_story;           /* gGameState.unk0 */
static int s_counter_task;    /* Boss7FrameCounter_Main task alive */
static Boss7Fx s_fx;
static Boss7Stats s_st;
static int s_won, s_lost, s_cleared_called;
static s32 s_blend_mode, s_blend_evy;   /* gDisplayMain.unk20 (b0 BG0 1st target, b6 effect) / unk24.b0 (BLDY) */
static s32 s_player_spd = 0x1000;       /* gPlayers->unk124->unk114 */
static s16 s_player_spd_anim = -1;
static Boss7TalkFn s_talk_fn;
static void *s_talk_ctx;

/* BossStage7 resources (data_ov000_02369b7c[2] = bs7_base, bs7_sky) and the z73 act archive files */
static BossModelRes data_ov000_02369b7c[2];
static BossAnimRes s_bs7_ca, s_bs7_va;      /* "bs7" files 8 (bs7.nsbca), 9 (bs7.nsbva) */
static BossModelRes s_rival_mdl[2];         /* son.nsbmd / blz.nsbmd models 0 and 1 (ball / spin) */
static BossAnimRes s_son_ca, s_son_va;      /* player_lz7: son.nsbca, son.nsbva */
static BossModelRes s_d6_mdl;               /* eff_tornado (0x12) or eff_flame (0x19) */
static BossAnimRes s_d6_anm[6];             /* 01/02/03 .nsbma, 01/02/03 .nsbtp (timing only) */
static BossModelRes s_d7_mdl[2];            /* eff_aura_son (0x20), eff_aura_blz (0x27) */
static BossAnimRes s_d7_ca[2][3], s_d7_ma[2][3];
/* son.bac command-7 table */
static u8 *s_bac;
static const Boss7BacAnim *s_bac_anims;
static const Boss7BacFrame *s_bac_frames;
static const Boss7BacEvent *s_bac_events;
static u16 s_bac_nanims, s_bac_nframes, s_bac_nevents;
/* lights: the zone lights (g_boss_lights) and the after-image set (NNS_G3dGlbLightColor 0x6182 / 0x1a98) */
static dcm_lights_t s_lights, s_lights_after;
static u16 s_after_col;
static dcm_mtx_t g_boss7_after_pal[DCM_MAX_PALETTE]; /* the after-image's joint palette */
/* bs7_base / bs7_sky material colours: own (as loaded) or the global material colour */
static u32 *s_mat_own[2];
static int s_mat_use_own[2][2] = {{1, 1}, {1, 1}}; /* [model][0 diffuse (0x40), 1 ambient (0x80)] */
static u16 s_glb_diff = 0x7fff, s_glb_amb = 0x7fff;

/* declarations */
#define DECL(f) static void f(void *p);
DECL(Boss7_State_WaitForStart) DECL(Boss7_State_Fight) DECL(Boss7_State_None) DECL(Boss7_State_EndFight)
DECL(Boss7_State_StartEndWait) DECL(Boss7_State_WaitTimer_ov000_022edc70) DECL(Boss7_Action_StartTalkDemo)
DECL(Boss7_State_ClashFadeOutBGM) DECL(Boss7_State_ClashWaitDelay) DECL(Boss7_State_ClashSpawnPartD7)
DECL(Boss7_State_ClashStartFlash) DECL(Boss7_State_ClashFlash) DECL(Boss7_State_ClashStartApproach)
DECL(Boss7_State_ClashApproach) DECL(Boss7_State_ClashStart) DECL(Boss7_State_ClashStruggle)
DECL(Boss7_State_ClashEnd) DECL(Boss7_State_WaitTimer_ov000_022ed138) DECL(Boss7_State_ClashResolve)
DECL(Boss7_State_ClashLost) DECL(Boss7_State_ClashLostRivalSkid) DECL(Boss7_State_ClashWon)
DECL(Boss7_State_WinCut1) DECL(Boss7_State_WinCutWait1) DECL(Boss7_State_WinCut2) DECL(Boss7_State_WinCutWait2)
DECL(Boss7_State_WinCut3) DECL(Boss7_State_WinCutWait3) DECL(Boss7_State_StartRivalExit)
DECL(Boss7_State_PlayerSkidToStop) DECL(Boss7PartD5_State_Idle) DECL(Boss7PartD5_State_RunAI)
DECL(Boss7PartD6_State_Appear) DECL(Boss7PartD6_State_WaitAnimEnd_ov000_022ec34c) DECL(Boss7PartD6_State_Activate)
DECL(Boss7PartD6_State_Active) DECL(Boss7PartD6_State_Vanish) DECL(Boss7PartD6_State_DestroyOnAnimEnd)
DECL(Boss7PartD7_State_Hide) DECL(Boss7PartD7_State_Hidden) DECL(Boss7PartD7_State_StartAppear)
DECL(Boss7PartD7_State_WaitAnimEnd_ov000_022ebf08) DECL(Boss7PartD7_State_StartLoop) DECL(Boss7PartD7_State_Loop)
DECL(Boss7PartD7_State_WaitAnimEnd_ov000_022ebe64) DECL(Boss7PartD7_State_StartDisappear)
DECL(Boss7PartD7_State_WaitAnimEnd_ov000_022ebdc4)
#undef DECL
#define DECLI(f) static u16 f(Boss7PartD5 *w);
DECLI(Boss7PartD5_InputState_None) DECLI(Boss7PartD5_InputState_Decide) DECLI(Boss7PartD5_InputState_Walk)
DECLI(Boss7PartD5_InputState_StartJump) DECLI(Boss7PartD5_InputState_HoldJump) DECLI(Boss7PartD5_InputState_AirSteer)
DECLI(Boss7PartD5_InputState_StartAirAttack) DECLI(Boss7PartD5_InputState_StartJumpThenAirAttack)
DECLI(Boss7PartD5_InputState_HoldJumpThenAirAttack) DECLI(Boss7PartD5_InputState_BeginAirAttackCharge)
DECLI(Boss7PartD5_InputState_HoldAirAttackCharge) DECLI(Boss7PartD5_InputState_StartSpindash)
DECLI(Boss7PartD5_InputState_CrouchTowardTarget) DECLI(Boss7PartD5_InputState_CheckCrouched)
DECLI(Boss7PartD5_InputState_ChargeSpindash) DECLI(Boss7PartD5_InputState_RollThenStop)
DECLI(Boss7PartD5_InputState_StartBackHop) DECLI(Boss7PartD5_InputState_TurnAround)
DECLI(Boss7PartD5_InputState_WaitAfterTurn) DECLI(Boss7PartD5_InputState_TapBack)
DECLI(Boss7PartD5_InputState_TapBackAgain) DECLI(Boss7PartD5_InputState_StartSpecial)
DECLI(Boss7PartD5_InputState_TriggerSpecial) DECLI(Boss7PartD5_InputState_StartSpecialAirWait)
DECLI(Boss7PartD5_InputState_SpecialAirWait) DECLI(Boss7PartD5_InputState_Defeated)
DECLI(Boss7PartD5_InputState_StartClashMash) DECLI(Boss7PartD5_InputState_ClashMash)
#undef DECLI
#define DECLM(f) static void f(Boss7PartD5 *w);
DECLM(Boss7PartD5_MoveState_StartIdle) DECLM(Boss7PartD5_MoveState_Idle) DECLM(Boss7PartD5_MoveState_StartRun)
DECLM(Boss7PartD5_MoveState_Run) DECLM(Boss7PartD5_MoveState_StartJump) DECLM(Boss7PartD5_MoveState_Jump)
DECLM(Boss7PartD5_MoveState_StartCrouch) DECLM(Boss7PartD5_MoveState_Crouch)
DECLM(Boss7PartD5_MoveState_StartSpindashCharge) DECLM(Boss7PartD5_MoveState_SpindashCharge)
DECLM(Boss7PartD5_MoveState_SpindashRelease) DECLM(Boss7PartD5_MoveState_Roll)
DECLM(Boss7PartD5_MoveState_StartBackHop) DECLM(Boss7PartD5_MoveState_BackHop)
DECLM(Boss7PartD5_MoveState_StartAirAttackCharge) DECLM(Boss7PartD5_MoveState_AirAttackCharge)
DECLM(Boss7PartD5_MoveState_StartAirDash) DECLM(Boss7PartD5_MoveState_AirDash)
DECLM(Boss7PartD5_MoveState_StartDashSkid) DECLM(Boss7PartD5_MoveState_DashSkid)
DECLM(Boss7PartD5_MoveState_StartHurt) DECLM(Boss7PartD5_MoveState_Hurt)
DECLM(Boss7PartD5_MoveState_WaitDefeatScene) DECLM(Boss7PartD5_MoveState_DefeatScene)
DECLM(Boss7PartD5_MoveState_DefeatSceneDone) DECLM(Boss7PartD5_MoveState_StartDefeatExit)
DECLM(Boss7PartD5_MoveState_DefeatExit) DECLM(Boss7PartD5_MoveState_StartSonicSpecial)
DECLM(Boss7PartD5_MoveState_SonicSpecialRise) DECLM(Boss7PartD5_MoveState_SonicSpecialStartOrbit)
DECLM(Boss7PartD5_MoveState_SonicSpecialSpinUp) DECLM(Boss7PartD5_MoveState_SonicSpecialOrbit)
DECLM(Boss7PartD5_MoveState_SonicSpecialWaitDashAngle) DECLM(Boss7PartD5_MoveState_SonicSpecialDashToPlayer)
DECLM(Boss7PartD5_MoveState_SonicSpecialStartCharge) DECLM(Boss7PartD5_MoveState_SonicSpecialCharge)
DECLM(Boss7PartD5_MoveState_SonicSpecialSpinDown) DECLM(Boss7PartD5_MoveState_SonicSpecialStartFall)
DECLM(Boss7PartD5_MoveState_SonicSpecialFall) DECLM(Boss7PartD5_MoveState_StartBlazeSpecial)
DECLM(Boss7PartD5_MoveState_BlazeSpecialRise) DECLM(Boss7PartD5_MoveState_BlazeSpecialStartHover)
DECLM(Boss7PartD5_MoveState_BlazeSpecialHoverDelay) DECLM(Boss7PartD5_MoveState_BlazeSpecialSpawnPillar)
DECLM(Boss7PartD5_MoveState_BlazeSpecialHover) DECLM(Boss7PartD5_MoveState_BlazeSpecialEnd)
DECLM(Boss7PartD5_MoveState_BlazeSpecialFall)
#undef DECLM
static void Boss7_SetRivalAnimation(Boss7PartD5 *s, s32 st);
static void Boss7_SetCollidersEnabled(Boss7PartD5 *p, s32 a, s32 b);
static void Boss7PartD5_OnHit(ObjBox *a, ObjBox *b);
static Boss7PartD5 *Boss7PartD5_Create(const ObjParam *p, s32 b, s32 c);
static Boss7PartD6 *Boss7PartD6_Create(const ObjParam *p, s32 b, s32 c);
static Boss7PartD7 *Boss7PartD7_Create(const ObjParam *p, s32 b, s32 c);

/* ============================================================ native services */
/* BossHelp_Create / BossHelp_Destroy: boss_core.c (g_boss_hooks.help, boss_help()) */
static void Boss7_HelpCreate(u32 idx) {
    s_st.helps++;
    BossHelp_Create(idx);
}
static void Boss7_HelpDestroy(void) {
    s_st.help_destroys++;
    BossHelp_Destroy();
}
static void boss_music(s32 seq) {          /* Sound_PlaySeq(gBGMSoundHandle, seq) */
    if (g_boss_hooks.music)
        g_boss_hooks.music(g_boss_hooks.ctx, seq);
}
static void boss_music_stop(void) {        /* NNS_SndPlayerStopSeqAll(0) / NNS_SndPlayerStopSeq(&gBGMSoundHandle, ..) */
    if (g_boss_hooks.music_stop)
        g_boss_hooks.music_stop(g_boss_hooks.ctx);
}

/* gDisplayMain.unk20 = 0; .b0 = 1 (BG0 1st target); .b6 = 2 (brightness up); unk24.b0 = evy */
static void Boss7_SetBlend(s32 evy) {
    s_blend_mode = 2;
    s_blend_evy = evy & 0x1f;
}

/* Model_SetResource(m, m->unk138, idx) for the rival: model idx of son.nsbmd / blz.nsbmd */
static void rival_set_resource(Boss7PartD5 *w, int idx) {
    Model_SetResource(&w->unk20C, &s_rival_mdl[idx & 1]);
    w->mdl_idx = idx & 1;
    w->vis_on = 0; /* NNS_G3dRenderObjInit: animation objects detached from the render object */
}

/* NNSi_G3dModifyMatFlag(model's materials, on, 0x40 diffuse / 0x80 ambient) +
 * NNS_G3dGlbMaterialColorDiffAmb: a material without the flag takes the global colour */
static u32 rgb555_rgb888(u16 c) {
    u32 r = c & 31, g = (c >> 5) & 31, b = (c >> 10) & 31;
    r = (r << 3) | (r >> 2);
    g = (g << 3) | (g >> 2);
    b = (b << 3) | (b >> 2);
    return (r << 16) | (g << 8) | b;
}
static void apply_material_colors(void) {
    int i;
    u32 k, gd = rgb555_rgb888(s_glb_diff), ga = rgb555_rgb888(s_glb_amb);
    for (i = 0; i < 2; i++) {
        dcm_material_t *mat;
        const BossModelRes *r = &data_ov000_02369b7c[i];
        if (!r->loaded || s_mat_own[i] == NULL)
            continue;
        mat = (dcm_material_t *)r->mdl.materials;
        for (k = 0; k < r->mdl.hdr->num_materials; k++) {
            u32 od = s_mat_own[i][k * 2], oa = s_mat_own[i][k * 2 + 1];
            mat[k].diffuse = s_mat_use_own[i][0] ? od : ((od & 0xff000000u) | gd);
            mat[k].ambient = s_mat_use_own[i][1] ? oa : ((oa & 0xff000000u) | ga);
        }
    }
}
static void NNSi_G3dModifyMatFlag_Model(int model, int on, u32 flag) {
    if (flag & 0x40)
        s_mat_use_own[model][0] = on;
    if (flag & 0x80)
        s_mat_use_own[model][1] = on;
    apply_material_colors();
}
static void NNS_G3dGlbMaterialColorDiffAmb(u16 diff, u16 amb, int vtx) {
    (void)vtx;
    s_glb_diff = diff & 0x7fff;
    s_glb_amb = amb & 0x7fff;
    apply_material_colors();
}

/* 3D BAC sprite effects (EffTrail_Create, EffBoostA_3D_Create, EffBoostA2_3D_Create,
 * EffFire01_3D_Create(Following), EffKemuriSpin3D_Create): not translated */
static Boss7Fx *boss7_fx(const char *what, DcPlayer *o, s32 a) {
    s_st.fx3d++;
    boss_log("fx3d", a, o != NULL ? o->x >> 8 : 0);
    (void)what;
    memset(&s_fx, 0, sizeof(s_fx));
    if (o != NULL) {
        s_fx.h.x = o->x;
        s_fx.h.y = o->y;
        s_fx.h.z = o->z;
        s_fx.h.disp_flags = o->disp_flags & 1;
    }
    return &s_fx;
}
static void *EffTrail_Create(Boss7PartD5 *w, s32 a, s32 b, s32 c, s32 d) {
    (void)b; (void)c; (void)d;
    return boss7_fx("EffTrail_Create", &w->h, a);
}
static void EffTrail_StartFollow(void *p) { (void)p; }
static Boss7Fx *EffBoostA_3D_Create(Boss7PartD5 *w, s32 a, s32 b, s32 c, s32 d) {
    (void)b; (void)c; (void)d;
    return boss7_fx("EffBoostA_3D_Create", &w->h, a);
}
static Boss7Fx *EffBoostA2_3D_Create(Boss7PartD5 *w, s32 a, s32 b, s32 c, s32 d) {
    (void)b; (void)c; (void)d;
    return boss7_fx("EffBoostA2_3D_Create", &w->h, a);
}
static Boss7Fx *EffFire01_3D_Create(Boss7PartD5 *w, s16 x, s16 y, s16 z) {
    (void)y; (void)z;
    return boss7_fx("EffFire01_3D_Create", &w->h, x);
}
static Boss7Fx *EffFire01_3D_CreateFollowing(Boss7PartD5 *w, s32 a, s32 b, s32 c) {
    (void)b; (void)c;
    return boss7_fx("EffFire01_3D_CreateFollowing", &w->h, a);
}
static Boss7Fx *EffKemuriSpin3D_Create(Boss7PartD5 *w, s32 a, s32 b, s32 c, s32 d) {
    (void)b; (void)c; (void)d;
    return boss7_fx("EffKemuriSpin3D_Create", &w->h, a);
}

/* Player actions the player module lacks (copied as in boss1.c / boss2.c) */
static void Player_State_Blank(DcPlayer *p) { (void)p; }
static void Player_Action_Blank(DcPlayer *s) {
    dcp_player_init_state(s);
    dcp_reset_gimmick_state(s);
    s->state = Player_State_Blank;
}
static void Player_Action_KnockbackIntangible(DcPlayer *w, s16 vx, s16 vy) {
    dcp_player_init_state(w);
    dcp_set_anim(w, 0xf);
    w->vel_x = vx;
    w->vel_y = vy;
    w->gnd_vel = 0;
    if (vx < 0)
        w->disp_flags &= ~1;
    else
        w->disp_flags |= 1;
    w->obj_flags |= 2;
    w->move_flags |= 0x10;
    w->move_flags |= 0x8000;
    w->move_flags &= ~0x80000u;
    w->move_flags &= ~1u;
    w->state = dcp_state_hurt;
}
/* Player_SetAnimation (state part; the sprite part goes through g_dcp_hooks.anim) */
static void Player_SetAnimation(DcPlayer *p, s32 a) { dcp_set_anim(p, a); }

/* Pad_Reset (copied) */
static void Pad_Reset(Boss7Pad *o, const u16 *a, const u16 *b) {
    s32 i;
    memset(o, 0, sizeof(*o));
    for (i = 0; i < 12; i++)
        o->delay[i] = a != NULL ? a[i] : 0x14;
    for (i = 0; i < 12; i++)
        o->interval[i] = b != NULL ? b[i] : 2;
}
/* Pad_Update (copied) */
static void Pad_Update(Boss7Pad *p, u16 keys) {
    s32 i;
    p->prev = p->cur;
    p->cur = keys;
    p->trg = p->cur & (p->cur ^ p->prev);
    p->rel = p->prev & (p->cur ^ p->prev);
    p->rep = p->trg;
    for (i = 0; i < 12; i++) {
        u32 bit = 1u << i;
        if (!(p->cur & bit)) {
            p->cnt[i] = p->delay[i];
        } else if (p->cnt[i] != 0) {
            p->cnt[i]--;
        } else {
            p->rep |= bit;
            p->cnt[i] = p->interval[i];
        }
    }
}

/* AnimatorSprite_SetAnimation / AnimatorSprite_ProcessAnimation on the son.bac table
 * (the same timing as the objects module's spr_process; command 7 -> callback) */
static void AnimatorSprite_SetAnimation(Boss7Spr *s, s32 anim) {
    s->unk8 = (u16)anim;
    s->unkA = 0;
    s->wait = 0;
    s->unkE = 0;
    s->unkC = 0;
    s->unk2C &= ~3u;
    s->pos = 0;
}
typedef void (*Boss7SprCb)(const Boss7BacEvent *ev, s32 unused, void *owner);
static void AnimatorSprite_ProcessAnimation(Boss7Spr *s, Boss7SprCb cb, void *owner) {
    const Boss7BacAnim *a;
    u16 fi, k;
    if (!s->has_res || s_bac == NULL || s->unk8 >= s_bac_nanims)
        return;
    if (s->unk2C & 1)
        return;
    s->unk2C &= ~2u;
    if (s->wait > 0) {
        s->wait -= s->unk2A;
        s->unkE += s->unk2A;
        if (s->wait > 0)
            return;
        s->unkE += s->wait;
        if (s->unkA == s->unkC) {
            s->unk2C |= 2;
            if (!(s->unk2C & 4)) {
                s->unk2C |= 1;
                return;
            }
        }
    }
    a = &s_bac_anims[s->unk8];
    if (a->frame_count == 0)
        return;
    if (s->pos >= a->frame_count) { /* BAC_FrameGroupFunc_EndAnimation */
        s->unkA = 0;
        s->unkE = 0;
        s->pos = (u16)(a->loop_frame >= 0 ? a->loop_frame : 0);
    }
    fi = (u16)(a->first_frame + s->pos);
    if (fi >= s_bac_nframes)
        return;
    if (cb != NULL)
        for (k = 0; k < s_bac_frames[fi].count; k++)
            if (s_bac_frames[fi].first_event + k < s_bac_nevents)
                cb(&s_bac_events[s_bac_frames[fi].first_event + k], 0, owner);
    s->unkC = a->frame_count; /* BAC_FrameGroupFunc_EndFrame */
    s->unkA = (u16)(s->pos + 1);
    s->wait = (s16)(s_bac_frames[fi].duration << 4);
    s->pos++;
}

/* MapObject_Init (obj_manager.c obj_mapobject_init) fields used */
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
    coll[0].cb = NULL; /* MapObject_OnHit_Default (the Boss7 parts never arm coll0) */
    rect_set_atkdef(&coll[0], 9, 0xa);
    rect_set_atkdef(&coll[1], 0x13, 0x13);
    rect_set_atkdef(&coll[2], 0x13, 0x13);
    for (i = 0; i < 3; i++)
        coll[i].flags &= ~4;
}

/* MapObject_In_Default (copied; MapObject_NetReceiveHits / the unk204 link are VS-only) */
static void MapObject_In_Default(DcPlayer *h, u16 *unk1F0, ObjBox *coll) {
    if (obj_pause_check(h->obj_flags))
        return;
    if (*unk1F0 == 0)
        return;
    (*unk1F0)--;
    if (*unk1F0 & 2)
        h->disp_flags |= 0x20;
    else
        h->disp_flags &= ~0x20;
    coll[0].def |= 5;  /* unk158 */
    coll[1].atk &= ~7u; /* unk180 */
    if (*unk1F0 != 0)
        return;
    h->disp_flags &= ~0x20;
    coll[0].def = 0xa;
    coll[1].atk |= 3;
}

/* MapObject_Collide_Default (copied; owner != NULL -> StageObj_HandleCollider) */
static void MapObject_Collide_Default(DcPlayer *h, void *self, ObjBox *coll) {
    int i;
    if (h->obj_flags & 0xc)
        return;
    for (i = 0; i < 3; i++)
        if (coll[i].owner != NULL)
            obj_handle_collider(h, self, OBJ_OWNER_OBJECT, &coll[i], 0);
}

/* StageObj_Main for the Boss7 objects: as boss_obj_main plus the MapObject
 * In step (unk6C) and the map collision the rival needs (move_flags & 0x100 clear) */
typedef struct {
    BossFn draw, collide, dtor;
} Boss7Ops;
static void boss7_obj_main(DcPlayer *h, BossFn *state, u16 *unk1F0, ObjBox *coll, const Boss7Ops *ops, void *self,
                           int *alive) {
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
    if (!obj_pause_check(h->obj_flags) || (h->obj_flags & 0x40))
        MapObject_In_Default(h, unk1F0, coll);
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

/* Model_Draw: pose (+ the rival's Translate2 op: cb {FlushVP, Identity, RotateTranslate2Scale},
 * v * S * T2 * R * T) and the visibility animation of slot 4 */
static void boss7_pose(BossModel *m, const fx32 *trans2, u8 *vis, int vis_on) {
    boss_model_pose(m, NULL);
    if (!m->posed)
        return;
    if (trans2 != NULL) {
        float t[3];
        int c;
        for (c = 0; c < 3; c++)
            t[c] = trans2[c] / 4096.0f;
        for (c = 0; c < 3; c++) /* t2 * R (unscaled rotation rows) */
            m->mdl_mtx.m[3][c] += t[0] * (m->rot.m[0 * 3 + c] / 4096.0f) + t[1] * (m->rot.m[1 * 3 + c] / 4096.0f) +
                                  t[2] * (m->rot.m[2 * 3 + c] / 4096.0f);
    }
    if (vis != NULL) {
        const BossAnmObj *a = m->anm[4];
        if (vis_on && a != NULL && a->res != NULL && a->res->loaded && a->res->set.hdr != NULL &&
            a->res->set.hdr->kind == DCA_KIND_VIS)
            dca_visibility(&a->res->set, a->idx, (float)(a->frame >> 12), &m->res->mdl, vis);
        else
            memset(vis, 1, DCM_MAX_JOINTS);
    }
}

/* boss_model_draw with a visibility array, a palette and lights of our own */
static int boss7_model_draw(const BossModel *m, const dcm_mtx_t *pal, const dcm_mtx_t *mtx, const u8 *vis,
                            const dcm_lights_t *lights, int list) {
    dcm_camera_t cam;
    dcm_draw_opts_t o;
    if (m->res == NULL || !m->res->loaded || !m->posed || (m->flag & 1))
        return 0;
    boss_camera(&cam);
#ifdef _arch_dreamcast
    o.list = list;
    o.pt_list = PVR_LIST_PT_POLY;
    o.alpha_scale = 255;
    return dcm_draw(&m->res->mdl, pal, mtx, &cam, vis, lights, &o);
#else
    (void)o;
    (void)vis;
    (void)lights;
    if (list != 0)
        return 0;
    {
        static float xyz[3 * 4096];
        int n, i;
        if (m->res->mdl.hdr->num_vertices > 4096)
            return 0;
        n = dcm_transform_vertices(&m->res->mdl, pal, mtx, &cam, xyz);
        if (g_boss_debug_fb)
            for (i = 0; i < n; i++) {
                int x = (int)xyz[3 * i], y = (int)xyz[3 * i + 1];
                if (x >= 0 && x < 640 && y >= 0 && y < 480 && xyz[3 * i + 2] > 0) {
                    u8 *px = g_boss_debug_fb + 3 * (y * 640 + x);
                    px[0] = (u8)(g_boss_debug_color >> 16);
                    px[1] = (u8)(g_boss_debug_color >> 8);
                    px[2] = (u8)g_boss_debug_color;
                }
            }
        return n;
    }
#endif
}

/* MTX_RotY33_(m, data_02071470 / data_0206f470): facing left (0xC000) / right (0x4000) */
static void Boss7_SetFacingRot(BossModel *m, int left) {
    u16 a = left ? 0xc000 : 0x4000;
    MTX_RotY33(&m->rot, SIN(a), COS(a));
}

/* ============================================================ Boss7 */
/* Boss7FrameCounter_Main (copied) */
static void Boss7FrameCounter_Main(void) { gBoss7->unk220++; }

/* Boss7_SetCollidersEnabled (copied) */
static void Boss7_SetCollidersEnabled(Boss7PartD5 *p, s32 a, s32 b) {
    ObjBox *c = &p->unk378;
    if (a)
        c->flags |= 4;
    else
        c->flags &= ~4;
    c = &p->unk3A4;
    if (b)
        c->flags |= 4;
    else
        c->flags &= ~4;
}

/* Boss7_StartPartD5AI (copied) */
static void Boss7PartD5_Action_StartAI(Boss7PartD5 *s);
static void Boss7_StartPartD5AI(void) { Boss7PartD5_Action_StartAI(gBoss7->unk214); }

/* Boss7_SetRivalAnimation (modified: Model_SetResource(m, m->unk138, i) -> the rival model i;
 * son.nsbca / son.nsbva or, for the specials 0x48..0x4a, bs7.nsbca / bs7.nsbva) */
static void Boss7_SetRivalAnimation(Boss7PartD5 *s, s32 st) {
    s32 prev;
    BossModel *m;
    prev = s->unk3D4;
    s->unk3D4 = st;
    if (s->unk3D0 == 1) {
        if (st >= 0x30)
            st += s->unk3D0 * 0x1b;
        if (prev >= 0x30)
            prev += s->unk3D0 * 0x1b;
    }
    if (st < 0 || st >= 0x68)
        st = 0x3c;
    if (prev < 0 || prev >= 0x68)
        prev = 0x3c;
    s->h.disp_flags &= ~8;
    s->h.disp_flags &= ~4;
    s->unk378.flags &= ~4; /* unk382 */
    s->unk3A4.flags &= ~4; /* unk3AE */
    m = &s->unk20C; /* base.unk124 */
    {
        if (data_ov000_02368c44[prev] != data_ov000_02368c44[st]) {
            /* a = s->unk2D0; b = s->unk2D4 (render object joint / material records, kept) */
            rival_set_resource(s, data_ov000_02368c44[st]);
            s->h.disp_flags &= ~0x400;
        }
        if (s->h.disp_flags & 0x400)
            m->flags[0] |= 0x40;
        else
            m->flags[0] &= ~0x40;
        if ((st >= 0x48 && st <= 0x4a) || (st >= 0x63 && st <= 0x65)) {
            Model_SetAnimation(m, 0, s->unk4CC, data_ov000_02368bdc[st]);
            if (data_ov000_02368cac[st] != 0) {
                Model_SetAnimation(m, 4, s->unk4D0, data_ov000_02368cac[st]);
                s->vis_on = 1;
            }
        } else {
            Model_SetAnimation(m, 0, s->unk348, data_ov000_02368bdc[st]); /* unk13C */
            if (data_ov000_02368cac[st] != 0) {
                Model_SetAnimation(m, 4, s->unk358, data_ov000_02368cac[st]); /* unk14C */
                s->vis_on = 1;
            }
        }
        m->speed = 0x1000;
        s->h.disp_flags &= ~0x400;
    }
    s->spr0.unk2A = 0x10;
    s->spr1.unk2A = 0x10;
    AnimatorSprite_SetAnimation(&s->spr0, data_ov000_02368d14[st]);
    AnimatorSprite_SetAnimation(&s->spr1, data_ov000_02368d14[st]);
}

/* Boss7_InitCamera (modified: POWCNT LCD swap dropped; u16 writes to the low halves) */
static void Boss7_InitCamera(void) {
    CAM.unkDC = 1;
    CAM.unkC = 0x1a66000;
    CAM.unk14 = FX_Div(0x1000, 0x34cc);
    CAM.t[0].ox = CAM.t[0].oy = CAM.t[0].oz = NULL;                 /* cam->unk0..8 */
    CAM.t[0].lerp = (CAM.t[0].lerp & ~0xffff) | 0x200;              /* cam->unk30 (u16) */
    CAM.t[0].accel = 0x400;                                         /* cam->unk38 */
    CAM.t[1].ox = CAM.t[1].oy = CAM.t[1].oz = NULL;                 /* cam->unk3C..44 */
    CAM.t[1].lerp = (CAM.t[1].lerp & ~0xffff) | 0x400;              /* cam->unk6C (u16) */
    CAM.t[1].accel = 0x400;                                         /* cam->unk74 */
}

/* Boss7_Draw (modified: Model_Draw -> pose) */
static void Boss7_Draw(void *p) {
    Boss7 *s = (Boss7 *)p;
    s->unk24C.posed = 0;
    if (s->h.disp_flags & 0x20)
        return;
    Model_Animate(&s->unk24C);
    boss7_pose(&s->unk24C, NULL, s->vis, 1);
}

/* Boss7_Destructor (modified: the archive resources are released by boss7_unload;
 * gDisplaySub blend register dropped) */
static void Boss7_Destructor(void *p) {
    Boss7 *s = (Boss7 *)p;
    boss_sfx_stop(s->unk388); /* Sound_FreeHandle */
    boss_model_release(&s->unk24C);
    rect_forget(&s->coll0);
    rect_forget(&s->coll1);
    rect_forget(&s->coll2);
    s_counter_task = 0;
}

/* Boss7_Create (modified: archive files -> the converted packages loaded by boss7_load;
 * NNS_G3dResDefaultSetup of the exc: effects -> boss_effects_load) */
void *Boss7_Create(const ObjParam *p, s32 b, s32 c) {
    Boss7 *w = &s_boss7;
    BossModel *sub;
    if (p != NULL && p->x0 == 0xff && p->y0 == 0xff)
        return NULL;
    boss_model_release(&w->unk24C);
    memset(w, 0, sizeof(*w));
    w->alive = 1;
    s_counter_task = 1; /* Task_Create(Boss7FrameCounter_Main, 0, 0, 0x503, 2, 0) */
    gBoss7 = w;
    w->func = Boss7_State_WaitForStart;
    w->unk22E = Rand();
    w->unk388 = 1; /* Sound_AllocHandle */
    memset(&gBossStatus, 0, sizeof(gBossStatus));
    gBossStatus.unk4 = 0x24;
    gBossStatus.unk8 = (u16)Boss_GetStartHealth();
    gBossStatus.unkA = (u16)Boss_GetStartHealth();
    w->unk240 = &s_bs7_ca; /* NNS_FndGetArchiveFileByIndex(ctx, 8) */
    w->unk244 = &s_bs7_va; /* 9 */
    w->unk23C = &data_ov000_02369b7c[0];
    w->unk248 = &data_ov000_02369b7c[1];
    MapObject_Init(&w->h, &w->coll0, p, b, c);
    w->h.obj_flags |= 0x12;
    w->h.move_flags |= 0xa300;
    w->h.disp_flags |= 0x20;
    w->unk210 = 1;
    w->h.x = 0xa000;
    w->h.y = 0x20000;
    w->h.z = 0;
    NNSi_G3dModifyMatFlag_Model(0, 1, 0x40);
    NNSi_G3dModifyMatFlag_Model(0, 1, 0x80);
    NNSi_G3dModifyMatFlag_Model(1, 1, 0x40);
    NNSi_G3dModifyMatFlag_Model(1, 1, 0x80);
    sub = &w->unk24C;
    Model_Init(sub, 0);
    Model_SetResource(sub, w->unk23C);
    Model_SetAnimation(sub, 0, w->unk240, 3);
    Model_SetAnimation(sub, 4, w->unk244, gGameState_unk0 == 0 ? 4 : 3);
    sub->flags[0] |= 4; /* unk10C */
    sub->flags[4] |= 4; /* unk110 */
    sub->trans[0] = w->h.x << 4;
    sub->trans[1] = -(w->h.y << 4);
    sub->trans[2] = w->h.z << 4;
    sub->scale[0] = sub->scale[1] = sub->scale[2] = 0x34cc;
    /* exc: eff_hit / eff_tornado / eff_flame / eff_aura_son / eff_aura_blz / eff_homing / eff_boost /
     * eff_strike_big / _hit / _fin / eff_clash / eff_suction: set up by boss_effects_load and boss7_load */
    w->unk214 = Boss7PartD5_Create(NULL, b, c); /* MapObject_Spawn(0xd5, b, c, 0, ...) */
    return w;
}

/* Boss7_State_WaitForStart (modified: NNS sky resource -> BossModelRes) */
static void Boss7_State_WaitForStart(void *p) {
    Boss7 *s = (Boss7 *)p;
    BossModel *a;
    Boss7PartD5 *p1;
    if (!(gBossStatus.unk0 & 1))
        return;
    a = &CAM.unkE0; /* data_ov000_0236a524 */
    p1 = s->unk214;
    s->h.disp_flags &= ~0x20;
    p1->h.disp_flags &= ~0x20;
    CAM.unk0 |= 1;
    Model_Init(a, 0);
    Model_SetResource(a, s->unk248);
    a->scale[0] = a->scale[1] = a->scale[2] = 0x1355b50;
    Boss7_InitCamera();
    s->unk210 = 1;
    CAM.t[0].cur.x = 0x9b800;  /* c->unk24 */
    CAM.t[0].cur.y = -0x1a0000;
    CAM.t[0].cur.z = 0xc8000;
    CAM.t[0].tgt = CAM.t[0].cur;
    CAM.t[1].cur.x = 0x9b800;  /* c->unk60 */
    CAM.t[1].cur.y = -0x1a8000;
    CAM.t[1].cur.z = 0;
    CAM.t[1].tgt = CAM.t[1].cur;
    NaviMsg_SetFaceAnim(9);
    NaviMsg_ShowMessage(0xf, 0xf0);
    Boss7_StartPartD5AI();
    s->func = Boss7_State_Fight;
    boss_log("start", s->unk214->unk3D0, gGameState_unk0);
}

/* Boss7_State_Fight (copied) */
static void Boss7_State_Fight(void *p) {
    Boss7 *w = (Boss7 *)p;
    if (w->unk20C & 0x20) {
        s16 m = w->unk228;
        if (gPlayers->invuln > 0) /* unk3B2 */
            m >>= 2;
        w->unk22A += w->unk22C;
        w->unk22A = w->unk22A < 0 ? 0 : (w->unk22A > m ? m : w->unk22A);
        if (w->unk224 < gPlayers->x)
            gPlayers->push_x = -w->unk22A; /* unk36 */
        else
            gPlayers->push_x = w->unk22A;
        s_st.pull_frames++;
    }
    if (w->unk20C & 2)
        w->func = Boss7_State_EndFight;
}

/* Boss7_State_None (copied) */
static void Boss7_State_None(void *p) { (void)p; }

/* Boss7_State_EndFight (copied) */
static void Boss7_State_EndFight(void *p) {
    Boss7 *s = (Boss7 *)p;
    s->unk210 = 3;
    CAM.unkDC = 2;
    s->func = Boss7_State_StartEndWait;
}

/* Boss7_State_StartEndWait (copied) */
static void Boss7_State_StartEndWait(void *p) {
    Boss7 *s = (Boss7 *)p;
    s->unk230 = 0;
    s->func = Boss7_State_WaitTimer_ov000_022edc70;
}

/* Boss7_State_WaitTimer_ov000_022edc70 (copied) */
static void Boss7_State_WaitTimer_ov000_022edc70(void *p) {
    Boss7 *s = (Boss7 *)p;
    if (s->unk230++ > 0x3c)
        s->func = Boss7_Action_StartTalkDemo;
}

/* Boss7_Action_StartTalkDemo (modified: TalkDemo_Create -> the talk hook) */
static void Boss7_Action_StartTalkDemo(void *p) {
    Boss7 *s = (Boss7 *)p;
    u16 gTalkDemoId;
    if (gGameState_unk0 == 0)
        gTalkDemoId = 0xa;
    else
        gTalkDemoId = 0xb;
    s_st.talks++;
    boss_log("talk", gTalkDemoId, 0);
    if (s_talk_fn != NULL) /* TalkDemo_Create: System_Freeze(1, 1, 1) until the talk ends */
        s_talk_fn(s_talk_ctx, gTalkDemoId);
    s->func = Boss7_State_ClashFadeOutBGM;
}

/* Boss7_State_ClashFadeOutBGM (modified: NNS_SndPlayerStopSeq(&gBGMSoundHandle, 0x3c) -> music_stop) */
static void Boss7_State_ClashFadeOutBGM(void *p) {
    Boss7 *s = (Boss7 *)p;
    s->unk230 = 0;
    boss_music_stop();
    s->func = Boss7_State_ClashWaitDelay;
}

/* Boss7_State_ClashWaitDelay (copied) */
static void Boss7_State_ClashWaitDelay(void *p) {
    Boss7 *s = (Boss7 *)p;
    if (s->unk230++ > 0x3c) {
        s->unk214->unk43C = Boss7PartD5_InputState_StartClashMash;
        s->func = Boss7_State_ClashSpawnPartD7;
    }
}

/* Boss7_SpawnPartD7 (modified: MapObject_Spawn(0xd7, 0, 0, chara) -> pool) */
static Boss7PartD7 *Boss7_SpawnPartD7(DcPlayer *p0, s32 p1) {
    ObjParam prm;
    Boss7PartD7 *t0;
    memset(&prm, 0, sizeof(prm));
    prm.type = 0xd7;
    prm.flags = (u16)p1;
    t0 = Boss7PartD7_Create(&prm, 0, 0);
    if (t0 != NULL)
        t0->unk348 = p0;
    return t0;
}

/* Boss7PartD7_Action_Appear (copied) */
static void Boss7PartD7_Action_Appear(Boss7PartD7 *s) {
    if (s != NULL)
        s->func = Boss7PartD7_State_StartAppear;
}

/* Boss7_State_ClashSpawnPartD7 (modified: gGameState.unk0 -> story character) */
static void Boss7_State_ClashSpawnPartD7(void *p) {
    Boss7 *p0 = (Boss7 *)p;
    Boss7PartD5 *t0 = p0->unk214;
    DcPlayer *t1 = gPlayers;
    BossEffect *t4, *t5;
    p0->unk218 = Boss7_SpawnPartD7(&p0->unk214->h, p0->unk214->unk3D0);
    p0->unk21C = Boss7_SpawnPartD7(t1, gGameState_unk0);
    Boss7PartD7_Action_Appear(p0->unk218);
    Boss7PartD7_Action_Appear(p0->unk21C);
    t4 = BossEffect_CreateBoost(1, t1->x, t1->y, t1->z);
    MTX_RotY33(&t4->unk134.rot, SIN(0x4000), COS(0x4000)); /* data_0206f470 */
    t5 = BossEffect_CreateBoost(1, t0->h.x, t0->h.y, t0->h.z);
    MTX_RotY33(&t5->unk134.rot, SIN(0xc000), COS(0xc000)); /* data_02071470 */
    if (t1->chara == 0) /* unk324 */
        Sound_PlaySeqArc(0, 0, 0xa);
    else
        Sound_PlaySeqArc(0, 0, 0x17);
    p0->func = Boss7_State_ClashStartFlash;
}

/* Boss7_State_ClashStartFlash (modified: BLDCNT/BLDY -> boss7_blend, material flags -> colours) */
static void Boss7_State_ClashStartFlash(void *p) {
    Boss7 *s = (Boss7 *)p;
    s->unk230 = 0;
    s->unk232 = 0xc00;
    Boss7_SetBlend(s->unk232 >> 8);
    s->unk234 = 0x1f00;
    NNSi_G3dModifyMatFlag_Model(0, 0, 0x40);
    NNSi_G3dModifyMatFlag_Model(0, 0, 0x80);
    NNSi_G3dModifyMatFlag_Model(1, 0, 0x40);
    NNSi_G3dModifyMatFlag_Model(1, 0, 0x80);
    NNS_G3dGlbMaterialColorDiffAmb(0x7fff, 0x7fff, 0);
    s->func = Boss7_State_ClashFlash;
}

/* Boss7_State_ClashFlash (modified: BLDY -> boss7_blend) */
static void Boss7_State_ClashFlash(void *p) {
    Boss7 *s = (Boss7 *)p;
    u16 v;
    u16 c;
    s->unk232 -= 0x40;
    if ((s16)s->unk232 <= 0)
        s->unk232 = 0;
    s_blend_evy = ((s->unk232 + 0xff) >> 8) & 0x1f; /* gDisplayMain.unk24.b0 */
    s->unk234 -= 0x100;
    if (s->unk234 <= 0x400)
        s->unk234 = 0x400;
    v = (u16)((s->unk234 + 0xff) >> 8);
    c = (u16)(v | (v << 5) | (v << 10));
    NNS_G3dGlbMaterialColorDiffAmb(c, c, 0);
    if (s->unk230++ > 0x78)
        s->func = Boss7_State_ClashStartApproach;
}

/* Boss7_State_ClashStartApproach (copied) */
static void Boss7_State_ClashStartApproach(void *p) {
    Boss7 *p0 = (Boss7 *)p;
    p0->unk218->unk20C.speed = 0x3000; /* +0x320 */
    p0->unk21C->unk20C.speed = 0x3000;
    if (gPlayers->chara == 0)
        Sound_PlaySeqArc(0, 0, 9);
    else
        Sound_PlaySeqArc(0, 0, 0x16);
    p0->func = Boss7_State_ClashApproach;
}

/* Boss7_State_ClashApproach (copied) */
static void Boss7_State_ClashApproach(void *p) {
    Boss7 *w = (Boss7 *)p;
    DcPlayer *b = &w->unk214->h;
    DcPlayer *pl = gPlayers;
    b->vel_x = -0x800;
    pl->gnd_vel = 0x800;
    pl->vel_x = 0x800;
    if (b->x - pl->x > 0x2000)
        return;
    pl->x = ((b->x + pl->x) >> 1) - 0x1000;
    b->x = ((b->x + pl->x) >> 1) + 0x1000;
    b->vel_x = 0;
    pl->gnd_vel = 0;
    pl->vel_x = 0;
    w->unk218->unk20C.speed = 0x1000;
    w->unk21C->unk20C.speed = 0x1000;
    w->func = Boss7_State_ClashStart;
}

/* Boss7_State_ClashStart (modified: BLDY -> boss7_blend, Sound_PlaySeq -> music hook,
 * the player model speed -> boss7_info) */
static void Boss7_State_ClashStart(void *p) {
    Boss7 *s = (Boss7 *)p;
    Boss7PartD5 *o = s->unk214;
    DcPlayer *pl = gPlayers;
    BossEffect_CreateStrikeBig(1, (o->h.x + pl->x) >> 1, (o->h.y + pl->y) >> 1, ((o->h.z + pl->z) >> 1) + 0x1400);
    boss_sfx_handle(s->unk388, 0xbc);
    Sound_PlaySeqArc(0, 0, 0x9a);
    s->unk232 = 0xe00;
    Boss7_SetBlend(s->unk232 >> 8);
    Player_Action_Blank(pl);
    Player_SetAnimation(pl, 0x42);
    pl->disp_flags |= 4;
    s_player_spd = 0x4000; /* ((Mdl *)p->unk124)->unk114 */
    s_player_spd_anim = 0x42;
    Boss7_SetRivalAnimation(o, 0x42);
    Boss7_SetCollidersEnabled(o, 0, 0);
    o->h.disp_flags |= 4;
    o->unk20C.speed = 0xc000;
    s->unk394 = 0;
    ScreenShake_Start(8);
    Boss7_HelpCreate(0); /* BossHelp_Create(0) */
    boss_music(0x11);
    g_dcp_world.flags |= 1; /* gGameState.unk18 |= 1 */
    s->func = Boss7_State_ClashStruggle;
    boss_log("clash", 0, 0);
}

/* Boss7_UpdateMashPower (copied) */
static s32 Boss7_UpdateMashPower(s32 a, s32 b) {
    if (a & 3)
        return b + (((0x80 - b) << 10) >> 12);
    return (b * 0xf80) >> 12;
}

/* Boss7_SetPartD7ScaleFromPower (copied; unk320 = the aura's animation speed) */
static void Boss7_SetPartD7ScaleFromPower(Boss7PartD7 *p, s32 a) {
    s32 v = FX_Div(a << 12, 0x80000);
    if (p == NULL)
        return;
    p->unk20C.speed = (s32)(((s64)v * 0x1000 + 0x800) >> 12) + 0x800;
}

/* Boss7_CreateClashSpark (copied; x/y/z pass through to BossEffect_CreateStrikeHit) */
static BossEffect *Boss7_CreateClashSpark(Boss7 *w, s32 x, s32 y, s32 z) {
    BossEffect *o = BossEffect_CreateStrikeHit(1, x, y, z);
    s32 s;
    o->h.x += (s32)(((Rand() & 0xf) - 8) << 8);
    o->h.y += (s32)(((Rand() & 0xf) - 8) << 8);
    o->h.z += (s32)(((Rand() & 0xf) - 8) << 8);
    s = (FX_DivS32(w->unk390 << 12, 0x80) >> 1) + 0x800;
    o->unk134.scale[0] = FX_Mul(FX_Mul((Rand() & 0xf) << 8, -0x1000) + 0x34cc, s);
    o->unk134.scale[1] = FX_Mul(FX_Mul((Rand() & 0xf) << 8, -0x1000) + 0x34cc, s);
    s_st.sparks++;
    return o;
}

/* Boss7_State_ClashStruggle (modified: gPad.trg -> the player's raw pressed keys, BLDY -> boss7_blend,
 * NNS_SndPlayerSetVolume(gSESoundHandle, 0x30) dropped) */
static void Boss7_State_ClashStruggle(void *p) {
    Boss7 *s = (Boss7 *)p;
    Boss7PartD5 *o;
    DcPlayer *pl;
    s32 m;
    s32 v;
    u16 gPad_trg;
    pl = gPlayers;
    o = s->unk214;
    gPad_trg = pl->pad_raw[1];
    s->unk232 -= 0x40;
    if ((s16)s->unk232 <= 0)
        s->unk232 = 0;
    s_blend_evy = ((s->unk232 + 0xff) >> 8) & 0x1f;
    if (s->unk394 < 0x3c) {
        s->unk394++;
        CAM.unk3C.x = ScreenShake_GetOffsetX() << 14;
        CAM.unk3C.y = ScreenShake_GetOffsetY() << 14;
    } else {
        m = FX_DivS32(s->unk390 << 12, 0x80);
        CAM.unk3C.x = FX_Mul(ScreenShake_GetOffsetX() * 0xccc, m);
        v = FX_Mul(ScreenShake_GetOffsetY() * 0xccc, m);
        CAM.unk3C.y = v;
        CAM.unk3C.z = v;
    }
    s->unk390 = Boss7_UpdateMashPower(gPad_trg, s->unk390);
    s->unk38C = Boss7_UpdateMashPower(s->unk214->unk3D8.trg, s->unk38C); /* unk3DC */
    if (s->unk214->unk3D8.trg & 3)
        s_st.mash_rival++;
    if (gPad_trg & 3) {
        s_st.mash_player++;
        if (s->unk390 > 0x60)
            ScreenShake_Start(4);
        else if (s->unk390 > 0x40)
            ScreenShake_Start(3);
        else if (s->unk390 > 0x20)
            ScreenShake_Start(2);
        else
            ScreenShake_Start(1);
        Boss7_CreateClashSpark(s, (o->h.x + pl->x) >> 1, (o->h.y + pl->y) >> 1, ((o->h.z + pl->z) >> 1) + 0x1000);
        Sound_PlaySeqArc(0, 0, 0xcb);
    }
    Boss7_SetPartD7ScaleFromPower(s->unk21C, s->unk390);
    Boss7_SetPartD7ScaleFromPower(s->unk218, s->unk38C);
    s->unk384 += s->unk390 - s->unk38C;
    if (gPlayers->anim == 0x11 || gPlayers->anim == 0x12 || (gPlayers->pf & 0x400))
        s->unk21C->h.disp_flags |= 0x20;
    v = s->unk384;
    if (v < 0)
        v = -v;
    if (v >= 0x4000)
        s->func = Boss7_State_ClashEnd;
}

/* Boss7_State_ClashEnd (modified: NNS_SndPlayerStopSeqAll -> music_stop) */
static void Boss7_State_ClashEnd(void *p) {
    Boss7 *s = (Boss7 *)p;
    s->unk230 = 0;
    Boss7_HelpDestroy(); /* BossHelp_Destroy() */
    boss_music_stop();
    s->func = Boss7_State_WaitTimer_ov000_022ed138;
}

/* Boss7_State_WaitTimer_ov000_022ed138 (copied) */
static void Boss7_State_WaitTimer_ov000_022ed138(void *p) {
    Boss7 *s = (Boss7 *)p;
    if (s->unk230++ > 10)
        s->func = Boss7_State_ClashResolve;
}

/* Boss7_State_ClashResolve (copied) */
static void Boss7_State_ClashResolve(void *p) {
    Boss7 *p0 = (Boss7 *)p;
    p0->unk21C->unk20C.speed = 0x1000;
    p0->unk218->unk20C.speed = 0x1000;
    if (p0->unk384 <= 0) {
        p0->func = Boss7_State_ClashLost;
        return;
    }
    p0->unk218->h.obj_flags |= 8;
    p0->unk218 = NULL;
    gBossStatus.unk8 = 0;
    p0->func = Boss7_State_ClashWon;
}

/* Boss7_State_ClashLost (copied) */
static void Boss7_State_ClashLost(void *p) {
    Boss7 *w = (Boss7 *)p;
    DcPlayer *pl = gPlayers;
    Boss7PartD5 *b = w->unk214;
    Player_Action_KnockbackIntangible(pl, -0x300, -0x600);
    w->unk21C->func = Boss7PartD7_State_WaitAnimEnd_ov000_022ebe64;
    w->unk21C = NULL;
    b->h.vel_x = -0x200;
    Boss7_SetRivalAnimation(b, 5);
    Boss7_SetCollidersEnabled(b, 0, 0);
    b->unk20C.speed = 0x1000;
    b->h.disp_flags &= ~4;
    w->unk218->func = Boss7PartD7_State_WaitAnimEnd_ov000_022ebe64;
    w->unk218 = NULL;
    BossEffect_CreateStrikeFin(1, (b->h.x + pl->x) >> 1, (b->h.y + pl->y) >> 1, ((b->h.z + pl->z) >> 1) + 0x1000);
    Sound_PlaySeqArc(0, 0, 0x9b);
    w->func = Boss7_State_ClashLostRivalSkid;
    s_lost = 1;
    boss_log("clash_lost", w->unk384, 0);
}

/* Boss7_State_ClashLostRivalSkid (copied) */
static void Boss7_State_ClashLostRivalSkid(void *p) {
    Boss7 *w = (Boss7 *)p;
    Boss7PartD5 *pp = w->unk214;
    s32 st;
    pp->h.x += pp->h.vel_x;
    if (pp->h.vel_x < 0) {
        if (-pp->h.vel_x > 10)
            pp->h.vel_x = (s16)(pp->h.vel_x + 10);
        else
            pp->h.vel_x = 0;
    } else if (pp->h.vel_x > 0) {
        if (pp->h.vel_x > 10)
            pp->h.vel_x = (s16)(pp->h.vel_x - 10);
        else
            pp->h.vel_x = 0;
    }
    st = w->unk214->unk3D4;
    if (st == 5 && (pp->h.disp_flags & 8)) {
        Boss7_SetRivalAnimation(pp, 6);
        pp->h.disp_flags |= 4;
    } else if (pp->h.vel_x < -0x40 && st == 6 && (pp->h.disp_flags & 8)) {
        Boss7_SetRivalAnimation(pp, 7);
        pp->h.disp_flags &= ~4;
    } else if (st == 7 && (pp->h.disp_flags & 8)) {
        Boss7_SetRivalAnimation(pp, 0x3c);
        pp->h.disp_flags |= 4;
    }
    Boss7_SetCollidersEnabled(pp, 0, 0);
    if (w->unk214->unk3D4 != 0x3c)
        return;
    if (pp->h.vel_x == 0)
        w->func = Boss7_State_None;
}

/* Boss7_State_ClashWon (copied) */
static void Boss7_State_ClashWon(void *p) {
    Boss7 *p0 = (Boss7 *)p;
    Boss7PartD5 *r = p0->unk214;
    BossModel *t2;
    p0->unk210 = 0;
    ScreenShake_Start(0);
    p0->h.obj_flags |= 0x20;
    r->h.obj_flags |= 0x20;
    p0->unk21C->h.obj_flags |= 0x20;
    p0->unk24C.speed = 0; /* unk360 */
    r->unk20C.speed = 0;  /* +0x320 */
    p0->unk21C->unk20C.speed = 0;
    StageObj_SetPause();
    t2 = &r->unk20C; /* unk124 */
    Boss7_SetRivalAnimation(r, 0xf);
    if (t2->anm[0] != NULL) /* +0xE4: half of the animation */
        t2->anm[0]->frame = ((s32)t2->anm[0]->numFrames << 12) >> 1;
    if (t2->anm[4] != NULL) /* +0xF4 */
        t2->anm[4]->frame = ((s32)t2->anm[4]->numFrames << 12) >> 1;
    r->h.disp_flags |= 0x10;
    r->h.x += 0x1800;
    r->h.y -= 0x1000;
    r->h.move_flags &= ~0x80u;
    g_dcp_world.flags &= ~1u; /* gGameState.unk18 &= ~1 */
    p0->func = Boss7_State_WinCut1;
    s_won = 1;
    boss_log("defeat", p0->unk384, 0);
}

/* Boss7_State_WinCut1 / 2 / 3 (modified: gDisplayMain.unk48 -> g_boss_game.bright) */
static void Boss7_SetWinCutCamera(Boss7 *s, fx32 dist, fx32 h, u16 ang, int first) {
    Boss7PartD5 *o = s->unk214;
    CAM.unkDC = 2;
    CAM.t[0].lerp = (CAM.t[0].lerp & ~0xffff) | 0x1000;     /* c->unk30 */
    if (first)
        CAM.t[0].accel = 0;                                 /* c->unk38 */
    CAM.t[1].ox = CAM.t[1].oy = CAM.t[1].oz = NULL;         /* c->unk3C..44 */
    CAM.t[1].offx = CAM.t[1].offy = CAM.t[1].offz = 0;      /* c->unk48..50 */
    CAM.t[1].tgt.x = (gPlayers->x + o->h.x) << 3;           /* c->unk54 */
    CAM.t[1].tgt.y = -o->h.y << 4;
    CAM.t[1].tgt.z = 0;
    CAM.t[1].lerp = (CAM.t[1].lerp & ~0xffff) | 0x1000;     /* c->unk6C */
    if (first)
        CAM.t[1].accel = 0;                                 /* c->unk74 */
    CAM.unkC0 = dist;                                       /* c->unk78 */
    CAM.unkC8 = (CAM.unkC8 & ~0xffff) | 0x1000;             /* c->unk80 (u16) */
    CAM.unkCC = h;                                          /* c->unk84 */
    CAM.unkD4 = 0x1000;                                     /* c->unk8C */
    CAM.unkD6 = ang;                                        /* c->unk8E */
    CAM.unkDA = 0x1000;                                     /* c->unk92 */
}
static void Boss7_State_WinCut1(void *p) {
    Boss7 *s = (Boss7 *)p;
    Boss7_SetWinCutCamera(s, 0x37000, 0, 0x71c, 1);
    Boss_DrawExcBackground(6);
    g_boss_game.bright = 0x10;
    Sound_PlaySeqArc(0, 0, 0x7c);
    s->unk230 = 0;
    s->func = Boss7_State_WinCutWait1;
}
static void Boss7_State_WinCut2(void *p) {
    Boss7 *s = (Boss7 *)p;
    Boss7_SetWinCutCamera(s, 0x50000, -0x20000, 0xe38e, 0);
    Boss_DrawExcBackground(7);
    g_boss_game.bright = 0x10;
    Sound_PlaySeqArc(0, 0, 0x7c);
    s->unk230 = 0;
    s->func = Boss7_State_WinCutWait2;
}
static void Boss7_State_WinCut3(void *p) {
    Boss7 *s = (Boss7 *)p;
    Boss7_SetWinCutCamera(s, 0x50000, 0x20000, 0x2aaa, 0);
    Boss_DrawExcBackground(6);
    g_boss_game.bright = 0x10;
    Sound_PlaySeqArc(0, 0, 0x7c);
    s->unk230 = 0;
    s->func = Boss7_State_WinCutWait3;
}
/* Boss7_State_WinCutWait1 / 2 / 3 (copied) */
static void Boss7_WinCutWait(Boss7 *s, BossFn next) {
    if (g_boss_game.bright > 0)
        g_boss_game.bright = (s16)(g_boss_game.bright - 2);
    if (s->unk230++ > 0x1e)
        s->func = next;
}
static void Boss7_State_WinCutWait1(void *p) { Boss7_WinCutWait((Boss7 *)p, Boss7_State_WinCut2); }
static void Boss7_State_WinCutWait2(void *p) { Boss7_WinCutWait((Boss7 *)p, Boss7_State_WinCut3); }
static void Boss7_State_WinCutWait3(void *p) { Boss7_WinCutWait((Boss7 *)p, Boss7_State_StartRivalExit); }

/* Boss7_State_StartRivalExit (modified: DISPCNT BG2 off -> exc_bg = -1, material flags -> colours) */
static void Boss7_State_StartRivalExit(void *p) {
    Boss7 *w = (Boss7 *)p;
    Boss7PartD5 *b2 = w->unk214;
    DcPlayer *pl = gPlayers;
    BossEffect_CreateStrikeFin(1, (b2->h.x + pl->x) >> 1, (b2->h.y + pl->y) >> 1, ((b2->h.z + pl->z) >> 1) + 0x1000);
    CAM.unkDC = 2;
    CAM.t[0].lerp = (CAM.t[0].lerp & ~0xffff) | 0x1000;     /* st->unk30 */
    CAM.t[1].ox = CAM.t[1].oy = CAM.t[1].oz = NULL;
    CAM.t[1].offx = CAM.t[1].offy = CAM.t[1].offz = 0;
    CAM.t[1].tgt.x = b2->h.x << 4;                          /* st->unk54 */
    CAM.t[1].tgt.y = 0x14000 - (b2->h.y << 4);
    CAM.t[1].tgt.z = b2->h.z << 4;
    CAM.t[1].lerp = (CAM.t[1].lerp & ~0xffff) | 0x1000;
    CAM.unkC0 = 0x64000;
    CAM.unkC8 = (CAM.unkC8 & ~0xffff) | 0x1000;
    CAM.unkCC = -0x10000;
    CAM.unkD4 = 0x1000;
    CAM.unkD6 = 0xe000;
    CAM.unkDA = 0x1000;
    g_boss_game.exc_bg = -1; /* SetVisiblePlane(GetVisiblePlane() & ~4) */
    w->h.obj_flags &= ~0x20u;
    w->unk214->h.obj_flags &= ~0x20u;
    w->unk21C->h.obj_flags &= ~0x20u;
    w->unk24C.speed = 0x1000; /* unk360 */
    w->unk214->unk20C.speed = 0x1000;
    w->unk21C->unk20C.speed = 0x1000;
    StageObj_ClearPause();
    pl->vel_x = 0x200;
    Player_SetAnimation(pl, 5);
    pl->disp_flags &= ~4;
    w->unk21C->func = Boss7PartD7_State_WaitAnimEnd_ov000_022ebe64;
    w->unk21C = NULL;
    w->unk214->unk440 = Boss7PartD5_MoveState_StartDefeatExit;
    w->unk214->h.disp_flags &= ~0x10;
    w->unk214->h.move_flags |= 0x80;
    Sound_PlaySeqArc(0, 0, 0x9b);
    NNSi_G3dModifyMatFlag_Model(0, 1, 0x40);
    NNSi_G3dModifyMatFlag_Model(0, 1, 0x80);
    NNSi_G3dModifyMatFlag_Model(1, 1, 0x40);
    NNSi_G3dModifyMatFlag_Model(1, 1, 0x80);
    g_boss_game.bright = 0x10;
    NaviMsg_SetFaceAnim(0xd);
    NaviMsg_ShowMessage(0x13, 0xf0);
    w->unk230 = 0;
    w->func = Boss7_State_PlayerSkidToStop;
}

/* Boss7_State_PlayerSkidToStop (copied; gDisplayMain.unk48 -> g_boss_game.bright) */
static void Boss7_State_PlayerSkidToStop(void *p) {
    Boss7 *w = (Boss7 *)p;
    DcPlayer *pl = gPlayers;
    s16 st;
    w->unk230++;
    if (!(w->unk230 & 3) && g_boss_game.bright > 0)
        g_boss_game.bright--;
    pl->x += pl->vel_x;
    if (pl->vel_x < 0) {
        if (-pl->vel_x > 10)
            pl->vel_x = (s16)(pl->vel_x + 10);
        else
            pl->vel_x = 0;
    } else if (pl->vel_x > 0) {
        if (pl->vel_x > 10)
            pl->vel_x = (s16)(pl->vel_x - 10);
        else
            pl->vel_x = 0;
    }
    st = gPlayers->anim;
    if (st == 5 && (pl->disp_flags & 8)) {
        Player_SetAnimation(pl, 6);
        pl->disp_flags |= 4;
    } else if (pl->vel_x < 0x40 && st == 6 && (pl->disp_flags & 8)) {
        Player_SetAnimation(pl, 7);
        pl->disp_flags &= ~4;
    } else if (st == 7 && (pl->disp_flags & 8)) {
        Player_SetAnimation(pl, 0x3c);
        pl->disp_flags |= 4;
    }
    if (gPlayers->anim != 0x3c)
        return;
    if (pl->vel_x != 0)
        return;
    if (g_boss_game.bright == 0)
        w->func = Boss7_State_None;
}

/* ============================================================ Boss7PartD5: the rival */
/* Boss7PartD5_State_Idle (copied) */
static void Boss7PartD5_State_Idle(void *p) { (void)p; }

/* Boss7PartD5_State_RunAI (copied) */
static void Boss7PartD5_State_RunAI(void *p) {
    Boss7PartD5 *s = (Boss7PartD5 *)p;
    Pad_Update(&s->unk3D8, s->unk43C(s));
    s->unk440(s);
}

/* Boss7PartD5_Action_StartAI (copied) */
static void Boss7PartD5_Action_StartAI(Boss7PartD5 *s) {
    Boss7_InitCamera();
    s->unk43C = Boss7PartD5_InputState_Decide;
    s->func = Boss7PartD5_State_RunAI;
}

/* Boss7PartD5_Action_JumpOverPlayer (copied) */
static void Boss7PartD5_Action_JumpOverPlayer(Boss7PartD5 *s) {
    s->unk456 = (s16)((gPlayers->x - s->h.x) >> 9);
    s->unk43C = Boss7PartD5_InputState_StartJump;
    s_st.jump_overs++;
}

/* Boss7PartD5_IsPlayerRollingToward (copied) */
static dcp_bool Boss7PartD5_IsPlayerRollingToward(Boss7PartD5 *s) {
    DcPlayer *p = gPlayers;
    Boss7 *g = gBoss7;
    s32 d;
    if (p->anim != 0x31)
        return DCP_FALSE;
    if (g->unk220 & data_ov000_02363bc4[GetIdxB()][gBossStatus.unk8 - 1])
        return DCP_FALSE;
    d = s->h.x - p->x;
    if (d < 0)
        d = -d;
    if (d > 0x8000)
        return DCP_FALSE;
    if ((s->h.x < p->x && (p->disp_flags & 1)) || (s->h.x > p->x && !(p->disp_flags & 1)))
        return DCP_TRUE;
    return DCP_FALSE;
}

/* Boss7PartD5_IsPlayerJumpAttacking (copied) */
static dcp_bool Boss7PartD5_IsPlayerJumpAttacking(Boss7PartD5 *s) {
    DcPlayer *p;
    s32 d;
    Boss7 *g = gBoss7;
    if (g->unk220 & data_ov000_02363be4[GetIdxB()][gBossStatus.unk8 - 1])
        return DCP_FALSE;
    p = gPlayers;
    if (p->anim != 0x30)
        return DCP_FALSE;
    d = s->h.x - p->x;
    if (d < 0)
        d = -d;
    if (d < 0x6000) {
        if ((s->h.x < p->x && p->vel_x < 0) || (s->h.x > p->x && p->vel_x > 0))
            return DCP_TRUE;
    }
    return d < 0x3000;
}

/* Boss7PartD5_ChooseEvade (copied) */
static void Boss7PartD5_ChooseEvade(Boss7PartD5 *s) {
    DcPlayer *p = gPlayers;
    s32 x = s->h.x;
    s32 dx = p->x - x;
    s32 adx = dx < 0 ? -dx : dx;
    s32 dy = s->h.y - p->y;
    s_st.evades++;
    if (adx < 0x3000) {
        if (x > 0xa000)
            s->unk456 = (s16)-((Rand() & 0xf) + 0xf);
        else
            s->unk456 = (s16)((Rand() & 0xf) + 0xf);
        s->unk458 = (u16)((Rand() & 0x3f) + 0x3c);
        if (!(Rand() & 1))
            s->unk43C = Boss7PartD5_InputState_StartJump;
        else
            s->unk43C = Boss7PartD5_InputState_StartSpindash;
        return;
    }
    if (adx < 0x4000 && dy > 0x3000 && p->vel_y < 0) {
        s->unk456 = (s16)(dx >> 8);
        s->unk43C = Boss7PartD5_InputState_Walk;
        return;
    }
    if ((s->h.disp_flags & 1 ? 0x14000 - x : x) > 0x4000 && !(Rand() & 1)) {
        if (s->h.x < p->x)
            s->unk456 = -1;
        else
            s->unk456 = 1;
        s->unk43C = Boss7PartD5_InputState_StartBackHop;
        return;
    }
    if (s->h.x > 0xa000)
        s->unk456 = (s16)-((Rand() & 0xf) + 0xf);
    else
        s->unk456 = (s16)((Rand() & 0xf) + 0xf);
    s->unk458 = (u16)((Rand() & 0x3f) + 0x3c);
    s->unk43C = Boss7PartD5_InputState_StartSpindash;
}

/* Boss7PartD5_ChooseAttack (copied) */
static void Boss7PartD5_ChooseAttack(Boss7PartD5 *w) {
    s32 d = w->h.x - gPlayers->x;
    u16 hp;
    u8 k;
    if (d < 0)
        d = -d;
    hp = gBossStatus.unk8;
    if (hp >= 6) {
        if (d < 0x5000)
            k = data_ov000_02363c84[w->unk452][w->unk450];
        else if (d < 0xa000)
            k = data_ov000_02363ca4[w->unk452][w->unk450];
        else
            k = data_ov000_02363cc4[w->unk452][w->unk450];
    } else if (hp >= 4) {
        if (d < 0x5000)
            k = data_ov000_02363ce4[w->unk452][w->unk450];
        else if (d < 0xa000)
            k = data_ov000_02363d04[w->unk452][w->unk450];
        else
            k = data_ov000_02363d24[w->unk452][w->unk450];
    } else {
        if (d < 0x5000)
            k = data_ov000_02363d44[w->unk452][w->unk450];
        else if (d < 0xa000)
            k = data_ov000_02363d64[w->unk452][w->unk450];
        else
            k = data_ov000_02363d84[w->unk452][w->unk450];
    }
    switch (k) {
    case 0:
        break;
    case 1:
        w->unk456 = (s16)((Rand() & 0x1f) + 0x14);
        if (w->h.x > gPlayers->x)
            w->unk456 = (s16)-w->unk456;
        w->unk458 = (u16)((Rand() & 0x3f) + 0x1e);
        w->unk43C = Boss7PartD5_InputState_StartSpindash;
        break;
    case 2:
        w->unk456 = (s16)(Rand() & 0x1f);
        w->unk43C = Boss7PartD5_InputState_StartAirAttack;
        break;
    case 3:
        w->unk43C = Boss7PartD5_InputState_StartSpecial;
        break;
    }
    if (k != 0) {
        g_boss_stats.attacks++;
        s_st.attacks++;
        boss_log("attack", k, gBossStatus.unk8);
    }
    w->unk450++;
    if (w->unk450 >= 8) {
        w->unk450 = 0;
        w->unk452 = Rand() & 3;
    }
}

/* Boss7PartD5_TryReposition (copied) */
static dcp_bool Boss7PartD5_TryReposition(Boss7PartD5 *w) {
    if (w->unk44E > data_ov000_02363ba4[GetLevel()][gBossStatus.unk8 - 1]) {
        s32 x = w->h.x;
        s32 rel = x < 0xa000 ? x : x - 0x14000;
        s32 a = rel < 0 ? -rel : rel;
        s32 d;
        if (a < 0x4000) {
            if (rel > 0)
                w->unk456 = (s16)((Rand() & 0x3f) + 0x1e);
            else
                w->unk456 = (s16)-((Rand() & 0x3f) + 0x1e);
            w->unk43C = Boss7PartD5_InputState_Walk;
            w->unk44E = 0;
            s_st.repositions++;
            return DCP_TRUE;
        }
        d = x - gPlayers->x;
        if (d < 0)
            d = -d;
        if (d < 0x2000) {
            if (x < 0xa000)
                w->unk456 = (s16)((Rand() & 0x3f) + 0x2d);
            else
                w->unk456 = (s16)-((Rand() & 0x3f) + 0x2d);
            w->unk43C = Boss7PartD5_InputState_Walk;
            w->unk44E = 0;
            s_st.repositions++;
            return DCP_TRUE;
        }
    }
    if (w->h.x < gPlayers->x && (w->h.disp_flags & 1)) {
        w->unk456 = 10;
        w->unk43C = Boss7PartD5_InputState_Walk;
        return DCP_TRUE;
    }
    if (w->h.x > gPlayers->x && !(w->h.disp_flags & 1)) {
        w->unk456 = -10;
        w->unk43C = Boss7PartD5_InputState_Walk;
        return DCP_TRUE;
    }
    return DCP_FALSE;
}

/* ---------------------------------------------------- input states (return DS keys) */
/* Boss7PartD5_InputState_None (copied) */
static u16 Boss7PartD5_InputState_None(Boss7PartD5 *w) {
    (void)w;
    return 0;
}
/* Boss7PartD5_InputState_Defeated (copied) */
static u16 Boss7PartD5_InputState_Defeated(Boss7PartD5 *w) {
    (void)w;
    return 0;
}

/* Boss7PartD5_InputState_Decide (copied) */
static u16 Boss7PartD5_InputState_Decide(Boss7PartD5 *w) {
    Boss7 *b = gBoss7;
    if ((u32)(gBossStatus.unk4 - 0x24) <= 1)
        w->unk44C++;
    if ((u32)(gBossStatus.unk4 - 0x24) <= 1)
        w->unk44E++;
    s_st.decides++;
    if (Boss7PartD5_IsPlayerRollingToward(w)) {
        Boss7PartD5_Action_JumpOverPlayer(w);
        return 0;
    }
    if (!(b->unk220 & 0x1f) && Boss7PartD5_IsPlayerJumpAttacking(w)) {
        Boss7PartD5_ChooseEvade(w);
        return 0;
    }
    if (Boss7PartD5_TryReposition(w))
        return 0;
    if (w->unk44C <= data_ov000_02363c44[GetLevel()][gBossStatus.unk8 - 1])
        return 0;
    w->unk44C = 0;
    Boss7PartD5_ChooseAttack(w);
    return 0;
}

/* Boss7PartD5_InputState_Walk (copied) */
static u16 Boss7PartD5_InputState_Walk(Boss7PartD5 *w) {
    u16 r = 0;
    if ((u32)(gBossStatus.unk4 - 0x24) <= 1)
        w->unk44C++;
    if (Boss7PartD5_IsPlayerRollingToward(w)) {
        Boss7PartD5_Action_JumpOverPlayer(w);
        return 0;
    }
    if (Boss7PartD5_IsPlayerJumpAttacking(w)) {
        Boss7PartD5_ChooseEvade(w);
        return 0;
    }
    if (w->unk44C > data_ov000_02363c44[GetLevel()][gBossStatus.unk8 - 1]) {
        w->unk44C = 0;
        Boss7PartD5_ChooseAttack(w);
        return 0;
    }
    if (w->unk456 > 0) {
        w->unk456--;
        r |= 0x10;
    } else if (w->unk456 < 0) {
        w->unk456++;
        r |= 0x20;
    } else {
        w->unk43C = Boss7PartD5_InputState_Decide;
    }
    return r;
}

/* Boss7PartD5_InputState_StartJump (copied) */
static u16 Boss7PartD5_InputState_StartJump(Boss7PartD5 *s) {
    s->unk43C = Boss7PartD5_InputState_HoldJump;
    s->unk454 = 0;
    s->unk43C = Boss7PartD5_InputState_HoldJump;
    return 1;
}

/* Boss7PartD5_InputState_HoldJump (copied) */
static u16 Boss7PartD5_InputState_HoldJump(Boss7PartD5 *w) {
    if (w->unk454++ > 4) {
        if (w->h.move_flags & 1)
            w->unk43C = Boss7PartD5_InputState_Decide;
        else
            w->unk43C = Boss7PartD5_InputState_AirSteer;
        w->unk454 = (u16)((Rand() + 0x14) & 0x1f);
    }
    return 1;
}

/* Boss7PartD5_InputState_AirSteer (copied) */
static u16 Boss7PartD5_InputState_AirSteer(Boss7PartD5 *w) {
    u16 r = 1;
    w->unk44C++;
    if (w->unk454 != 0 && --w->unk454 == 0) {
        if (w->unk44C > data_ov000_02363c44[GetLevel()][gBossStatus.unk8 - 1]) {
            w->unk44C = 0;
            w->unk456 = 0x3c;
            w->unk43C = Boss7PartD5_InputState_StartAirAttack;
            return 0;
        }
    }
    if (w->unk456 > 0) {
        w->unk456--;
        r |= 0x10;
    } else if (w->unk456 < 0) {
        w->unk456++;
        r |= 0x20;
    }
    if (w->h.move_flags & 1)
        w->unk43C = Boss7PartD5_InputState_Decide;
    return r;
}

/* Boss7PartD5_InputState_StartAirAttack (copied) */
static u16 Boss7PartD5_InputState_StartAirAttack(Boss7PartD5 *s) {
    if (s->h.move_flags & 1)
        s->unk43C = Boss7PartD5_InputState_StartJumpThenAirAttack;
    else
        s->unk43C = Boss7PartD5_InputState_BeginAirAttackCharge;
    return 0;
}

/* Boss7PartD5_InputState_StartJumpThenAirAttack (copied) */
static u16 Boss7PartD5_InputState_StartJumpThenAirAttack(Boss7PartD5 *s) {
    s->unk454 = (u16)((Rand() & 0x1f) + 0xf);
    s->unk43C = Boss7PartD5_InputState_HoldJumpThenAirAttack;
    return 0;
}

/* Boss7PartD5_InputState_HoldJumpThenAirAttack (copied) */
static u16 Boss7PartD5_InputState_HoldJumpThenAirAttack(Boss7PartD5 *s) {
    s->unk454--;
    if (s->unk454 == 0)
        s->unk43C = Boss7PartD5_InputState_BeginAirAttackCharge;
    return 1;
}

/* Boss7PartD5_InputState_BeginAirAttackCharge (modified: void in the decompilation; the
 * keys of the tail-called state stay in r0 and reach Pad_Update) */
static u16 Boss7PartD5_InputState_BeginAirAttackCharge(Boss7PartD5 *w) {
    if (gBossStatus.unk8 >= 6) {
        w->unk42E = data_ov000_02363b54[Rand() & 7];
    } else if (gBossStatus.unk8 >= 4) {
        w->unk42E = data_ov000_02363b34[Rand() & 7];
    } else {
        w->unk42E = data_ov000_02363b5c[Rand() & 7];
    }
    w->unk43C = Boss7PartD5_InputState_HoldAirAttackCharge;
    return w->unk43C(w);
}

/* Boss7PartD5_InputState_HoldAirAttackCharge (copied) */
static u16 Boss7PartD5_InputState_HoldAirAttackCharge(Boss7PartD5 *s) {
    if (s->unk456-- == 0)
        s->unk43C = Boss7PartD5_InputState_Decide;
    return 0x100;
}

/* Boss7PartD5_InputState_StartSpindash (copied) */
static u16 Boss7PartD5_InputState_StartSpindash(Boss7PartD5 *s) {
    s->unk454 = 0;
    s->unk43C = Boss7PartD5_InputState_CrouchTowardTarget;
    return 0;
}

/* Boss7PartD5_InputState_CrouchTowardTarget (copied) */
static u16 Boss7PartD5_InputState_CrouchTowardTarget(Boss7PartD5 *p) {
    u16 r = 0x80;
    if (p->unk456 > 0)
        r |= 0x10;
    else
        r |= 0x20;
    if (p->unk454++ > 4)
        p->unk43C = Boss7PartD5_InputState_CheckCrouched;
    return r;
}

/* Boss7PartD5_InputState_CheckCrouched (copied) */
static u16 Boss7PartD5_InputState_CheckCrouched(Boss7PartD5 *p0) {
    if (gBossStatus.unk4 != 0x27) {
        p0->unk43C = Boss7PartD5_InputState_Decide;
        return 0;
    }
    p0->unk43C = Boss7PartD5_InputState_ChargeSpindash;
    return p0->unk43C(p0);
}

/* Boss7PartD5_InputState_ChargeSpindash (copied) */
static u16 Boss7PartD5_InputState_ChargeSpindash(Boss7PartD5 *s) {
    if (s->unk458-- == 0)
        s->unk43C = Boss7PartD5_InputState_RollThenStop;
    return 0x81;
}

/* Boss7PartD5_InputState_RollThenStop (copied) */
static u16 Boss7PartD5_InputState_RollThenStop(Boss7PartD5 *p) {
    u16 r = 0;
    if (p->unk456 > 0) {
        p->unk456--;
    } else if (p->unk456 < 0) {
        p->unk456++;
    } else {
        r |= 0x40;
        p->unk43C = Boss7PartD5_InputState_Decide;
    }
    return r;
}

/* Boss7PartD5_InputState_StartBackHop (modified: void, tail call -> returns its keys) */
static u16 Boss7PartD5_InputState_StartBackHop(Boss7PartD5 *p) {
    if ((p->unk456 > 0 && p != NULL && !(p->h.disp_flags & 1)) || (p->unk456 < 0 && p != NULL && (p->h.disp_flags & 1)))
        p->unk43C = Boss7PartD5_InputState_TurnAround;
    else
        p->unk43C = Boss7PartD5_InputState_TapBack;
    return p->unk43C(p);
}

/* Boss7PartD5_InputState_TurnAround (copied) */
static u16 Boss7PartD5_InputState_TurnAround(Boss7PartD5 *p) {
    u16 r = 0;
    if (p->unk456 > 0)
        r |= 0x20;
    else if (p->unk456 < 0)
        r |= 0x10;
    if ((p->unk456 > 0 && (p->h.disp_flags & 1)) || (p->unk456 < 0 && !(p->h.disp_flags & 1))) {
        p->unk454 = 0;
        p->unk43C = Boss7PartD5_InputState_WaitAfterTurn;
    }
    return r;
}

/* Boss7PartD5_InputState_WaitAfterTurn (copied) */
static u16 Boss7PartD5_InputState_WaitAfterTurn(Boss7PartD5 *s) {
    if (s->unk454++ > 6)
        s->unk43C = Boss7PartD5_InputState_TapBack;
    return 0;
}

/* Boss7PartD5_InputState_TapBack (copied) */
static u16 Boss7PartD5_InputState_TapBack(Boss7PartD5 *p) {
    u16 r = 0;
    if (!(p->h.move_flags & 1)) {
        p->unk43C = Boss7PartD5_InputState_Decide;
        return r;
    }
    if (p->h.disp_flags & 1)
        r |= 0x10;
    else if (!(p->h.disp_flags & 1))
        r |= 0x20;
    p->unk454 = 0;
    p->unk43C = Boss7PartD5_InputState_TapBackAgain;
    return r;
}

/* Boss7PartD5_InputState_TapBackAgain (copied) */
static u16 Boss7PartD5_InputState_TapBackAgain(Boss7PartD5 *p) {
    u16 r = 0;
    if (p->unk454++ > 3) {
        if (p->h.disp_flags & 1)
            r |= 0x10;
        else if (!(p->h.disp_flags & 1))
            r |= 0x20;
        p->unk43C = Boss7PartD5_InputState_Decide;
    }
    return r;
}

/* Boss7PartD5_InputState_StartSpecial (copied) */
static u16 Boss7PartD5_InputState_StartSpecial(Boss7PartD5 *s) {
    if (s->h.move_flags & 1)
        s->unk43C = Boss7PartD5_InputState_TriggerSpecial;
    else
        s->unk43C = Boss7PartD5_InputState_StartSpecialAirWait;
    return 0;
}

/* Boss7PartD5_InputState_TriggerSpecial (copied) */
static u16 Boss7PartD5_InputState_TriggerSpecial(Boss7PartD5 *w) {
    if (gBossStatus.unk8 >= 6) {
        w->unk42E = data_ov000_02363b4c[Rand() & 7];
    } else if (gBossStatus.unk8 >= 4) {
        w->unk42E = data_ov000_02363b1c[Rand() & 7];
    } else {
        w->unk42E = data_ov000_02363b2c[Rand() & 7];
    }
    w->unk43C = Boss7PartD5_InputState_Decide;
    return 4;
}

/* Boss7PartD5_InputState_StartSpecialAirWait (copied) */
static u16 Boss7PartD5_InputState_StartSpecialAirWait(Boss7PartD5 *s) {
    s->unk454 = 0x78;
    s->unk43C = Boss7PartD5_InputState_SpecialAirWait;
    return 0;
}

/* Boss7PartD5_InputState_SpecialAirWait (copied) */
static u16 Boss7PartD5_InputState_SpecialAirWait(Boss7PartD5 *s) {
    s->unk454--;
    if (s->unk454 == 0)
        s->unk43C = Boss7PartD5_InputState_BeginAirAttackCharge;
    return 0;
}

/* Boss7PartD5_InputState_StartClashMash (modified: void, tail call -> returns its keys) */
static u16 Boss7PartD5_InputState_StartClashMash(Boss7PartD5 *s) {
    s->unk444 = 0;
    s->unk43C = Boss7PartD5_InputState_ClashMash;
    return s->unk43C(s);
}

/* Boss7PartD5_InputState_ClashMash (substantially rewritten: from the ARM listing
 * src/asm_fallback/Boss7PartD5_InputState_ClashMash.c).  On odd frames the rival presses A
 * with probability v / 256, where v grows with the player's lead in the clash (unk384),
 * falls off over 1800 frames (unk444) and wobbles with two sines (frame << 6 and
 * unk22E + frame << 5).  The divisions by 16 / 2 / 1800 round toward zero as the listing's
 * sign corrections do; Rand() is the same gRandSeed LCG step. */
static u16 Boss7PartD5_InputState_ClashMash(Boss7PartD5 *s) {
    Boss7 *mgr = gBoss7;
    u16 r = 0;
    if (mgr->unk220 & 1) {
        s32 base = data_ov000_02363b18[GetLevel()];   /* 0x78 / 0xa0 */
        s32 ratio, fall, a, b, d;
        u16 v;
        s->unk444 = s->unk444 + 1;
        if ((u32)s->unk444 > 0x708u)
            s->unk444 = 0x708;
        ratio = FX_Div((mgr->unk384 + 0x4000) << 12, 0x8000000);
        fall = ((0x708 - s->unk444) * 0xb33) / 0x708;
        a = FX_Mul(ratio, fall + 0x4cc);
        b = FX_Mul(a, (SIN(mgr->unk220 << 6) + 0x1000) / 2);
        d = FX_Mul(((base * 10) / 16) * b, (SIN(mgr->unk22E + (mgr->unk220 << 5)) + 0x1000) / 2);
        v = (u16)((base * 6) / 16 + (d >> 12));
        if (v >= (Rand() & 0xff))
            r |= 1;
    }
    return r;
}

/* ---------------------------------------------------- checks used by the move states */
/* Boss7PartD5_CheckCrouch (copied) */
static dcp_bool Boss7PartD5_CheckCrouch(Boss7PartD5 *s) {
    if (s->unk3D8.cur & 0x80) {
        s->unk440 = Boss7PartD5_MoveState_StartCrouch;
        return DCP_TRUE;
    }
    return DCP_FALSE;
}
/* Boss7PartD5_CheckJump (copied) */
static dcp_bool Boss7PartD5_CheckJump(Boss7PartD5 *s) {
    if (s->unk3D8.trg & 1) {
        s->unk440 = Boss7PartD5_MoveState_StartJump;
        return DCP_TRUE;
    }
    return DCP_FALSE;
}
/* Boss7PartD5_CheckRun (copied) */
static dcp_bool Boss7PartD5_CheckRun(Boss7PartD5 *s) {
    if (s->unk3D8.cur & 0x30) {
        s->unk440 = Boss7PartD5_MoveState_StartRun;
        return DCP_TRUE;
    }
    return DCP_FALSE;
}

/* Boss7PartD5_StartSpecialAttack (copied) */
static void Boss7PartD5_StartSpecialAttack(Boss7PartD5 *p) {
    gBossStatus.unk4 = 0x2c;
    s_st.specials++;
    boss_log("special", p->unk3D0, p->unk42E);
    if (p->unk3D0 == 0)
        p->unk440 = Boss7PartD5_MoveState_StartSonicSpecial;
    else
        p->unk440 = Boss7PartD5_MoveState_StartBlazeSpecial;
    p->unk440(p);
}

/* Boss7PartD5_SetAnimSpeedFromVelocity (copied) */
static void Boss7PartD5_SetAnimSpeedFromVelocity(Boss7PartD5 *p, s32 a) {
    s32 t;
    s16 v;
    t = (a >> 5) + (p->h.gnd_vel >> 6);
    if (t < 0)
        t = -t;
    v = (s16)t;
    if (v <= 0x10)
        v = 0x10;
    if (v >= 0x80)
        v = 0x80;
    p->spr0.unk2A = v; /* unkBE */
    p->spr1.unk2A = v; /* unk106 */
    p->unk20C.speed = (v << 12) >> 4; /* unk124->unk114 */
}

/* Boss7PartD5_UpdateRunAnimation (copied) */
static void Boss7PartD5_UpdateRunAnimation(Boss7PartD5 *s) {
    u16 v;
    v = (u16)(s->h.vel_x < 0 ? -s->h.vel_x : s->h.vel_x);
    if (s->unk3D4 < 0x3d || s->unk3D4 > 0x42)
        Boss7_SetRivalAnimation(s, 0x3d);
    if (s->unk3D4 == 0x3d && (s->h.disp_flags & 8) && v >= (u16)s->unk432)
        Boss7_SetRivalAnimation(s, 0x3e);
    if (s->unk3D4 == 0x3e && (s->h.disp_flags & 8)) {
        if (v >= (u16)s->unk434)
            Boss7_SetRivalAnimation(s, 0x3f);
        else
            Boss7_SetRivalAnimation(s, 0x3d);
    }
    if (s->unk3D4 == 0x3f && (s->h.disp_flags & 8)) {
        if (v >= (u16)s->unk436)
            Boss7_SetRivalAnimation(s, 0x40);
        else
            Boss7_SetRivalAnimation(s, 0x3e);
    }
    s->h.disp_flags |= 4;
    Boss7_SetCollidersEnabled(s, 1, 0);
}

/* Boss7PartD5_HoverBob (copied) */
static void Boss7PartD5_HoverBob(Boss7PartD5 *w) {
    w->h.vel_x = 0;
    w->h.y += FX_Mul(SIN(w->u.sp.unk466), 0x4cc) >> 4;
    w->u.sp.unk466 += 0x100;
}

/* Boss7PartD5_UpdateCircleMotion (copied) */
static void Boss7PartD5_UpdateCircleMotion(Boss7PartD5 *w) {
    Boss7 *b = gBoss7;
    w->u.sp.unk466 += w->u.sp.unk468;
    w->h.x = FX_Mul(COS(w->u.sp.unk466), w->u.sp.unk46C) >> 4;
    w->h.x += b->unk224;
    w->h.z = FX_Mul(SIN(w->u.sp.unk466), w->u.sp.unk46C) >> 4;
}

/* Boss7PartD5_SpawnFireParticle (modified: EffFire01_3D_Create not translated) */
static void Boss7PartD5_SpawnFireParticle(Boss7PartD5 *w) {
    s16 dx, dy, dz;
    Boss7Fx *o;
    dx = (s16)((Rand() & 7) - 8);
    dx += w->h.vel_x >> 8;
    dy = (s16)((Rand() & 0xf) - 0xc);
    dy += w->h.vel_y >> 8;
    dz = (s16)((Rand() & 0x1f) - 10);
    o = EffFire01_3D_Create(w, dx, dy, dz);
    if (w->h.disp_flags & 1)
        o->h.vel_x = (s16)((Rand() & 0x3f) + 0x1c0);
    else
        o->h.vel_x = (s16)-((Rand() & 0x3f) + 0x140);
    o->h.vel_y = (s16)-((Rand() & 0x1f) + 0x40);
    o->h.move_flags |= 0x80;
    o->h.gravity = (s16)-((Rand() & 0x1f) + 0x10);
    o->unk210 = (o->unk210 & ~0x3f000000u) | 0x1000000;
}

/* Boss7PartD5_PushAway (copied) */
static void Boss7PartD5_PushAway(DcPlayer *a, DcPlayer *b) {
    s16 v;
    if (a->vel_x > 0)
        v = 0x800;
    else if (a->vel_x > 0)
        v = -0x800;
    else if (a->x < b->x)
        v = 0x800;
    else
        v = -0x800;
    if (b->move_flags & 0x8000)
        b->vel_x = v;
    else
        b->gnd_vel = v;
    b->vel_y += b->vel_y >> 1;
    b->vel_y = (s16)-b->vel_y;
    b->vel_y = b->vel_y < -0x500 ? -0x500 : (b->vel_y > 0x500 ? 0x500 : b->vel_y);
}

/* Boss7PartD5_CreateClashEffect (copied: billboard ops {SetCameraRot, FlushP}) */
static void Boss7PartD5_CreateClashEffect(s32 p0, s32 p1, s32 p2) {
    BossEffect *t0 = BossEffect_CreateClash(1, p0, p1, p2);
    t0->unk134.cb[0] = 0x13; /* +0x13E */
    t0->unk134.cb[1] = 3;
    t0->unk134.cb[2] = 0;
    t0->unk134.scale[0] = 0x1a66; /* +0x14C */
    t0->unk134.scale[1] = 0x1a66;
    t0->unk134.scale[2] = 0x1a66;
}

/* Boss7PartD5_CreateHitEffect (copied) */
static void Boss7PartD5_CreateHitEffect(s32 a, s32 b, s32 c) {
    MtxFx33 mtx;
    BossEffect *obj = BossEffect_CreateHit(1, a, b, c);
    u16 ang = (u16)((Rand() & 0xfff) + 0xaaa);
    MTX_RotX33(&obj->unk134.rot, SIN(ang), COS(ang));
    ang = Rand();
    MTX_RotY33(&mtx, SIN(ang), COS(ang));
    MTX_Concat33(&obj->unk134.rot, &mtx, &obj->unk134.rot);
}

/* Boss7PartD5_OnHit (modified: the owner is checked to be the player, obj_player_on_hurt /
 * obj_player_attack_bounce, NaviMsg / RingManager through hooks).  Inline Hit() copied. */
static void Boss7PartD5_Hit(Boss7 *mgr, Boss7PartD5 *w, DcPlayer *pl) {
    u32 v = mgr->unk220;
    if (v - w->unk448 < 4)
        return;
    w->unk448 = v;
    Boss7PartD5_PushAway(&w->h, pl);
    Boss7PartD5_CreateClashEffect((w->h.x + pl->x) >> 1, (w->h.y + pl->y) >> 1, (w->h.z + pl->z) >> 1);
    Sound_PlaySeqArc(0, 0, 0xcb);
    s_st.clash_bounces++;
    boss_log("bounce", w->h.x >> 8, pl->x >> 8);
}
static void Boss7PartD5_OnHit(ObjBox *a, ObjBox *b) {
    Boss7 *mgr = gBoss7;
    Boss7PartD5 *w = (Boss7PartD5 *)a->owner;
    DcPlayer *pl = obj_box_player(b);
    u16 hp = gBossStatus.unk8;
    s32 spd;
    if (pl == NULL || w == NULL)
        return;
    if (hp == 1)
        return;
    if (gBossStatus.unk4 == 0x29) {
        if (pl->invuln > 0)
            return;
        if ((u16)(s16)(pl->anim - 0x30) <= 1) {
            Boss7PartD5_Hit(mgr, w, pl);
            return;
        }
        if (pl->invuln > 0)
            return;
        obj_player_on_hurt(b, a);
        return;
    }
    if (gBossStatus.unk4 == 0x28) {
        if (pl->anim == 0x30) {
            Boss7PartD5_Hit(mgr, w, pl);
            return;
        }
        if (pl->invuln > 0)
            return;
        obj_player_on_hurt(b, a);
        return;
    }
    if ((u16)(s16)(pl->anim - 0x30) <= 1) {
        gBossStatus.unk8 = hp - 1;
        g_boss_stats.hits++;
        if (gBossStatus.unk8 <= 1) {
            gBossStatus.unk8 = 1;
            g_dcp_world.flags &= ~1u; /* gGameState.unk18 &= ~1 */
            gPlayers->pf |= 0x200000;
            gPlayers->held = 0;     /* unk420 */
            gPlayers->pressed = 0;  /* unk422 */
            gPlayers->released = 0; /* unk424 */
            if (g_boss_hooks.ring_flags) /* RingManager_SetFlags(RingManager_GetFlags() | 1) */
                g_boss_hooks.ring_flags(g_boss_hooks.ctx, 1);
            NaviMsg_SetFaceAnim(0xb);
            NaviMsg_ShowMessage(0x12, 0xf0);
            w->unk43C = Boss7PartD5_InputState_Defeated;
        } else {
            NaviMsg_SetFaceAnim(0xb);
            NaviMsg_ShowMessage(0x10, 0xf0);
        }
        boss_log("hit", gBossStatus.unk8, pl->anim);
        w->unk440 = Boss7PartD5_MoveState_StartHurt;
        obj_player_attack_bounce(gPlayers);
        Boss7PartD5_CreateHitEffect(b->x << 8, b->y << 8, 0);
        return;
    }
    if (pl->invuln > 0)
        return;
    if (w->h.x < pl->x) {
        pl->push_x = (s16)((((a->x + a->r) - b->l) << 8) - pl->x);
        spd = 0x400;
    } else {
        pl->push_x = (s16)((((a->x + a->l) - b->r) << 8) - pl->x);
        spd = -0x400;
    }
    pl->push_x >>= 1;
    w->h.push_x = (s16)-pl->push_x;
    if (pl->move_flags & 0x8000)
        pl->vel_x = (s16)spd;
    else
        pl->gnd_vel = (s16)spd;
    w->h.vel_x = (s16)-spd;
    s_st.pushes++;
}

/* Boss7PartD5_SpriteCallback (copied: command 7 sets the box geometry, keeps its armed bit) */
static void Boss7PartD5_SpriteCallback(const Boss7BacEvent *p0, s32 p1, void *owner) {
    Boss7PartD5 *p2 = (Boss7PartD5 *)owner;
    ObjBox *box;
    u16 t2;
    (void)p1;
    if (p0->type != 7)
        return;
    if (p0->slot > 1)
        return;
    box = p0->slot == 0 ? &p2->unk378 : &p2->unk3A4;
    t2 = box->flags;
    box->owner = p2; /* +0x390 */
    rect_set_box2d(box, p0->l, p0->t, p0->r, p0->b);
    if (t2 & 4)
        box->flags |= 4;
    else
        box->flags &= ~4;
}

/* Boss7PartD5_Collide (copied) */
static void Boss7PartD5_Collide(void *p) {
    Boss7PartD5 *t0 = (Boss7PartD5 *)p;
    if (t0->h.obj_flags & 0xc)
        return;
    if ((s16)t0->unk1F0 > 0)
        return;
    if (t0->unk378.owner != NULL)
        obj_handle_collider(&t0->h, t0, OBJ_OWNER_OBJECT, &t0->unk378, 1);
    if (t0->unk3A4.owner != NULL)
        obj_handle_collider(&t0->h, t0, OBJ_OWNER_OBJECT, &t0->unk3A4, 1);
    /* unk1B8 (ObjCollisionMap): none */
}

/* Boss7PartD5_Draw (modified: Model_Draw -> boss7_pose; the after-image (gBoss7->unk20C & 0x10:
 * an older position drawn with light colour 0x6182 Sonic / 0x1a98 Blaze, then the zone light
 * colours data_02074960) keeps its own pose and light set) */
static void Boss7PartD5_Draw(void *p) {
    Boss7PartD5 *w = (Boss7PartD5 *)p;
    Boss7 *mgr = gBoss7;
    BossModel *sub = &w->unk20C;
    u16 i;
    int c;
    sub->posed = 0;
    w->after_posed = 0;
    if (w->h.disp_flags & 0x20)
        return;
    w->unk4BC++;
    w->unk4BC &= 3;
    for (c = 0; c < 3; c++)
        w->unk48C[w->unk4BC][c] = sub->trans[c];
    if (mgr->unk20C & 0x10) {
        i = (w->unk4BC + (mgr->unk220 & 1)) & 3;
        s_after_col = w->unk3D0 == 0 ? 0x6182 : 0x1a98; /* NNS_G3dGlbLightColor(0..2, ..) */
        for (c = 0; c < 3; c++)
            sub->trans[c] = w->unk48C[i][c];
        boss7_pose(sub, w->unk260, w->after_vis, w->vis_on);
        if (sub->posed && sub->palette != NULL) {
            memcpy(g_boss7_after_pal, sub->palette, sub->res->mdl.hdr->num_palette * sizeof(dcm_mtx_t));
            w->after_mtx = sub->mdl_mtx;
            w->after_posed = 1;
        }
        s_st.afterimage_frames++;
        /* NNS_G3dGlbLightColor(i, data_02074960[zone][act][i]): the zone set again */
    }
    w->h.disp_flags &= ~8;
    if (w->h.disp_flags & 4) {
        sub->flags[0] |= 4; /* unk10C */
        sub->flags[4] |= 4; /* unk110 */
    } else {
        sub->flags[0] &= ~4;
        sub->flags[4] &= ~4;
    }
    Boss7_SetFacingRot(sub, w->h.disp_flags & 1);
    sub->trans[0] = w->h.x << 4;
    sub->trans[1] = -(w->h.y << 4);
    sub->trans[2] = w->h.z << 4;
    if (!(w->h.disp_flags & 0x10)) {
        AnimatorSprite_ProcessAnimation(&w->spr0, Boss7PartD5_SpriteCallback, w);
        Model_Animate(sub);
    }
    boss7_pose(sub, w->unk260, w->vis, w->vis_on);
    if (sub->flags[0] & 2)
        w->h.disp_flags |= 8;
    if (w->spr0.unk2C & 2)
        w->h.disp_flags |= 8;
}
/* Boss7PartD5_Destructor (modified) */
static void Boss7PartD5_Destructor(void *p) {
    Boss7PartD5 *s = (Boss7PartD5 *)p;
    boss_model_release(&s->unk20C);
    rect_forget(&s->unk378);
    rect_forget(&s->unk3A4);
    rect_forget(&s->coll0);
    rect_forget(&s->coll1);
    rect_forget(&s->coll2);
}

/* Boss7PartD5_Create (modified: StageObj_LoadModel / LoadMotion / Load2dBAC -> the converted
 * player packages and the son.bac table; gGameState.unk0 -> story character) */
static Boss7PartD5 *Boss7PartD5_Create(const ObjParam *p, s32 b, s32 c) {
    Boss7 *mgr = gBoss7;
    Boss7PartD5 *w = &s_d5;
    ObjBox *coll;
    BossModel *sub;
    boss_model_release(&w->unk20C);
    memset(w, 0, sizeof(*w));
    w->alive = 1;
    mgr->unk214 = w;
    w->func = Boss7PartD5_State_Idle;
    w->unk43C = Boss7PartD5_InputState_None;
    w->unk440 = Boss7PartD5_MoveState_StartIdle;
    w->unk452 = Rand() & 3;
    Pad_Reset(&w->unk3D8, NULL, NULL);
    w->unk4CC = &s_bs7_ca; /* "bs7" file 8 */
    w->unk4D0 = &s_bs7_va; /* 9 */
    sub = &w->unk20C;
    Model_Init(sub, 0);
    if (gGameState_unk0 == 0) {
        w->unk3D0 = 1; /* Blaze: blz.nsbmd */
    } else {
        w->unk3D0 = 0; /* Sonic: son.nsbmd */
    }
    rival_set_resource(w, 0); /* StageObj_LoadModel(.., model 0) */
    w->unk348 = &s_son_ca;    /* son.nsbca */
    w->unk358 = &s_son_va;    /* son.nsbva */
    w->spr0.has_res = 1;      /* StageObj_Load2dBAC("/act/son.bac") */
    MapObject_Init(&w->h, &w->coll0, p, b, c);
    w->h.obj_flags |= 0x10;
    w->h.move_flags |= 0x88080;
    w->h.disp_flags |= 0x20;
    w->h.x = b;
    w->h.y = c;
    w->h.z = mgr->h.z;
    w->unk430 = (s16)(data_ov000_02368b7e[w->unk3D0][0] / 2);
    w->unk432 = (s16)(w->unk430 * 0.15f);
    w->unk434 = (s16)(w->unk430 * 0.3f);
    w->unk436 = (s16)(w->unk430 * 0.5f);
    w->unk438 = (s16)(w->unk430 * 0.7f);
    w->unk43A = (s16)(w->unk430 * 0.9f);
    w->h.gravity = data_ov000_02368ba4[w->unk3D0][0];  /* unk40 */
    w->h.fall_max = data_ov000_02368ba6[w->unk3D0][0]; /* unk42 */
    w->h.box_l = -6;  /* unk64 */
    w->h.box_r = 6;   /* unk66 */
    w->h.box_t = -12; /* unk65 */
    w->h.box_b = 13;  /* unk67 */
    coll = &w->unk378;
    *coll = w->coll0;
    rect_set_atkdef(coll, 0x11, 0);
    coll->flags |= 0x40;
    coll->cb = Boss7PartD5_OnHit;
    coll->owner = w;
    coll->owner_kind = OBJ_OWNER_OBJECT;
    coll = &w->unk3A4;
    *coll = w->coll1;
    coll->owner = w;
    coll->owner_kind = OBJ_OWNER_OBJECT;
    sub->scale[0] = sub->scale[1] = sub->scale[2] = 0x34cc;
    w->unk260[1] = -0x10000; /* sub->unk58 */
    sub->cb[0] = 4;          /* unkA: FlushVP */
    sub->cb[1] = 1;          /* unkB: Identity */
    sub->cb[2] = 0xf;        /* unkC: RotateTranslate2Scale */
    w->spr0.unk2A = 0x10;    /* AnimatorSprite_Init(&spr0, son.bac, 0, 0x18, ...) */
    w->spr0.unk2C = 0x18;
    Boss7_SetRivalAnimation(w, 0x3c);
    w->h.disp_flags |= 4;
    return w;
}

/* ---------------------------------------------------- move states */
/* Boss7PartD5_MoveState_StartIdle (copied) */
static void Boss7PartD5_MoveState_StartIdle(Boss7PartD5 *p0) {
    Boss7 *t0 = gBoss7;
    t0->unk210 = 1;
    Boss7_SetRivalAnimation(p0, 0x3c);
    Boss7_SetCollidersEnabled(p0, 1, 0);
    p0->h.disp_flags |= 4;
    if (p0->unk480 != NULL) {
        EffTrail_StartFollow(p0->unk480);
        p0->unk480 = NULL;
    }
    if (p0->unk488 != NULL) {
        ((Boss7Fx *)p0->unk488)->h.obj_flags |= 8;
        p0->unk488 = NULL;
    }
    t0->unk20C &= ~0x10u;
    gBossStatus.unk4 = 0x24;
    p0->unk440 = Boss7PartD5_MoveState_Idle;
    p0->unk440(p0);
}

/* Boss7PartD5_MoveState_Idle (copied) */
static void Boss7PartD5_MoveState_Idle(Boss7PartD5 *w) {
    s32 v = w->h.vel_x;
    if (v < 0) {
        if (data_ov000_02368b80[w->unk3D0][0] < -v)
            w->h.vel_x = (s16)(v + data_ov000_02368b80[w->unk3D0][0]);
        else
            w->h.vel_x = 0;
    } else if (v > 0) {
        if (data_ov000_02368b80[w->unk3D0][0] < v)
            w->h.vel_x = (s16)(v - data_ov000_02368b80[w->unk3D0][0]);
        else
            w->h.vel_x = 0;
    }
    if (w->unk3D8.trg & 4) {
        Boss7PartD5_StartSpecialAttack(w);
        return;
    }
    if (Boss7PartD5_CheckJump(w))
        return;
    if (Boss7PartD5_CheckRun(w))
        return;
    if (Boss7PartD5_CheckCrouch(w))
        return;
    if (gBossStatus.unk8 > 1)
        return;
    if (w->unk1F0 == 0)
        w->unk440 = Boss7PartD5_MoveState_WaitDefeatScene;
}

/* Boss7PartD5_MoveState_StartRun (copied) */
static void Boss7PartD5_MoveState_StartRun(Boss7PartD5 *s) {
    s->u.sp.unk464 = 6;
    s->u.sp.unk466 = 1;
    gBossStatus.unk4 = 0x25;
    s->unk440 = Boss7PartD5_MoveState_Run;
}

/* Boss7PartD5_MoveState_Run (copied) */
static void Boss7PartD5_MoveState_Run(Boss7PartD5 *w) {
    u16 f = w->unk3D8.cur;
    if (!(f & 0x30)) {
        s16 d = data_ov000_02368b80[w->unk3D0][0];
        d = (s16)(d + (d >> 1));
        if (w->h.vel_x < 0) {
            if (d < -w->h.vel_x)
                w->h.vel_x = (s16)(w->h.vel_x + d);
            else
                w->h.vel_x = 0;
        } else if (w->h.vel_x > 0) {
            if (d < w->h.vel_x)
                w->h.vel_x = (s16)(w->h.vel_x - d);
            else
                w->h.vel_x = 0;
        }
    } else {
        s16 a = gPlayerPhysicsTable[w->unk3D0][0];
        s32 lim = w->unk430;
        if (f & 0x20) {
            if (w->u.sp.unk464 == 0 && !(w->h.disp_flags & 1)) {
                w->u.sp.unk464 = 6;
                w->u.sp.unk466 = 0;
                w->h.disp_flags |= 1;
            }
            a = (s16)-gPlayerPhysicsTable[w->unk3D0][0];
        } else if (f & 0x10) {
            if (w->u.sp.unk464 == 0 && (w->h.disp_flags & 1)) {
                w->u.sp.unk464 = 6;
                w->u.sp.unk466 = 0;
                w->h.disp_flags &= ~1;
            }
            a = gPlayerPhysicsTable[w->unk3D0][0];
        }
        w->h.vel_x = (s16)(w->h.vel_x + a);
        w->h.vel_x = w->h.vel_x < (s16)-lim ? (s16)-lim : (w->h.vel_x > (s16)lim ? (s16)lim : w->h.vel_x);
        w->h.gnd_vel = w->h.vel_x;
    }
    Boss7PartD5_UpdateRunAnimation(w);
    Boss7PartD5_SetAnimSpeedFromVelocity(w, w->h.gnd_vel);
    if (w->u.sp.unk464 != 0) {
        w->u.sp.unk464--;
        if (((w->h.disp_flags & 1) && (w->unk3D8.trg & 0x10)) || (!(w->h.disp_flags & 1) && (w->unk3D8.trg & 0x20))) {
            if (++w->u.sp.unk466 == 2) {
                w->unk440 = Boss7PartD5_MoveState_StartBackHop;
                return;
            }
        }
    }
    if (w->unk3D8.trg & 4) {
        Boss7PartD5_StartSpecialAttack(w);
        return;
    }
    if (Boss7PartD5_CheckJump(w))
        return;
    if (Boss7PartD5_CheckCrouch(w))
        return;
    if (w->h.vel_x != 0)
        return;
    if (w->u.sp.unk464 == 0)
        w->unk440 = Boss7PartD5_MoveState_StartIdle;
}

/* Boss7PartD5_MoveState_StartJump (copied) */
static void Boss7PartD5_MoveState_StartJump(Boss7PartD5 *p0) {
    Boss7_SetRivalAnimation(p0, 0x31);
    Boss7_SetCollidersEnabled(p0, 1, 0);
    p0->h.move_flags |= 0x10;
    p0->h.vel_y = (s16)-data_ov000_02368ba2[p0->unk3D0][0];
    if (p0->unk3D0 == 0)
        Sound_PlaySeqArc(0, 0, 2);
    else
        Sound_PlaySeqArc(0, 0, 0x13);
    gBossStatus.unk4 = 0x28;
    p0->unk440 = Boss7PartD5_MoveState_Jump;
    s_st.jumps++;
}

/* Boss7PartD5_MoveState_Jump (copied) */
static void Boss7PartD5_MoveState_Jump(Boss7PartD5 *w) {
    u16 f = w->unk3D8.cur;
    if (!(f & 0x30)) {
        s32 v = w->h.vel_x;
        if (v < 0) {
            if (data_ov000_02368b80[w->unk3D0][0] < -v)
                w->h.vel_x = (s16)(v + data_ov000_02368b80[w->unk3D0][0]);
            else
                w->h.vel_x = 0;
        } else if (v > 0) {
            if (data_ov000_02368b80[w->unk3D0][0] < v)
                w->h.vel_x = (s16)(v - data_ov000_02368b80[w->unk3D0][0]);
            else
                w->h.vel_x = 0;
        }
    } else {
        s16 a = gPlayerPhysicsTable[w->unk3D0][0];
        s32 lim = w->unk430;
        if (f & 0x20)
            w->h.disp_flags |= 1;
        else if (f & 0x10)
            w->h.disp_flags &= ~1;
        if (w->h.disp_flags & 1)
            a = (s16)-a;
        w->h.vel_x = (s16)(w->h.vel_x + a);
        w->h.vel_x = w->h.vel_x < (s16)-lim ? (s16)-lim : (w->h.vel_x > (s16)lim ? (s16)lim : w->h.vel_x);
        w->h.gnd_vel = w->h.vel_x;
    }
    if (w->h.move_flags & 1) {
        w->h.vel_y = 0;
        w->h.move_flags &= ~0x10u;
        w->unk440 = Boss7PartD5_MoveState_StartIdle;
        return;
    }
    if (w->unk3D8.trg & 0x100)
        w->unk440 = Boss7PartD5_MoveState_StartAirAttackCharge;
}

/* Boss7PartD5_MoveState_StartCrouch (copied) */
static void Boss7PartD5_MoveState_StartCrouch(Boss7PartD5 *s) {
    s->h.move_flags |= 0x10;
    s->h.vel_x = 0;
    Boss7_SetRivalAnimation(s, 3);
    Boss7_SetCollidersEnabled(s, 1, 0);
    gBossStatus.unk4 = 0x27;
    s->unk440 = Boss7PartD5_MoveState_Crouch;
    s->unk440(s);
    s_st.crouches++;
}

/* Boss7PartD5_MoveState_Crouch (copied) */
static void Boss7PartD5_MoveState_Crouch(Boss7PartD5 *p) {
    if (p->unk3D8.cur & 0x20)
        p->h.disp_flags |= 1;
    else if (p->unk3D8.cur & 0x10)
        p->h.disp_flags &= ~1;
    if (!(p->unk3D8.cur & 0x80)) {
        p->unk440 = Boss7PartD5_MoveState_StartIdle;
        return;
    }
    if (p->unk3D8.trg & 1)
        p->unk440 = Boss7PartD5_MoveState_StartSpindashCharge;
}

/* Boss7PartD5_MoveState_StartSpindashCharge (modified: EffKemuriSpin3D not translated) */
static void Boss7PartD5_MoveState_StartSpindashCharge(Boss7PartD5 *p0) {
    p0->h.move_flags |= 0x10;
    p0->h.vel_x = 0;
    Boss7_SetRivalAnimation(p0, 0x32);
    Boss7_SetCollidersEnabled(p0, 1, 0);
    p0->h.disp_flags |= 4;
    p0->spr0.unk2A = 0x80; /* unkBE */
    p0->spr1.unk2A = 0x80; /* unk106 */
    p0->unk20C.speed = 0x8000;
    p0->unk484 = EffKemuriSpin3D_Create(p0, p0->unk3D0, -10, 0xe, 0);
    if (p0->unk3D0 == 0)
        Sound_PlaySeqArc(0, 0, 0);
    else
        Sound_PlaySeqArc(0, 0, 0x11);
    gBossStatus.unk4 = 0x29;
    p0->unk440 = Boss7PartD5_MoveState_SpindashCharge;
    p0->unk440(p0);
    s_st.spindashes++;
}

/* Boss7PartD5_MoveState_SpindashCharge (copied) */
static void Boss7PartD5_MoveState_SpindashCharge(Boss7PartD5 *p0) {
    if (p0->unk3D8.cur & 0x80)
        return;
    if (p0->unk484 != NULL) /* AnimatorSprite_SetAnimation(unk484 + 0x1c4, ..) */
        AnimatorSprite_SetAnimation(&((Boss7Fx *)p0->unk484)->spr1C4, p0->unk3D0 == 0 ? 1 : 3);
    p0->unk484 = NULL;
    p0->unk440 = Boss7PartD5_MoveState_SpindashRelease;
}

/* Boss7PartD5_MoveState_SpindashRelease (modified: EffTrail / EffBoostA_3D not translated) */
static void Boss7PartD5_MoveState_SpindashRelease(Boss7PartD5 *w) {
    BossEffect *fx;
    w->h.move_flags |= 0x10;
    if (w->h.disp_flags & 1)
        w->h.vel_x = (s16)-data_ov000_02368b86[w->unk3D0][0];
    else
        w->h.vel_x = data_ov000_02368b86[w->unk3D0][0];
    Boss7_SetRivalAnimation(w, 0x31);
    Boss7_SetCollidersEnabled(w, 0, 1);
    w->h.disp_flags |= 4;
    if (w->unk3D0 == 0) {
        w->unk480 = EffTrail_Create(w, 0x18, 0xa, 0x7ca5, 0x7ca5);
        fx = BossEffect_CreateHoming(1, w->h.x, w->h.y, w->h.z + 8);
        fx->unk134.speed = 0x999; /* unk248 */
        fx->unk134.scale[0] = FX_Mul(fx->unk134.scale[0], 0x1800);
        fx->unk134.scale[1] = FX_Mul(fx->unk134.scale[1], 0x1800);
        fx->unk134.scale[2] = FX_Mul(fx->unk134.scale[2], 0x1800);
        w->unk488 = EffBoostA_3D_Create(w, 0, -4, 8, 0);
        Sound_PlaySeqArc(0, 0, 1);
    } else {
        Sound_PlaySeqArc(0, 0, 0x12);
    }
    gBossStatus.unk4 = 0x2a;
    w->unk440 = Boss7PartD5_MoveState_Roll;
    w->unk440(w);
}

/* Boss7PartD5_MoveState_Roll (copied) */
static void Boss7PartD5_MoveState_Roll(Boss7PartD5 *p0) {
    Boss7 *t0 = gBoss7;
    Boss7PartD5_SetAnimSpeedFromVelocity(p0, p0->h.gnd_vel);
    if (p0->unk3D0 != 0) {
        if ((t0->unk220 & 1) == 0)
            EffFire01_3D_CreateFollowing(p0, 0, 0, 0);
    }
    if (p0->h.vel_x != 0) {
        if ((p0->h.move_flags & 4) == 0) {
            if ((p0->unk3D8.trg & 0x40) == 0)
                return;
        }
    }
    if (p0->unk480 != NULL) {
        EffTrail_StartFollow(p0->unk480);
        p0->unk480 = NULL;
    }
    if (p0->unk488 != NULL) {
        ((Boss7Fx *)p0->unk488)->h.obj_flags |= 8;
        p0->unk488 = NULL;
    }
    p0->h.vel_x = (s16)(p0->h.vel_x >> 1);
    p0->unk440 = Boss7PartD5_MoveState_StartIdle;
}

/* Boss7PartD5_MoveState_StartBackHop (copied) */
static void Boss7PartD5_MoveState_StartBackHop(Boss7PartD5 *p) {
    Boss7_SetRivalAnimation(p, 0x15);
    Boss7_SetCollidersEnabled(p, 0, 0);
    if (p->h.disp_flags & 1)
        p->h.vel_x = 0xa00;
    else
        p->h.vel_x = -0xa00;
    p->h.vel_y = -0x140;
    p->h.move_flags |= 0x10;
    gBossStatus.unk4 = 0x26;
    p->unk440 = Boss7PartD5_MoveState_BackHop;
    s_st.backhops++;
}

/* Boss7PartD5_MoveState_BackHop (copied) */
static void Boss7PartD5_MoveState_BackHop(Boss7PartD5 *s) {
    s->h.vel_x = (s16)((s->h.vel_x * 0xe0) >> 8);
    if (s->h.move_flags & 1) {
        s->h.move_flags &= ~0x10u;
        s->unk440 = Boss7PartD5_MoveState_StartIdle;
    }
}

/* Boss7PartD5_MoveState_StartAirAttackCharge (copied) */
static void Boss7PartD5_MoveState_StartAirAttackCharge(Boss7PartD5 *w) {
    Boss7 *b = gBoss7;
    if (w->h.move_flags & 1) {
        w->unk440 = Boss7PartD5_MoveState_StartIdle;
        return;
    }
    w->h.vel_x = 0;
    w->h.vel_y = -0x20;
    w->h.move_flags &= ~0x80u;
    Boss7_SetRivalAnimation(w, 0x32);
    Boss7_SetCollidersEnabled(w, 0, 0);
    w->h.disp_flags |= 4;
    w->spr0.unk2A = 0;
    w->spr1.unk2A = 0;
    w->unk20C.speed = 0;
    if (w->unk3D0 == 0)
        b->unk20C |= 0x10;
    if (w->h.x > gPlayers->x)
        w->h.disp_flags |= 1;
    else
        w->h.disp_flags &= ~1;
    gBossStatus.unk4 = 0x2b;
    w->unk440 = Boss7PartD5_MoveState_AirAttackCharge;
    w->unk440(w);
    s_st.air_attacks++;
}

/* Boss7PartD5_MoveState_AirAttackCharge (copied) */
static void Boss7PartD5_MoveState_AirAttackCharge(Boss7PartD5 *w) {
    Boss7 *b = gBoss7;
    w->spr0.unk2A += 4;
    w->spr1.unk2A += 4;
    w->unk20C.speed += 0x400;
    if (w->unk3D0 == 0) {
        if ((b->unk220 & 0x1f) == 0)
            BossEffect_CreateHoming(1, w->h.x, w->h.y, w->h.z + 8);
    } else {
        if ((b->unk220 & 7) == 0)
            Boss7PartD5_SpawnFireParticle(w);
    }
    if (w->spr0.unk2A < 0x80)
        return;
    if (w->unk3D8.cur & 0x100)
        return;
    w->spr0.unk2A = 0x80;
    w->spr1.unk2A = 0x80;
    w->unk20C.speed = 0x8000;
    w->unk440 = Boss7PartD5_MoveState_StartAirDash;
    w->unk440(w);
}

/* Boss7PartD5_MoveState_StartAirDash (modified: BLDY -> boss7_blend, EffBoostA2_3D not translated) */
static void Boss7PartD5_MoveState_StartAirDash(Boss7PartD5 *w) {
    s32 a;
    BossEffect *fx;
    Boss7Fx *o;
    Boss7_SetRivalAnimation(w, 0x31);
    Boss7_SetCollidersEnabled(w, 0, 1);
    a = dcp_atan2(0x1f000 - w->h.y, gPlayers->x - w->h.x); /* FX_Atan2 */
    w->h.trick_input = a; /* unk8 */
    if (w->h.x > gPlayers->x)
        w->h.disp_flags |= 1;
    else
        w->h.disp_flags &= ~1;
    if (w->unk3D0 == 0) {
        fx = BossEffect_CreateHoming(1, w->h.x, w->h.y, w->h.z + 8);
        fx->unk134.speed = 0x999;
        fx->unk134.scale[0] = FX_Mul(fx->unk134.scale[0], 0x1800);
        fx->unk134.scale[1] = FX_Mul(fx->unk134.scale[1], 0x1800);
        fx->unk134.scale[2] = FX_Mul(fx->unk134.scale[2], 0x1800);
        Sound_PlaySeqArc(0, 0, 7);
        o = EffBoostA2_3D_Create(w, 0, 0, 0, 0);
    } else {
        Sound_PlaySeqArc(0, 0, 0xbd);
        o = EffBoostA2_3D_Create(w, 1, 0, 0, 0);
    }
    if (o != NULL) {
        if (o->h.disp_flags & 1)
            o->h.angle = (u8)((a + 0x8000) >> 8);
        else
            o->h.angle = (u8)(a >> 8);
    }
    Boss7_SetBlend(0xb);
    ScreenShake_Start(4);
    w->unk440 = Boss7PartD5_MoveState_AirDash;
    w->unk440(w);
}

/* Boss7PartD5_MoveState_AirDash (modified: BLDY -> boss7_blend) */
static void Boss7PartD5_MoveState_AirDash(Boss7PartD5 *w) {
    Boss7 *b = gBoss7;
    BossEffect *fx;
    if (s_blend_evy != 0) {
        s_blend_evy = (s_blend_evy - 1) & 0x1f; /* data_02089250.b0 -= 1 */
        CAM.unk3C.x = ScreenShake_GetOffsetX() << 13;
        CAM.unk3C.y = ScreenShake_GetOffsetY() << 13;
    }
    if (w->unk3D0 == 0) {
        if (!(b->unk220 & 7)) {
            fx = BossEffect_CreateHoming(1, w->h.x, w->h.y, w->h.z + 8);
            fx->unk134.speed = 0x2000;
            fx->unk134.scale[0] = FX_Mul(fx->unk134.scale[0], 0xc00);
            fx->unk134.scale[1] = FX_Mul(fx->unk134.scale[1], 0xc00);
            fx->unk134.scale[2] = FX_Mul(fx->unk134.scale[2], 0xc00);
        }
    } else if (!(b->unk220 & 1)) {
        Boss7PartD5_SpawnFireParticle(w);
    }
    w->h.vel_x = (s16)(FX_Mul(COS((u16)w->h.trick_input), data_ov000_02363b74[w->unk42C]) >> 4);
    w->h.vel_y = (s16)(FX_Mul(SIN((u16)w->h.trick_input), data_ov000_02363b74[w->unk42C]) >> 4);
    if (!(w->h.move_flags & 1))
        return;
    if (s_blend_evy != 0)
        return;
    w->h.vel_y = 0;
    w->h.move_flags |= 0x80;
    if (w->unk480 != NULL) {
        EffTrail_StartFollow(w->unk480);
        w->unk480 = NULL;
    }
    b->unk20C &= ~0x10u;
    w->unk440 = Boss7PartD5_MoveState_StartDashSkid;
}

/* Boss7PartD5_MoveState_StartDashSkid (copied) */
static void Boss7PartD5_MoveState_StartDashSkid(Boss7PartD5 *s) {
    Boss7_SetCollidersEnabled(s, 1, 0);
    if (s->h.x > gPlayers->x)
        s->h.disp_flags |= 1;
    else
        s->h.disp_flags &= ~1;
    s->unk440 = Boss7PartD5_MoveState_DashSkid;
    s->unk440(s);
}

/* Boss7PartD5_MoveState_DashSkid (copied) */
static void Boss7PartD5_MoveState_DashSkid(Boss7PartD5 *w) {
    Boss7 *b = gBoss7;
    if (w->unk3D0 == 0) {
        if ((b->unk220 & 0x1f) == 0)
            BossEffect_CreateHoming(1, w->h.x, w->h.y, w->h.z + 8);
    } else {
        if ((b->unk220 & 7) == 0)
            Boss7PartD5_SpawnFireParticle(w);
    }
    Boss7PartD5_MoveState_Idle(w);
    if (w->h.vel_x == 0)
        w->unk440 = Boss7PartD5_MoveState_StartIdle;
}

/* Boss7PartD5_MoveState_StartHurt (copied) */
static void Boss7PartD5_MoveState_StartHurt(Boss7PartD5 *w) {
    Boss7 *b = gBoss7;
    Boss7_SetRivalAnimation(w, 0xf);
    Sound_PlaySeqArc(0, 0, 0x7b);
    w->unk1F0 = 0xb4;
    w->h.move_flags |= 0x90;
    w->h.vel_y = -0x300;
    if (w->h.disp_flags & 1)
        w->h.vel_x = 0x180;
    else
        w->h.vel_x = -0x180;
    if (w->unk480 != NULL) {
        EffTrail_StartFollow(w->unk480);
        w->unk480 = NULL;
    }
    if (w->unk488 != NULL) {
        ((Boss7Fx *)w->unk488)->h.obj_flags |= 8;
        w->unk488 = NULL;
    }
    b->unk20C &= ~0x10u;
    gBossStatus.unk4 = 0x2d;
    w->unk440 = Boss7PartD5_MoveState_Hurt;
    s_st.hurts++;
}

/* Boss7PartD5_MoveState_Hurt (copied) */
static void Boss7PartD5_MoveState_Hurt(Boss7PartD5 *s) {
    if (s->h.move_flags & 1) {
        s->h.move_flags &= ~0x10u;
        s->unk440 = Boss7PartD5_MoveState_StartIdle;
    }
}

/* Boss7PartD5_MoveState_WaitDefeatScene (modified: NNS_SndPlayerMoveVolume(BGM, 0x20, 0xb4) logged) */
static void Boss7PartD5_MoveState_WaitDefeatScene(Boss7PartD5 *w) {
    if (!(w->h.move_flags & 1))
        return;
    if (!(gPlayers->move_flags & 1))
        return;
    if ((s16)w->unk1F0 > 0)
        return;
    Boss7_SetRivalAnimation(w, 0x3c);
    Boss7_SetCollidersEnabled(w, 0, 0);
    w->h.disp_flags |= 4;
    gPlayers->disp_flags &= ~1;
    w->h.disp_flags |= 1;
    w->u.df.unk464 = gPlayers->x << 4;
    w->u.df.unk468 = gPlayers->y << 4;
    w->u.df.unk470 = w->h.x << 4;
    w->u.df.unk474 = w->h.y << 4;
    w->unk47C = 0;
    w->u.df.unk46C = (gPlayers->x >> 8) - 0x40;
    w->u.df.unk46C = w->u.df.unk46C < 0 ? -w->u.df.unk46C : w->u.df.unk46C;
    w->u.df.unk46C >>= 1;
    w->u.df.unk46C = w->u.df.unk46C < 0x10 ? 0x10 : (w->u.df.unk46C > 0x28 ? 0x28 : w->u.df.unk46C);
    w->u.df.unk478 = (w->h.x >> 8) - 0x100;
    w->u.df.unk478 = w->u.df.unk478 < 0 ? -w->u.df.unk478 : w->u.df.unk478;
    w->u.df.unk478 >>= 1;
    w->u.df.unk478 = w->u.df.unk478 < 0x10 ? 0x10 : (w->u.df.unk478 > 0x28 ? 0x28 : w->u.df.unk478);
    boss_log("bgm_volume", 0x20, 0xb4);
    w->unk440 = Boss7PartD5_MoveState_DefeatScene;
}

/* Boss7PartD5_MoveState_DefeatScene (copied) */
static void Boss7PartD5_MoveState_DefeatScene(Boss7PartD5 *s) {
    u16 ang;
    s32 st;
    s32 sn;
    s->unk47C += 0x7a;
    if (s->unk47C >= 0x1000) {
        s->unk47C = 0x1000;
        s->unk440 = Boss7PartD5_MoveState_DefeatSceneDone;
    }
    ang = (u16)(FX_Mul(s->unk47C, 0x8000000) >> 12);
    if (s->unk47C < 0x1000) {
        st = s->unk3D4;
        if (st < 0x13 || st > 0x15)
            Boss7_SetRivalAnimation(s, 0x14);
        else if (st == 0x13 && ang < 0x4000)
            Boss7_SetRivalAnimation(s, 0x14);
        else if (st == 0x14 && (s->h.disp_flags & 8))
            Boss7_SetRivalAnimation(s, 0x15);
    } else if (s->unk3D4 != 0x3c) {
        Boss7_SetRivalAnimation(s, 0x3c);
    }
    Boss7_SetCollidersEnabled(s, 0, 0);
    sn = SIN(ang);
    gPlayers->y = (s->u.df.unk468 - s->u.df.unk46C * sn) >> 4;
    s->h.y = (s->u.df.unk474 - s->u.df.unk478 * sn) >> 4;
    gPlayers->x = (s->u.df.unk464 + FX_Mul(0x40000 - s->u.df.unk464, s->unk47C)) >> 4;
    s->h.x = (s->u.df.unk470 + FX_Mul(0x100000 - s->u.df.unk470, s->unk47C)) >> 4;
}

/* Boss7PartD5_MoveState_DefeatSceneDone (copied) */
static void Boss7PartD5_MoveState_DefeatSceneDone(Boss7PartD5 *w) {
    (void)w;
    gBoss7->unk20C |= 2;
}

/* Boss7PartD5_MoveState_StartDefeatExit (copied) */
static void Boss7PartD5_MoveState_StartDefeatExit(Boss7PartD5 *p) {
    gBossStatus.unk4 = 0x2d;
    Boss7_SetRivalAnimation(p, 0xf);
    p->h.move_flags |= 0x190;
    p->h.move_flags &= ~1u;
    p->h.vel_x = 0x300;
    p->h.vel_y = -0x600;
    p->unk440 = Boss7PartD5_MoveState_DefeatExit;
}

/* Boss7PartD5_MoveState_DefeatExit (modified: data_022c4590.unk10 -> g_obj_stage.ring_bonus,
 * StageShutdown_Create(Stage_OnBossCleared) -> g_boss_hooks.cleared once) */
static void Boss7PartD5_MoveState_DefeatExit(Boss7PartD5 *p) {
    if (p->h.y <= 0x6b000)
        return;
    g_obj_stage.ring_bonus = gPlayers->rings * 1000; /* unk3AE */
    g_boss_game.cleared = 1;
    if (!s_cleared_called) {
        s_cleared_called = 1;
        if (g_boss_hooks.cleared)
            g_boss_hooks.cleared(g_boss_hooks.ctx);
        boss_log("cleared", g_obj_stage.ring_bonus, 0);
    }
}

/* ---- Sonic's special: tornado (rival = Sonic) */
/* Boss7PartD5_MoveState_StartSonicSpecial (copied) */
static void Boss7PartD5_MoveState_StartSonicSpecial(Boss7PartD5 *w) {
    Boss7 *b = gBoss7;
    Boss7_SetRivalAnimation(w, 0x31);
    Boss7_SetCollidersEnabled(w, 0, 0);
    if (w->h.x < 0x4000) {
        w->h.vel_x = 0x180;
        w->h.disp_flags &= ~1;
    } else if (w->h.x > 0x10000) {
        w->h.vel_x = -0x180;
        w->h.disp_flags |= 1;
    }
    w->h.move_flags |= 0x10;
    w->h.vel_y = (s16)-data_ov000_02368ba2[w->unk3D0][0];
    b->unk210 = 2;
    w->unk440 = Boss7PartD5_MoveState_SonicSpecialRise;
}

/* Boss7PartD5_MoveState_SonicSpecialRise (copied) */
static void Boss7PartD5_MoveState_SonicSpecialRise(Boss7PartD5 *s) {
    if (s->h.vel_y > -0x100) {
        s->h.vel_x = 0;
        s->unk440 = Boss7PartD5_MoveState_SonicSpecialStartOrbit;
    }
}

/* Boss7PartD5_MoveState_SonicSpecialStartOrbit (copied) */
static void Boss7PartD5_MoveState_SonicSpecialStartOrbit(Boss7PartD5 *w) {
    Boss7 *b = gBoss7;
    w->h.vel_x = 0;
    w->u.sp.unk466 = 0;
    w->u.sp.unk468 = 0;
    w->u.sp.unk46C = 0;
    w->u.sp.unk464 = data_ov000_02363b3c[w->unk42E];
    Boss7_SetCollidersEnabled(w, 0, 1);
    b->unk20C |= 0x20;
    b->unk224 = w->h.x;
    b->unk228 = data_ov000_02363c24[w->unk42E][w->u.sp.unk464 - 1];
    b->unk22A = 0;
    b->unk22C = (s16)(data_ov000_02363c64[w->unk42E][w->u.sp.unk464 - 1] >> 1);
    w->unk440 = Boss7PartD5_MoveState_SonicSpecialSpinUp;
}

/* Boss7PartD5_MoveState_SonicSpecialSpinUp (modified: MapObject_Spawn(0xd6) -> pool) */
static void Boss7PartD5_MoveState_SonicSpecialSpinUp(Boss7PartD5 *w) {
    Boss7 *b;
    Boss7PartD6 *o;
    if (w->h.vel_y >= 0) {
        w->h.vel_y = 0;
        w->h.move_flags &= ~0x80u;
    }
    w->u.sp.unk468 += 0x40;
    w->u.sp.unk46C += 0x4cc;
    Boss7PartD5_UpdateCircleMotion(w);
    if (w->u.sp.unk46C > 0x30000)
        w->u.sp.unk46C = 0x30000;
    if (w->u.sp.unk468 > 0x1555)
        w->u.sp.unk468 = 0x1555;
    if (w->u.sp.unk46C != 0x30000)
        return;
    if (w->u.sp.unk468 != 0x1555)
        return;
    b = gBoss7;
    o = Boss7PartD6_Create(NULL, b->unk224, 0x20000);
    if (o != NULL) {
        o->h.timer = data_ov000_02363c04[w->unk42E][w->u.sp.unk464 - 1]; /* unkC */
        w->u.sp.unk470 = o->h.timer;
    } else
        w->u.sp.unk470 = data_ov000_02363c04[w->unk42E][w->u.sp.unk464 - 1];
    b->unk22C = data_ov000_02363c64[w->unk42E][w->u.sp.unk464 - 1];
    w->unk440 = Boss7PartD5_MoveState_SonicSpecialOrbit;
}

/* Boss7PartD5_MoveState_SonicSpecialOrbit (copied) */
static void Boss7PartD5_MoveState_SonicSpecialOrbit(Boss7PartD5 *w) {
    u16 t;
    Boss7PartD5_UpdateCircleMotion(w);
    if (w->u.sp.unk470 > 0)
        w->u.sp.unk470--;
    if (w->u.sp.unk470 != 0)
        return;
    w->u.sp.unk464--;
    if (w->u.sp.unk464 != 0) {
        w->unk440 = Boss7PartD5_MoveState_SonicSpecialWaitDashAngle;
        return;
    }
    t = data_ov000_02363b84[GetIdxB()][w->unk42E];
    if (t >= (Rand() & 0xf))
        w->unk440 = Boss7PartD5_MoveState_SonicSpecialStartCharge;
    else
        w->unk440 = Boss7PartD5_MoveState_SonicSpecialSpinDown;
}

/* Boss7PartD5_MoveState_SonicSpecialWaitDashAngle (copied) */
static void Boss7PartD5_MoveState_SonicSpecialWaitDashAngle(Boss7PartD5 *w) {
    Boss7 *b = gBoss7;
    Boss7PartD5_UpdateCircleMotion(w);
    if (b->unk224 < gPlayers->x) {
        if (w->u.sp.unk466 < 0xaaab)
            return;
        if (w->u.sp.unk466 <= 0xd555) {
            w->h.vel_x = 0xf00;
            w->unk440 = Boss7PartD5_MoveState_SonicSpecialDashToPlayer;
        }
    } else {
        if (w->u.sp.unk466 < 0x2aab)
            return;
        if (w->u.sp.unk466 <= 0x5555) {
            w->h.vel_x = -0xf00;
            w->unk440 = Boss7PartD5_MoveState_SonicSpecialDashToPlayer;
        }
    }
}

/* Boss7PartD5_MoveState_SonicSpecialDashToPlayer (copied) */
static void Boss7PartD5_MoveState_SonicSpecialDashToPlayer(Boss7PartD5 *w) {
    Boss7 *b = gBoss7;
    s32 d;
    b->unk22C = -8;
    if (w->h.vel_x > 0)
        d = gPlayers->x - w->h.x;
    else
        d = w->h.x - gPlayers->x;
    if (d > 0x800)
        return;
    w->h.vel_x = 0;
    w->u.sp.unk468 >>= 1;
    b->unk224 = w->h.x;
    b->unk228 = data_ov000_02363c24[w->unk42E][w->u.sp.unk464 - 1];
    b->unk22C = (s16)(data_ov000_02363c64[w->unk42E][w->u.sp.unk464 - 1] >> 1);
    b->unk22A >>= 2;
    w->unk440 = Boss7PartD5_MoveState_SonicSpecialSpinUp;
}

/* Boss7PartD5_MoveState_SonicSpecialStartCharge (copied) */
static void Boss7PartD5_MoveState_SonicSpecialStartCharge(Boss7PartD5 *p) {
    gBoss7->unk22C = -8;
    Boss7_SetRivalAnimation(p, 0x32);
    p->h.disp_flags |= 4;
    p->spr0.unk2A = 0;
    p->spr1.unk2A = 0;
    p->unk20C.speed = 0;
    p->unk440 = Boss7PartD5_MoveState_SonicSpecialCharge;
}

/* Boss7PartD5_MoveState_SonicSpecialCharge (copied) */
static void Boss7PartD5_MoveState_SonicSpecialCharge(Boss7PartD5 *w) {
    Boss7 *b = gBoss7;
    BossEffect *fx;
    w->spr0.unk2A += 4;
    w->spr1.unk2A += 4;
    w->unk20C.speed += 0x400;
    if (w->spr0.unk2A >= 0x80) {
        w->spr0.unk2A = 0x80;
        w->spr1.unk2A = 0x80;
        w->unk20C.speed = 0x8000;
    }
    w->u.sp.unk468 -= 0x20;
    if (w->u.sp.unk468 < 0)
        w->u.sp.unk468 = 0;
    w->u.sp.unk46C -= 0x666;
    if (w->u.sp.unk46C < 0)
        w->u.sp.unk46C = 0;
    Boss7PartD5_UpdateCircleMotion(w);
    if (!(b->unk220 & 0xf))
        BossEffect_CreateHoming(1, w->h.x, w->h.y, w->h.z + 8);
    if (w->spr0.unk2A != 0x80)
        return;
    if (w->u.sp.unk46C != 0)
        return;
    b = gBoss7;
    w->spr0.unk2A = 0x80;
    w->spr1.unk2A = 0x80;
    w->unk20C.speed = 0x8000;
    b->unk20C &= ~0x20u;
    b->unk20C |= 0x10;
    fx = BossEffect_CreateHoming(1, w->h.x, w->h.y, w->h.z + 8);
    fx->unk134.speed = 0x999;
    fx->unk134.scale[0] = FX_Mul(fx->unk134.scale[0], 0x1800);
    fx->unk134.scale[1] = FX_Mul(fx->unk134.scale[1], 0x1800);
    fx->unk134.scale[2] = FX_Mul(fx->unk134.scale[2], 0x1800);
    w->unk440 = Boss7PartD5_MoveState_StartAirDash;
}

/* Boss7PartD5_MoveState_SonicSpecialSpinDown (copied) */
static void Boss7PartD5_MoveState_SonicSpecialSpinDown(Boss7PartD5 *p) {
    gBoss7->unk22C = -8;
    p->u.sp.unk468 -= 0x20;
    if (p->u.sp.unk468 < 0)
        p->u.sp.unk468 = 0;
    p->u.sp.unk46C -= 0x666;
    Boss7PartD5_UpdateCircleMotion(p);
    if (p->u.sp.unk46C < 0)
        p->unk440 = Boss7PartD5_MoveState_SonicSpecialStartFall;
}

/* Boss7PartD5_MoveState_SonicSpecialStartFall (copied) */
static void Boss7PartD5_MoveState_SonicSpecialStartFall(Boss7PartD5 *p0) {
    gBoss7->unk20C &= ~0x20u;
    p0->h.vel_x = 0;
    p0->h.vel_y = (s16)(-data_ov000_02368ba2[p0->unk3D0][0] / 2);
    p0->h.move_flags |= 0x80;
    Boss7_SetCollidersEnabled(p0, 0, 0);
    p0->unk440 = Boss7PartD5_MoveState_SonicSpecialFall;
}

/* Boss7PartD5_MoveState_SonicSpecialFall (copied) */
static void Boss7PartD5_MoveState_SonicSpecialFall(Boss7PartD5 *s) {
    if (!(s->h.move_flags & 1))
        return;
    s->h.vel_y = 0;
    s->h.move_flags &= ~0x10u;
    s->unk440 = Boss7PartD5_MoveState_StartIdle;
}

/* ---- Blaze's special: fire pillars (rival = Blaze) */
/* Boss7PartD5_MoveState_StartBlazeSpecial (copied) */
static void Boss7PartD5_MoveState_StartBlazeSpecial(Boss7PartD5 *w) {
    Boss7 *b = gBoss7;
    Boss7_SetRivalAnimation(w, 0x13);
    Boss7_SetCollidersEnabled(w, 0, 0);
    w->h.move_flags |= 0x10;
    w->h.vel_y = (s16)-(gPlayerPhysicsTable[1][0x13] + 0x100);
    if (w->h.x < 0x4000) {
        w->h.vel_x = 0x180;
        w->h.disp_flags &= ~1;
    } else if (w->h.x > 0x10000) {
        w->h.vel_x = -0x180;
        w->h.disp_flags |= 1;
    }
    b->unk210 = 2;
    w->u.sp.unk464 = data_ov000_02363b24[w->unk42E];
    w->u.sp.unk466 = 0;
    w->unk440 = Boss7PartD5_MoveState_BlazeSpecialRise;
    w->unk440(w);
}

/* Boss7PartD5_MoveState_BlazeSpecialRise (copied) */
static void Boss7PartD5_MoveState_BlazeSpecialRise(Boss7PartD5 *p0) {
    if (p0->h.vel_y < 0)
        return;
    p0->h.vel_x = 0;
    p0->h.vel_y = 0;
    p0->h.move_flags &= ~0x80u;
    Boss7_SetRivalAnimation(p0, 0x4a);
    p0->h.disp_flags |= 4;
    Boss7_SetCollidersEnabled(p0, 0, 0);
    p0->unk440 = Boss7PartD5_MoveState_BlazeSpecialStartHover;
}

/* Boss7PartD5_MoveState_BlazeSpecialStartHover (copied) */
static void Boss7PartD5_MoveState_BlazeSpecialStartHover(Boss7PartD5 *s) {
    s->u.sp.unk46A = 0;
    s->unk440 = Boss7PartD5_MoveState_BlazeSpecialHoverDelay;
}

/* Boss7PartD5_MoveState_BlazeSpecialHoverDelay (copied) */
static void Boss7PartD5_MoveState_BlazeSpecialHoverDelay(Boss7PartD5 *p0) {
    u16 t0;
    Boss7PartD5_HoverBob(p0);
    t0 = p0->u.sp.unk46A;
    p0->u.sp.unk46A = (u16)(p0->u.sp.unk46A + 1);
    if (t0 <= 0x1e)
        return;
    p0->unk440 = Boss7PartD5_MoveState_BlazeSpecialSpawnPillar;
    p0->unk440(p0);
}

/* Boss7PartD5_MoveState_BlazeSpecialSpawnPillar (modified: MapObject_Spawn(0xd6) -> pool) */
static void Boss7PartD5_MoveState_BlazeSpecialSpawnPillar(Boss7PartD5 *w) {
    Boss7PartD6 *o;
    w->u.sp.unk468 = (s16)data_ov000_02363b44[w->unk42E];
    if (w->h.x < gPlayers->x)
        w->h.disp_flags &= ~1;
    else
        w->h.disp_flags |= 1;
    o = Boss7PartD6_Create(NULL, gPlayers->x, 0x20000);
    if (o != NULL)
        o->h.timer = data_ov000_02363b6c[w->unk42E]; /* unkC */
    w->unk440 = Boss7PartD5_MoveState_BlazeSpecialHover;
    w->unk440(w);
}

/* Boss7PartD5_MoveState_BlazeSpecialHover (copied) */
static void Boss7PartD5_MoveState_BlazeSpecialHover(Boss7PartD5 *p) {
    Boss7PartD5_HoverBob(p);
    if (p->u.sp.unk468-- != 0)
        return;
    p->u.sp.unk464--;
    if (p->u.sp.unk464 != 0)
        p->unk440 = Boss7PartD5_MoveState_BlazeSpecialSpawnPillar;
    else
        p->unk440 = Boss7PartD5_MoveState_BlazeSpecialEnd;
}

/* Boss7PartD5_MoveState_BlazeSpecialEnd (copied) */
static void Boss7PartD5_MoveState_BlazeSpecialEnd(Boss7PartD5 *w) {
    u16 t;
    t = data_ov000_02363b94[GetIdxB()][w->unk42E];
    if (t >= (Rand() & 0xf)) {
        w->unk42C = w->unk42E;
        Boss7PartD5_MoveState_StartAirAttackCharge(w);
        return;
    }
    Boss7_SetRivalAnimation(w, 0x13);
    w->h.move_flags |= 0x10;
    w->h.move_flags |= 0x80;
    w->h.vel_y = -0x280;
    w->unk440 = Boss7PartD5_MoveState_BlazeSpecialFall;
}

/* Boss7PartD5_MoveState_BlazeSpecialFall (copied) */
static void Boss7PartD5_MoveState_BlazeSpecialFall(Boss7PartD5 *p0) {
    Boss7 *t0;
    if ((p0->h.move_flags & 1) == 0)
        return;
    t0 = gBoss7;
    t0->unk210 = 1;
    Boss7_SetRivalAnimation(p0, 0x31);
    p0->h.move_flags &= ~0x10u;
    p0->h.vel_x = 0;
    p0->h.vel_y = 0;
    p0->unk440 = Boss7PartD5_MoveState_StartIdle;
}

/* ============================================================ Boss7PartD6: tornado / fire pillar */
/* Boss7PartD6_Draw (modified: Model_Draw -> pose) */
static void Boss7PartD6_Draw(void *p) {
    Boss7PartD6 *t1 = (Boss7PartD6 *)p;
    BossModel *t2 = &t1->unk20C;
    t2->posed = 0;
    if (t1->h.disp_flags & 0x20)
        return;
    t1->h.disp_flags &= ~8;
    if (t1->h.disp_flags & 4) {
        t2->flags[1] |= 4; /* +0x10D */
        t2->flags[2] |= 4; /* +0x10E */
    } else {
        t2->flags[1] &= ~4;
        t2->flags[2] &= ~4;
    }
    t2->trans[0] = t1->h.x << 4;
    t2->trans[1] = -(t1->h.y << 4);
    t2->trans[2] = t1->h.z << 4;
    Model_Animate(t2);
    boss_model_pose(t2, NULL);
    if (t2->flags[1] & 2)
        t1->h.disp_flags |= 8;
    if (t2->flags[2] & 2)
        t1->h.disp_flags |= 8;
}

static void Boss7PartD6_Collide(void *p) {
    Boss7PartD6 *s = (Boss7PartD6 *)p;
    MapObject_Collide_Default(&s->h, s, &s->coll0);
}

/* Boss7PartD6_Destructor (modified) */
static void Boss7PartD6_Destructor(void *p) {
    Boss7PartD6 *s = (Boss7PartD6 *)p;
    boss_sfx_stop(s->unk384); /* Sound_FreeHandle */
    boss_model_release(&s->unk20C);
    rect_forget(&s->coll0);
    rect_forget(&s->coll1);
    rect_forget(&s->coll2);
}

/* Boss7PartD6_Create (modified: pool slot, archive files -> converted packages / timing-only animations) */
static Boss7PartD6 *Boss7PartD6_Create(const ObjParam *p, s32 b, s32 c) {
    Boss7 *mgr = gBoss7;
    Boss7PartD6 *w = NULL;
    Boss7PartD5 *par = mgr->unk214;
    ObjBox *coll;
    BossModel *sub;
    int i;
    for (i = 0; i < B7_D6; i++)
        if (!s_d6[i].alive) {
            w = &s_d6[i];
            break;
        }
    if (w == NULL) {
        boss_log("d6_pool_full", 0, 0);
        return NULL;
    }
    boss_model_release(&w->unk20C);
    memset(w, 0, sizeof(*w));
    w->alive = 1;
    w->seq = ++s_seq;
    w->func = Boss7PartD6_State_Appear;
    w->unk378 = par->unk3D0;
    w->unk37C = par->unk42E;
    w->unk384 = 2; /* Sound_AllocHandle */
    /* "bs7": 0x12..0x18 (tornado) / 0x19..0x1f (flame): model, 01 ma, 02 ma, 03 ma, 01 tp, 02 tp, 03 tp */
    w->unk388 = &s_d6_mdl;
    w->unk38C = &s_d6_anm[0];
    w->unk390 = &s_d6_anm[1];
    w->unk394 = &s_d6_anm[2];
    w->unk398 = &s_d6_anm[3];
    w->unk39C = &s_d6_anm[4];
    w->unk3A0 = &s_d6_anm[5];
    MapObject_Init(&w->h, &w->coll0, p, b, c);
    w->h.obj_flags |= 0x10;
    w->h.move_flags |= 0x3300;
    w->h.x = b;
    w->h.y = c;
    w->h.z = mgr->h.z;
    if (w->unk378 == 0) {
        coll = &w->coll0;
        coll->flags &= ~4;
        coll = &w->coll1;
        rect_set_box2d(coll, -12, -256, 12, 0);
        coll->owner = w;
        coll->owner_kind = OBJ_OWNER_OBJECT;
        coll->flags &= ~4;
    } else {
        coll = &w->coll0;
        coll->flags &= ~4;
        coll = &w->coll1;
        rect_set_box2d(coll, -8, -256, 8, 0);
        coll->owner = w;
        coll->owner_kind = OBJ_OWNER_OBJECT;
        coll->flags &= ~4;
    }
    sub = &w->unk20C;
    Model_Init(sub, 0);
    Model_SetResource(sub, w->unk388);
    sub->trans[0] = w->h.x << 4;
    sub->trans[1] = -(w->h.y << 4);
    sub->trans[2] = w->h.z << 4;
    sub->scale[0] = sub->scale[1] = sub->scale[2] = 0x34cc;
    s_st.pillars++;
    boss_log("pillar", b >> 8, w->unk378);
    return w;
}

/* Boss7PartD6_State_Appear (modified: NNS_G3dGetTex for the pattern animation dropped) */
static void Boss7PartD6_State_Appear(void *p) {
    Boss7PartD6 *w = (Boss7PartD6 *)p;
    BossModel *m = &w->unk20C;
    Model_SetAnimation(m, 1, w->unk38C, 0);
    Model_SetAnimation(m, 2, w->unk398, 0);
    m->speed = 0x1000;
    if (w->unk378 == 0) {
        w->unk380 = BossEffect_CreateSuction(1, w->h.x, w->h.y, w->h.z);
        w->unk380->h.disp_flags |= 4;
        w->unk380->unkC = -1;
        boss_sfx_handle(w->unk384, 0x98);
    } else {
        m->speed = data_ov000_02363b64[w->unk37C];
        boss_sfx_handle(w->unk384, 0x99);
    }
    w->func = Boss7PartD6_State_WaitAnimEnd_ov000_022ec34c;
}

/* Boss7PartD6_State_WaitAnimEnd_ov000_022ec34c (copied) */
static void Boss7PartD6_State_WaitAnimEnd_ov000_022ec34c(void *p) {
    Boss7PartD6 *s = (Boss7PartD6 *)p;
    if (s->h.disp_flags & 8)
        s->func = Boss7PartD6_State_Activate;
}

/* Boss7PartD6_State_Activate (copied) */
static void Boss7PartD6_State_Activate(void *p) {
    Boss7PartD6 *w = (Boss7PartD6 *)p;
    BossModel *m = &w->unk20C;
    Model_SetAnimation(m, 1, w->unk390, 0);
    Model_SetAnimation(m, 2, w->unk39C, 0);
    m->speed = 0x1000;
    w->coll1.flags |= 4;
    w->h.disp_flags |= 4;
    w->func = Boss7PartD6_State_Active;
}

/* Boss7PartD6_State_Active (copied) */
static void Boss7PartD6_State_Active(void *p) {
    Boss7PartD6 *s = (Boss7PartD6 *)p;
    if (s->h.timer > 0) /* unkC */
        s->h.timer--;
    if (!(s->h.disp_flags & 8))
        return;
    if (s->h.timer == 0)
        s->func = Boss7PartD6_State_Vanish;
}

/* Boss7PartD6_State_Vanish (modified: NNS_SndPlayerStopSeq(handle, 0x3c) -> sfx_stop) */
static void Boss7PartD6_State_Vanish(void *p) {
    Boss7PartD6 *w = (Boss7PartD6 *)p;
    BossModel *m = &w->unk20C;
    Model_SetAnimation(m, 1, w->unk394, 0);
    Model_SetAnimation(m, 2, w->unk3A0, 0);
    w->coll1.flags &= ~4;
    w->h.disp_flags &= ~4;
    if (w->unk378 == 0) {
        w->unk380->h.obj_flags |= 8;
        boss_sfx_stop(w->unk384);
    } else {
        m->speed = data_ov000_02363b64[w->unk37C];
    }
    w->func = Boss7PartD6_State_DestroyOnAnimEnd;
}

/* Boss7PartD6_State_DestroyOnAnimEnd (copied) */
static void Boss7PartD6_State_DestroyOnAnimEnd(void *p) {
    Boss7PartD6 *s = (Boss7PartD6 *)p;
    if (s->h.disp_flags & 8)
        s->h.obj_flags |= 4;
}

/* ============================================================ Boss7PartD7: clash aura */
/* Boss7PartD7_Draw (modified: Model_Draw -> pose) */
static void Boss7PartD7_Draw(void *p) {
    Boss7PartD7 *obj = (Boss7PartD7 *)p;
    BossModel *cam = &obj->unk20C;
    cam->posed = 0;
    if (obj->h.disp_flags & 0x20)
        return;
    obj->h.x = obj->unk348->x;
    obj->h.y = obj->unk348->y;
    obj->h.z = obj->unk348->z;
    if (obj->unk348->disp_flags & 1)
        obj->h.disp_flags |= 1;
    else
        obj->h.disp_flags &= ~1;
    obj->h.disp_flags &= ~8;
    if (obj->h.disp_flags & 4) {
        cam->flags[0] |= 4; /* unk10C */
        cam->flags[1] |= 4; /* unk10D */
    } else {
        cam->flags[0] &= ~4;
        cam->flags[1] &= ~4;
    }
    Boss7_SetFacingRot(cam, obj->h.disp_flags & 1);
    cam->trans[0] = obj->h.x << 4;
    cam->trans[1] = -(obj->h.y << 4);
    cam->trans[2] = obj->h.z << 4;
    Model_Animate(cam);
    boss_model_pose(cam, NULL);
    if (cam->flags[0] & 2)
        obj->h.disp_flags |= 8;
    if (cam->flags[1] & 2)
        obj->h.disp_flags |= 8;
}

/* Boss7PartD7_Destructor (modified) */
static void Boss7PartD7_Destructor(void *p) {
    Boss7PartD7 *s = (Boss7PartD7 *)p;
    boss_model_release(&s->unk20C);
    rect_forget(&s->coll0);
    rect_forget(&s->coll1);
    rect_forget(&s->coll2);
}

/* Boss7PartD7_Create (modified: pool slot, archive files -> converted packages) */
static Boss7PartD7 *Boss7PartD7_Create(const ObjParam *p, s32 b, s32 c) {
    Boss7PartD7 *w = NULL;
    BossModel *sub;
    int i, k;
    for (i = 0; i < B7_D7 * 2; i++)
        if (!s_d7[i].alive) {
            w = &s_d7[i];
            break;
        }
    if (w == NULL)
        return NULL;
    boss_model_release(&w->unk20C);
    memset(w, 0, sizeof(*w));
    w->alive = 1;
    w->seq = ++s_seq;
    w->func = Boss7PartD7_State_Hide;
    w->unk344 = p->flags; /* p->unk4 */
    k = p->flags == 0 ? 0 : 1; /* 0x20..0x26 (son) / 0x27..0x2d (blz) */
    w->unk34C = &s_d7_mdl[k];
    w->unk350 = &s_d7_ca[k][0];
    w->unk35C = &s_d7_ma[k][0];
    w->unk354 = &s_d7_ca[k][1];
    w->unk360 = &s_d7_ma[k][1];
    w->unk358 = &s_d7_ca[k][2];
    w->unk364 = &s_d7_ma[k][2];
    MapObject_Init(&w->h, &w->coll0, p, b, c);
    w->h.obj_flags |= 0x10;
    w->h.move_flags |= 0x3300;
    w->h.disp_flags |= 0x20;
    sub = &w->unk20C;
    Model_Init(sub, 0);
    Model_SetResource(sub, w->unk34C);
    sub->scale[0] = sub->scale[1] = sub->scale[2] = 0x34cc;
    s_st.auras++;
    return w;
}

/* Boss7PartD7_State_Hide (copied) */
static void Boss7PartD7_State_Hide(void *p) {
    Boss7PartD7 *s = (Boss7PartD7 *)p;
    s->h.disp_flags |= 0x20;
    s->func = Boss7PartD7_State_Hidden;
}
/* Boss7PartD7_State_Hidden (copied) */
static void Boss7PartD7_State_Hidden(void *p) { (void)p; }
/* Boss7PartD7_State_Loop (copied) */
static void Boss7PartD7_State_Loop(void *p) { (void)p; }

/* Boss7PartD7_State_StartAppear (copied) */
static void Boss7PartD7_State_StartAppear(void *p) {
    Boss7PartD7 *s = (Boss7PartD7 *)p;
    BossModel *m = &s->unk20C;
    s->h.disp_flags &= ~0x24;
    Model_SetAnimation(m, 0, s->unk350, 0);
    Model_SetAnimation(m, 1, s->unk35C, 0);
    s->func = Boss7PartD7_State_WaitAnimEnd_ov000_022ebf08;
}
/* Boss7PartD7_State_WaitAnimEnd_ov000_022ebf08 (copied) */
static void Boss7PartD7_State_WaitAnimEnd_ov000_022ebf08(void *p) {
    Boss7PartD7 *s = (Boss7PartD7 *)p;
    if ((s->h.disp_flags & 8) == 0)
        return;
    s->func = Boss7PartD7_State_StartLoop;
    s->func(s);
}
/* Boss7PartD7_State_StartLoop (copied) */
static void Boss7PartD7_State_StartLoop(void *p) {
    Boss7PartD7 *s = (Boss7PartD7 *)p;
    BossModel *m = &s->unk20C;
    s->h.disp_flags |= 4;
    Model_SetAnimation(m, 0, s->unk354, 0);
    Model_SetAnimation(m, 1, s->unk360, 0);
    s->func = Boss7PartD7_State_Loop;
}
/* Boss7PartD7_State_WaitAnimEnd_ov000_022ebe64 (copied) */
static void Boss7PartD7_State_WaitAnimEnd_ov000_022ebe64(void *p) {
    Boss7PartD7 *s = (Boss7PartD7 *)p;
    if ((s->h.disp_flags & 8) == 0)
        return;
    s->func = Boss7PartD7_State_StartDisappear;
    s->func(s);
}
/* Boss7PartD7_State_StartDisappear (copied) */
static void Boss7PartD7_State_StartDisappear(void *p) {
    Boss7PartD7 *s = (Boss7PartD7 *)p;
    BossModel *m = &s->unk20C;
    s->h.disp_flags &= ~4;
    Model_SetAnimation(m, 0, s->unk358, 0);
    Model_SetAnimation(m, 1, s->unk364, 0);
    s->func = Boss7PartD7_State_WaitAnimEnd_ov000_022ebdc4;
}
/* Boss7PartD7_State_WaitAnimEnd_ov000_022ebdc4 (copied) */
static void Boss7PartD7_State_WaitAnimEnd_ov000_022ebdc4(void *p) {
    Boss7PartD7 *s = (Boss7PartD7 *)p;
    if ((s->h.disp_flags & 8) == 0)
        return;
    s->func = Boss7PartD7_State_Hide;
    s->func(s);
}

/* ============================================================ stage camera director */
/* StageZoneScript_Zone7Boss (copied; task 0x3eff, after ScreenShake, before BossCamera: run at the
 * end of boss7_update, nothing in between reads or writes what it uses).  gPlayers->unk17C, the
 * player model translation, is (x << 4) in this flat arena (Player_Draw). */
static void StageZoneScript_Zone7Boss(void) {
    Boss7 *ctx = gBoss7;
    fx32 a48, b48;
    s32 t;
    if (ctx == NULL || !ctx->alive || ctx->unk214 == NULL)
        return;
    a48 = gPlayers != NULL ? gPlayers->x << 4 : 0; /* a->unk48 */
    b48 = ctx->unk214->unk20C.trans[0];            /* b->unk48 */
    switch (ctx->unk210) {
    case 0:
        break;
    case 1:
        CAM.t[1].ox = CAM.t[1].oy = CAM.t[1].oz = NULL; /* p->unk3C..44 */
        CAM.t[1].tgt.x = (a48 + b48) / 2;               /* p->unk54 */
        CAM.t[1].tgt.y = -0x1d8000;
        CAM.t[1].tgt.z = 0;
        CAM.t[0].ox = CAM.t[0].oy = CAM.t[0].oz = NULL; /* p->unk0..8 */
        CAM.t[0].tgt.x = (a48 + b48) / 2;               /* p->unk18 */
        CAM.t[0].tgt.y = -0x1d0000;
        CAM.t[0].tgt.z = a48 - b48 < 0 ? b48 - a48 : a48 - b48;
        if (CAM.t[0].tgt.z < 0x96000)
            CAM.t[0].tgt.z = 0x96000;
        if (CAM.t[0].tgt.z > 0xc8000)
            CAM.t[0].tgt.z = 0xc8000;
        t = FX_Mul(FX_Div(CAM.t[0].tgt.z - 0x96000, 0x32000), 0x30000);
        CAM.t[1].tgt.y += t;
        CAM.t[0].tgt.y += t;
        break;
    case 2:
        CAM.t[1].ox = CAM.t[1].oy = CAM.t[1].oz = NULL;
        CAM.t[1].tgt.x = 0xa0000;
        CAM.t[1].tgt.y = -0x1d8000;
        CAM.t[1].tgt.z = 0;
        CAM.t[0].ox = CAM.t[0].oy = CAM.t[0].oz = NULL;
        CAM.t[0].tgt.x = 0xa0000;
        CAM.t[0].tgt.y = -0x1d0000;
        CAM.t[0].tgt.z = 0xc8000;
        t = FX_Mul(FX_Div(CAM.t[0].tgt.z - 0x96000, 0x32000), 0x30000);
        CAM.t[1].tgt.y += t;
        CAM.t[0].tgt.y += t;
        break;
    case 3:
        CAM.t[1].ox = CAM.t[1].oy = CAM.t[1].oz = NULL;
        CAM.t[1].tgt.x = 0xa0000;
        CAM.t[1].tgt.y = -0x1d8000;
        CAM.t[1].tgt.z = 0;
        CAM.unkC0 = a48 - b48 < 0 ? b48 - a48 : a48 - b48; /* p->unk78 */
        if (CAM.unkC0 < 0x50000)
            CAM.unkC0 = 0x50000;
        if (CAM.unkC0 > 0xc8000)
            CAM.unkC0 = 0xc8000;
        CAM.t[1].tgt.y += FX_Mul(FX_Div(0x78000 - (CAM.unkC0 - 0x50000), 0x78000), -0x10000);
        CAM.unkCC = 0x4000; /* p->unk84 */
        CAM.unkCC += FX_Mul(FX_Div(0x78000 - (CAM.unkC0 - 0x50000), 0x78000), 0xc000);
        CAM.unkD6 = (u16)((-FX_Div(ctx->unk384 << 12, 0x4000000) * 0x2000) >> 12); /* p->unk8E */
        break;
    }
}

/* ============================================================ resources and API */
static const char *const s_d6_names[2][6] = {
    {"eff_tornado01.nsbma", "eff_tornado02.nsbma", "eff_tornado03.nsbma", "eff_tornado01.nsbtp", "eff_tornado02.nsbtp",
     "eff_tornado03.nsbtp"},
    {"eff_flame01.nsbma", "eff_flame02.nsbma", "eff_flame03.nsbma", "eff_flame01.nsbtp", "eff_flame02.nsbtp",
     "eff_flame03.nsbtp"}};

static int timing_res(BossAnimRes *r, const char *name) {
    memset(r, 0, sizeof(*r));
    if (boss_anim_lengths(name, &r->nframes, &r->count) != 0) {
        printf("boss7: no frame counts for %s in bossdata.bin\n", name);
        return -1;
    }
    return 0;
}

static int load_bac(void) {
    char path[192];
    FILE *f;
    long n;
    snprintf(path, sizeof(path), "%s/boss7bac.bin", boss_data_dir());
    f = fopen(path, "rb");
    if (f == NULL) {
        printf("boss7: cannot open %s\n", path);
        return -1;
    }
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    fseek(f, 0, SEEK_SET);
    s_bac = (u8 *)malloc((size_t)n);
    if (s_bac == NULL || n < 16 || fread(s_bac, 1, (size_t)n, f) != (size_t)n || memcmp(s_bac, "B7BC", 4) != 0) {
        fclose(f);
        free(s_bac);
        s_bac = NULL;
        return -1;
    }
    fclose(f);
    s_bac_nanims = *(u16 *)(s_bac + 6);
    s_bac_nframes = *(u16 *)(s_bac + 8);
    s_bac_nevents = *(u16 *)(s_bac + 10);
    if (16 + s_bac_nanims * sizeof(Boss7BacAnim) + s_bac_nframes * sizeof(Boss7BacFrame) +
            s_bac_nevents * sizeof(Boss7BacEvent) > (size_t)n) {
        free(s_bac);
        s_bac = NULL;
        return -1;
    }
    s_bac_anims = (const Boss7BacAnim *)(s_bac + 16);
    s_bac_frames = (const Boss7BacFrame *)(s_bac_anims + s_bac_nanims);
    s_bac_events = (const Boss7BacEvent *)(s_bac_frames + s_bac_nframes);
    return 0;
}

/* BossStage7_OnModelLoaded (substantially rewritten): bs7_base / bs7_sky (data_02074164) plus the
 * archive files the Boss7 objects take (NaviBg_Load / NaviB_Load of the sub screen dropped) */
int boss7_load(void) {
    int r = 0, i, k, rc;
    DcPlayer *pl = g_boss_game.player;
    static const char *const rival[2][2] = {{"mod__son__son.dcm", "mod__son__son_ball.dcm"},
                                            {"mod__blz__blz.dcm", "mod__blz__blz_spin.dcm"}};
    static const char *const aura[2] = {"son", "blz"};
    char name[96];
    /* gGameState.unk0: the story character.  g_boss_game.unk0 when the integrator set it, else
     * the player's character (the same in story mode) */
    s_story = g_boss_game.unk0 != 0 || pl == NULL ? g_boss_game.unk0 : pl->chara;
    rc = s_story == 0 ? 1 : 0; /* the rival: Blaze against Sonic */
    r |= boss_res_load_model(&data_ov000_02369b7c[0], "mod__bs7_base__bs7_base.dcm");
    r |= boss_res_load_model(&data_ov000_02369b7c[1], "mod__bs7_sky__bs7_sky.dcm");
    r |= boss_res_load_anim(&s_bs7_ca, "narc__z73_act_lz7__bs7.dca");
    r |= boss_res_load_anim(&s_bs7_va, "narc__z73_act_lz7__bs7.dcv");
    r |= boss_res_load_model(&s_rival_mdl[0], rival[rc][0]);
    r |= boss_res_load_model(&s_rival_mdl[1], rival[rc][1]);
    r |= boss_res_load_anim(&s_son_ca, "narc__player_lz7__son.dca");
    r |= boss_res_load_anim(&s_son_va, "narc__player_lz7__son.dcv");
    r |= boss_res_load_model(&s_d6_mdl, rc == 0 ? "narc__z73_act_lz7__eff_tornado__eff_tornado.dcm"
                                                : "narc__z73_act_lz7__eff_flame__eff_flame.dcm");
    for (i = 0; i < 6; i++)
        r |= timing_res(&s_d6_anm[i], s_d6_names[rc][i]);
    for (k = 0; k < 2; k++) {
        snprintf(name, sizeof(name), "narc__z73_act_lz7__eff_aura_%s__eff_aura_%s.dcm", aura[k], aura[k]);
        r |= boss_res_load_model(&s_d7_mdl[k], name);
        for (i = 0; i < 3; i++) {
            snprintf(name, sizeof(name), "narc__z73_act_lz7__eff_aura_%s%02d.dca", aura[k], i + 1);
            r |= boss_res_load_anim(&s_d7_ca[k][i], name);
            snprintf(name, sizeof(name), "eff_aura_%s%02d.nsbma", aura[k], i + 1);
            r |= timing_res(&s_d7_ma[k][i], name);
        }
    }
    r |= load_bac();
    for (i = 0; i < 2; i++) { /* own material colours of bs7_base / bs7_sky */
        const BossModelRes *m = &data_ov000_02369b7c[i];
        s_mat_use_own[i][0] = s_mat_use_own[i][1] = 1;
        free(s_mat_own[i]);
        s_mat_own[i] = NULL;
        if (m->loaded && m->mdl.hdr->num_materials) {
            s_mat_own[i] = (u32 *)malloc(m->mdl.hdr->num_materials * 2 * sizeof(u32));
            if (s_mat_own[i] != NULL)
                for (k = 0; k < (int)m->mdl.hdr->num_materials; k++) {
                    s_mat_own[i][k * 2] = m->mdl.materials[k].diffuse;
                    s_mat_own[i][k * 2 + 1] = m->mdl.materials[k].ambient;
                }
        }
    }
    s_glb_diff = s_glb_amb = 0x7fff;
    return r ? -1 : 0;
}

/* BossStage7_FreeModels (substantially rewritten) */
void boss7_unload(void) {
    int i, k;
    for (i = 0; i < 2; i++) {
        boss_res_free_model(&data_ov000_02369b7c[i]);
        free(s_mat_own[i]);
        s_mat_own[i] = NULL;
        boss_res_free_model(&s_rival_mdl[i]);
        boss_res_free_model(&s_d7_mdl[i]);
        for (k = 0; k < 3; k++)
            boss_res_free_anim(&s_d7_ca[i][k]);
    }
    boss_res_free_anim(&s_bs7_ca);
    boss_res_free_anim(&s_bs7_va);
    boss_res_free_anim(&s_son_ca);
    boss_res_free_anim(&s_son_va);
    boss_res_free_model(&s_d6_mdl);
    free(s_bac);
    s_bac = NULL;
}

static const Boss7Ops s_ops_boss7 = {Boss7_Draw, NULL, Boss7_Destructor};
static const Boss7Ops s_ops_d5 = {Boss7PartD5_Draw, Boss7PartD5_Collide, Boss7PartD5_Destructor};
static const Boss7Ops s_ops_d6 = {Boss7PartD6_Draw, Boss7PartD6_Collide, Boss7PartD6_Destructor};
static const Boss7Ops s_ops_d7 = {Boss7PartD7_Draw, NULL, Boss7PartD7_Destructor};

/* NNS_G3dGlbLightVector / LightColor: the zone set and the after-image colours, in view space */
static void boss7_build_lights(void) {
    dcm_camera_t cam;
    int i, c, z = g_dcp_world.zone, a = g_dcp_world.act;
    if (z > 7)
        z = 6;
    if (a > 2)
        a = 2;
    boss_camera(&cam);
    memset(&s_lights, 0, sizeof(s_lights));
    s_lights.count = 3;
    for (i = 0; i < 3; i++) {
        float v[3];
        u16 col = data_02074960[z][a][i];
        v[0] = data_020748bc[z][i][0] / 4096.0f;
        v[1] = data_020748bc[z][i][1] / 4096.0f;
        v[2] = data_020748bc[z][i][2] / 4096.0f;
        for (c = 0; c < 3; c++)
            s_lights.dir[i][c] = v[0] * cam.view.m[0][c] + v[1] * cam.view.m[1][c] + v[2] * cam.view.m[2][c];
        s_lights.col[i][0] = (col & 31) / 31.0f;
        s_lights.col[i][1] = ((col >> 5) & 31) / 31.0f;
        s_lights.col[i][2] = ((col >> 10) & 31) / 31.0f;
    }
    s_lights_after = s_lights;
    for (i = 0; i < 3; i++) {
        s_lights_after.col[i][0] = (s_after_col & 31) / 31.0f;
        s_lights_after.col[i][1] = ((s_after_col >> 5) & 31) / 31.0f;
        s_lights_after.col[i][2] = ((s_after_col >> 10) & 31) / 31.0f;
    }
    g_boss_lights = &s_lights;
}

void boss7_update(void) {
    u32 last;
    Boss7 *s = &s_boss7;
    if (gPlayers != NULL && s_player_spd_anim >= 0 && gPlayers->anim != s_player_spd_anim) {
        s_player_spd = 0x1000; /* Player_SetAnimation resets the model speed */
        s_player_spd_anim = -1;
    }
    if (s->alive)                                                    /* 0x500 */
        boss7_obj_main(&s->h, &s->func, &s->unk1F0, &s->coll0, &s_ops_boss7, s, &s->alive);
    if (s_d5.alive)                                                  /* 0x501 */
        boss7_obj_main(&s_d5.h, &s_d5.func, &s_d5.unk1F0, &s_d5.coll0, &s_ops_d5, &s_d5, &s_d5.alive);
    /* 0x502: D6 / D7 in creation order (Task_Create appends after the same priority, so a part
     * created this frame still runs this frame) */
    last = 0;
    for (;;) {
        u32 best = 0xffffffffu;
        int i, bi = -1, bk = 0;
        for (i = 0; i < B7_D6; i++)
            if (s_d6[i].alive && s_d6[i].seq > last && s_d6[i].seq < best) {
                best = s_d6[i].seq;
                bi = i;
                bk = 6;
            }
        for (i = 0; i < B7_D7 * 2; i++)
            if (s_d7[i].alive && s_d7[i].seq > last && s_d7[i].seq < best) {
                best = s_d7[i].seq;
                bi = i;
                bk = 7;
            }
        if (bi < 0)
            break;
        last = best;
        if (bk == 6)
            boss7_obj_main(&s_d6[bi].h, &s_d6[bi].func, &s_d6[bi].unk1F0, &s_d6[bi].coll0, &s_ops_d6, &s_d6[bi],
                           &s_d6[bi].alive);
        else
            boss7_obj_main(&s_d7[bi].h, &s_d7[bi].func, &s_d7[bi].unk1F0, &s_d7[bi].coll0, &s_ops_d7, &s_d7[bi],
                           &s_d7[bi].alive);
    }
    if (s_counter_task && gBoss7 != NULL)                            /* 0x503 */
        Boss7FrameCounter_Main();
    StageZoneScript_Zone7Boss();                                     /* 0x3eff */
    boss7_build_lights();
}

int boss7_draw(int list) {
    int n = 0, i;
    if (!s_boss7.alive && !s_d5.alive)
        return 0;
    boss7_build_lights();
    g_boss_debug_color = 0x8080ff;
    if (s_boss7.alive)
        n += boss7_model_draw(&s_boss7.unk24C, s_boss7.unk24C.palette, &s_boss7.unk24C.mdl_mtx, s_boss7.vis, &s_lights,
                              list);
    if (s_d5.alive) {
        g_boss_debug_color = 0xff8040;
        n += boss7_model_draw(&s_d5.unk20C, s_d5.unk20C.palette, &s_d5.unk20C.mdl_mtx, s_d5.vis, &s_lights, list);
        if (s_d5.after_posed)
            n += boss7_model_draw(&s_d5.unk20C, g_boss7_after_pal, &s_d5.after_mtx, s_d5.after_vis, &s_lights_after,
                                  list);
    }
    g_boss_debug_color = 0xff4040;
    for (i = 0; i < B7_D6; i++)
        if (s_d6[i].alive)
            n += boss_model_draw(&s_d6[i].unk20C, list);
    g_boss_debug_color = 0x40ffff;
    for (i = 0; i < B7_D7 * 2; i++)
        if (s_d7[i].alive)
            n += boss_model_draw(&s_d7[i].unk20C, list);
    g_boss_debug_color = 0xffff40;
    return n;
}

void boss7_destroy(void) {
    int i;
    if (s_boss7.alive)
        Boss7_Destructor(&s_boss7);
    if (s_d5.alive)
        Boss7PartD5_Destructor(&s_d5);
    for (i = 0; i < B7_D6; i++)
        if (s_d6[i].alive)
            Boss7PartD6_Destructor(&s_d6[i]);
    for (i = 0; i < B7_D7 * 2; i++)
        if (s_d7[i].alive)
            Boss7PartD7_Destructor(&s_d7[i]);
    memset(&s_boss7, 0, sizeof(s_boss7));
    memset(&s_d5, 0, sizeof(s_d5));
    memset(s_d6, 0, sizeof(s_d6));
    memset(s_d7, 0, sizeof(s_d7));
    memset(&s_st, 0, sizeof(s_st));
    gBoss7 = NULL;
    s_counter_task = 0;
    s_won = s_lost = s_cleared_called = 0;
    s_blend_mode = s_blend_evy = 0;
    s_player_spd = 0x1000;
    s_player_spd_anim = -1;
    s_after_col = 0;
}

int boss7_defeated(void) { return s_won; }

/* ---------------------------------------------------- names (tests / debug) */
#define N(f) {(void (*)(void))f, #f}
typedef struct {
    void (*fn)(void);
    const char *name;
} Boss7Name;
static const Boss7Name s_names[] = {
    N(Boss7_State_WaitForStart), N(Boss7_State_Fight), N(Boss7_State_None), N(Boss7_State_EndFight),
    N(Boss7_State_StartEndWait), N(Boss7_State_WaitTimer_ov000_022edc70), N(Boss7_Action_StartTalkDemo),
    N(Boss7_State_ClashFadeOutBGM), N(Boss7_State_ClashWaitDelay), N(Boss7_State_ClashSpawnPartD7),
    N(Boss7_State_ClashStartFlash), N(Boss7_State_ClashFlash), N(Boss7_State_ClashStartApproach),
    N(Boss7_State_ClashApproach), N(Boss7_State_ClashStart), N(Boss7_State_ClashStruggle), N(Boss7_State_ClashEnd),
    N(Boss7_State_WaitTimer_ov000_022ed138), N(Boss7_State_ClashResolve), N(Boss7_State_ClashLost),
    N(Boss7_State_ClashLostRivalSkid), N(Boss7_State_ClashWon), N(Boss7_State_WinCut1), N(Boss7_State_WinCutWait1),
    N(Boss7_State_WinCut2), N(Boss7_State_WinCutWait2), N(Boss7_State_WinCut3), N(Boss7_State_WinCutWait3),
    N(Boss7_State_StartRivalExit), N(Boss7_State_PlayerSkidToStop),
    N(Boss7PartD5_MoveState_StartIdle), N(Boss7PartD5_MoveState_Idle), N(Boss7PartD5_MoveState_StartRun),
    N(Boss7PartD5_MoveState_Run), N(Boss7PartD5_MoveState_StartJump), N(Boss7PartD5_MoveState_Jump),
    N(Boss7PartD5_MoveState_StartCrouch), N(Boss7PartD5_MoveState_Crouch), N(Boss7PartD5_MoveState_StartSpindashCharge),
    N(Boss7PartD5_MoveState_SpindashCharge), N(Boss7PartD5_MoveState_SpindashRelease), N(Boss7PartD5_MoveState_Roll),
    N(Boss7PartD5_MoveState_StartBackHop), N(Boss7PartD5_MoveState_BackHop),
    N(Boss7PartD5_MoveState_StartAirAttackCharge), N(Boss7PartD5_MoveState_AirAttackCharge),
    N(Boss7PartD5_MoveState_StartAirDash), N(Boss7PartD5_MoveState_AirDash), N(Boss7PartD5_MoveState_StartDashSkid),
    N(Boss7PartD5_MoveState_DashSkid), N(Boss7PartD5_MoveState_StartHurt), N(Boss7PartD5_MoveState_Hurt),
    N(Boss7PartD5_MoveState_WaitDefeatScene), N(Boss7PartD5_MoveState_DefeatScene),
    N(Boss7PartD5_MoveState_DefeatSceneDone), N(Boss7PartD5_MoveState_StartDefeatExit),
    N(Boss7PartD5_MoveState_DefeatExit), N(Boss7PartD5_MoveState_StartSonicSpecial),
    N(Boss7PartD5_MoveState_SonicSpecialRise), N(Boss7PartD5_MoveState_SonicSpecialStartOrbit),
    N(Boss7PartD5_MoveState_SonicSpecialSpinUp), N(Boss7PartD5_MoveState_SonicSpecialOrbit),
    N(Boss7PartD5_MoveState_SonicSpecialWaitDashAngle), N(Boss7PartD5_MoveState_SonicSpecialDashToPlayer),
    N(Boss7PartD5_MoveState_SonicSpecialStartCharge), N(Boss7PartD5_MoveState_SonicSpecialCharge),
    N(Boss7PartD5_MoveState_SonicSpecialSpinDown), N(Boss7PartD5_MoveState_SonicSpecialStartFall),
    N(Boss7PartD5_MoveState_SonicSpecialFall), N(Boss7PartD5_MoveState_StartBlazeSpecial),
    N(Boss7PartD5_MoveState_BlazeSpecialRise), N(Boss7PartD5_MoveState_BlazeSpecialStartHover),
    N(Boss7PartD5_MoveState_BlazeSpecialHoverDelay), N(Boss7PartD5_MoveState_BlazeSpecialSpawnPillar),
    N(Boss7PartD5_MoveState_BlazeSpecialHover), N(Boss7PartD5_MoveState_BlazeSpecialEnd),
    N(Boss7PartD5_MoveState_BlazeSpecialFall),
    N(Boss7PartD5_InputState_None), N(Boss7PartD5_InputState_Decide), N(Boss7PartD5_InputState_Walk),
    N(Boss7PartD5_InputState_StartJump), N(Boss7PartD5_InputState_HoldJump), N(Boss7PartD5_InputState_AirSteer),
    N(Boss7PartD5_InputState_StartAirAttack), N(Boss7PartD5_InputState_StartJumpThenAirAttack),
    N(Boss7PartD5_InputState_HoldJumpThenAirAttack), N(Boss7PartD5_InputState_BeginAirAttackCharge),
    N(Boss7PartD5_InputState_HoldAirAttackCharge), N(Boss7PartD5_InputState_StartSpindash),
    N(Boss7PartD5_InputState_CrouchTowardTarget), N(Boss7PartD5_InputState_CheckCrouched),
    N(Boss7PartD5_InputState_ChargeSpindash), N(Boss7PartD5_InputState_RollThenStop),
    N(Boss7PartD5_InputState_StartBackHop), N(Boss7PartD5_InputState_TurnAround),
    N(Boss7PartD5_InputState_WaitAfterTurn), N(Boss7PartD5_InputState_TapBack), N(Boss7PartD5_InputState_TapBackAgain),
    N(Boss7PartD5_InputState_StartSpecial), N(Boss7PartD5_InputState_TriggerSpecial),
    N(Boss7PartD5_InputState_StartSpecialAirWait), N(Boss7PartD5_InputState_SpecialAirWait),
    N(Boss7PartD5_InputState_Defeated), N(Boss7PartD5_InputState_StartClashMash), N(Boss7PartD5_InputState_ClashMash),
};
#undef N
static const char *fn_name(void (*fn)(void)) {
    size_t i;
    if (fn == NULL)
        return "-";
    for (i = 0; i < sizeof(s_names) / sizeof(s_names[0]); i++)
        if (s_names[i].fn == fn) {
            const char *n = s_names[i].name;
            if (!strncmp(n, "Boss7PartD5_", 12))
                return n + 12;
            if (!strncmp(n, "Boss7_", 6))
                return n + 6;
            return n;
        }
    return "?";
}
/* part 0: Boss7 state, 1: rival move state, 2: rival input state */
const char *boss7_state_name(int part) {
    switch (part) {
    case 0:
        return s_boss7.alive ? fn_name((void (*)(void))s_boss7.func) : "-";
    case 1:
        return s_d5.alive ? fn_name((void (*)(void))s_d5.unk440) : "-";
    case 2:
        return s_d5.alive ? fn_name((void (*)(void))s_d5.unk43C) : "-";
    }
    return "-";
}

void boss7_body_pos(s32 *x, s32 *y, s32 *z) {
    *x = s_d5.h.x;
    *y = s_d5.h.y;
    *z = s_d5.h.z;
}
int boss7_box_armed(int box) {
    if (!s_d5.alive)
        return 0;
    return box == 0 ? (s_d5.unk378.flags & 4) != 0 : box == 1 ? (s_d5.unk3A4.flags & 4) != 0 : 0;
}

void boss7_blend(s32 *mode, s32 *evy) {
    *mode = s_blend_evy != 0 ? s_blend_mode : 0;
    *evy = s_blend_evy;
}
void boss7_set_talk_hook(Boss7TalkFn fn, void *ctx) {
    s_talk_fn = fn;
    s_talk_ctx = ctx;
}
const Boss7Stats *boss7_stats(void) { return &s_st; }

int boss7_info(Boss7Info *o) {
    int i;
    memset(o, 0, sizeof(*o));
    if (!s_d5.alive && !s_boss7.alive)
        return 0;
    o->rival_chara = s_d5.unk3D0;
    o->rival_x = s_d5.h.x;
    o->rival_y = s_d5.h.y;
    o->rival_z = s_d5.h.z;
    o->rival_vx = s_d5.h.vel_x;
    o->rival_vy = s_d5.h.vel_y;
    o->rival_anim = s_d5.unk3D4;
    o->rival_disp = s_d5.h.disp_flags;
    o->rival_box_armed[0] = (s_d5.unk378.flags & 4) != 0;
    o->rival_box_armed[1] = (s_d5.unk3A4.flags & 4) != 0;
    o->rival_box[0] = s_d5.unk378;
    o->rival_box[1] = s_d5.unk3A4;
    o->rival_pad_held = s_d5.unk3D8.cur;
    o->rival_pad_pressed = s_d5.unk3D8.trg;
    o->rival_move = boss7_state_name(1);
    o->rival_input = boss7_state_name(2);
    o->hurt_timer = s_d5.unk1F0;
    o->flags20C = s_boss7.unk20C;
    o->camera_mode = s_boss7.unk210;
    o->balance = s_boss7.unk384;
    o->power_player = s_boss7.unk390;
    o->power_rival = s_boss7.unk38C;
    o->pull = s_boss7.unk22A;
    o->clash = s_boss7.func == Boss7_State_ClashStruggle;
    o->won = s_won;
    o->lost = s_lost;
    for (i = 0; i < B7_D6; i++)
        o->pillars += s_d6[i].alive;
    for (i = 0; i < B7_D7 * 2; i++)
        o->auras += s_d7[i].alive && !(s_d7[i].h.disp_flags & 0x20);
    o->player_anim_speed = s_player_spd;
    boss7_blend(&o->blend_mode, &o->blend_evy);
    if (s_boss7.unk24C.res != NULL && s_boss7.unk24C.res->loaded) {
        o->base_nodes = s_boss7.unk24C.res->mdl.hdr->num_joints;
        for (i = 0; i < o->base_nodes && i < DCM_MAX_JOINTS; i++)
            o->base_hidden += s_boss7.vis[i] == 0;
        if (s_boss7.unk24C.res->mdl.hdr->num_materials)
            o->base_diffuse = s_boss7.unk24C.res->mdl.materials[0].diffuse;
    }
    o->rival_posed = s_d5.unk20C.posed;
    o->afterimage = s_d5.after_posed;
    return 1;
}

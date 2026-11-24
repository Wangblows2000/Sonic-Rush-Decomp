/* Internal interfaces of the native object module (not for the integrator). */
#ifndef OBJ_INTERNAL_H
#define OBJ_INTERNAL_H

#include "objects.h"

#define W g_dcp_world
#define RAND() dcp_rand() /* gRandSeed LCG (shared with the player) */

/* ------------------------------------------------- global state */
typedef struct ObjView { s32 x, y; s16 w, h; } ObjView; /* gCamera[n] @0x04 @0x08 @0x1E @0x20 */
typedef struct ObjData {               /* objdata.bin (tools/objdata.py) */
    u8 margin[0xf2];                   /* data_ov000_023686e0 */
    s16 ring_gravity;                  /* data_ov000_023690e4 */
    u8 itembox_rings[8];               /* data_ov000_02366b44 */
    u8 itembox_vs[8];                  /* data_ov000_02366b3c */
    u8 player_spr_anim[0x68];          /* data_ov000_02368d14 */
    u8 solid[48][8];                   /* data_ov000_023661c0: all-solid collision tiles
                                          (Needle uses the first 12 as needle_shape) */
    u8 animal[8][3];                   /* data_ov000_023661a8 */
    s8 shake[16];                      /* data_02077944 */
    /* objdata v2 */
    u8 col_attr[4];                    /* data_ov000_023661a4: top-solid attribute (1) */
    s8 bend_wobble[28];                /* data_ov000_023643d4 / _023643d8 / _023643dc */
    s16 pedestal[4][6];                /* data_ov000_02364400: JumpPedestal launch speeds */
    u8 dec_margin[0xf2];               /* data_ov000_023685ec: decoration spawn margin */
    /* objdata v4 */
    s16 valve_ofs[4][2];               /* data_ov000_02364444 */
    s16 valve_box[4][4];               /* data_ov000_02364454 */
    s16 valve_puff[4][4];              /* data_ov000_02364474 */
    u32 valve_anim[4][3];              /* data_ov000_02364494 */
    char ids[OBJ_BAC_COUNT][48];       /* resolved sprite ids of this stage */
    struct { char name[24]; u32 size; const u8 *data; } df[8]; /* the .df files (dat/gmk) of this stage */
    int n_df;
    u8 *df_buf;
} ObjData;
#define needle_shape solid

typedef struct ObjPlayerCtx {          /* per player: the player's ObjRects + sprite timing */
    DcPlayer *p;
    ObjBox body;                       /* @0x2A0 coll2A0 (hurt box, Player_OnHurt) */
    ObjBox atk;                        /* @0x2CC coll2CC (attack box)             */
    ObjSprite spr;                     /* @0x94 son.bac timing (hit box events)   */
    s16 prev_invuln, prev_invinc;
    u8 prev_boost;                     /* native: pf & 0x80 last frame (Player_StopBoost resets the hurt box) */
    /* player work the player module does not keep (gimmick states, objects/obj_pgimmick.c) */
    s16 g3EC, g3EE;                    /* @0x3EC @0x3EE (GmkTable: camera look offsets) */
    s32 g3F0, g3F4, g3F8, g3FC;        /* @0x3F0..@0x3FC */
    s16 g3D2, g3D4;                    /* @0x3D2 @0x3D4 (objdata v4: cleared by Player_Action_ResumeGlider) */
    /* native, Battle Play: the player's own water level (gCamera.unk62 / flags 0x1000000, 0x2000000). On the
     * DS each console kept its own gCamera; here the objects run once with player 1's camera, so the water
     * triggers also keep a level per player (obj_water_of) */
    u16 water;
    u8 water_set;
    u32 water_flags;
} ObjPlayerCtx;

typedef struct ObjGlobals {
    const rlvl_map_t *map;
    const uint8_t *level;
    const rlvl_header_t *hdr;
    const rlvl_object_t *lv_obj;
    const uint32_t *lv_obj_region;
    const rlvl_ring_t *lv_ring;
    const uint32_t *lv_ring_region;
    ObjParam *prm;                     /* mutable copy of the .ev entries */
    u8 *ring_state;                    /* .rg entry byte 0 (0xff = placed/collected) */
    ObjParam spawn_entries[OBJ_SPAWN_ENTRIES]; /* gMapObjectSpawnEntries */
    u32 spawn_mask_lo, spawn_mask_hi;  /* gMapObjectSpawnMaskLo/Hi */
    ObjView view[OBJ_MAX_VIEWS], ev_last[OBJ_MAX_VIEWS];
    int n_views;
    int event_state;                   /* 0 = InitialSpawn pending, 1 = update */
    ObjData data;
    ObjRes res[OBJ_BAC_COUNT];
    ObjPlayerCtx players[2];
    int n_players;
    Obj pool[OBJ_MAX];
    Obj *head;                         /* task list sorted by prio */
    char obj_dir[64], spr_dir[64];
    int anim_done_flag;
    ObjStats stats;
    DcpHooks prev_hooks;               /* chained player hooks */
    s32 map_w, map_h;                  /* data_ov000_0236abd8.unkE / unk10 (px) */
    /* decorations (StageEvent spawner 2, Decoration_*) */
    const rlvl_decor_t *lv_dec;
    const uint32_t *lv_dec_region;
    ObjParam *dprm;                    /* mutable copy of the .dc entries (x0 = 0xff: spawned) */
    ObjParam dec_slots[OBJ_DEC_SLOTS]; /* gDecorationSlots */
    u32 dec_mask;                      /* gDecorationSlotMask */
    u16 dec_animal_count;              /* gDecAnimalCount */
    /* object globals of the v2 kinds */
    s32 ironball_flag;                 /* data_ov000_0236a00c */
    s16 land_spawners;                 /* data_ov000_0236a05c */
    s32 land_base;                     /* data_ov000_0236a060 */
    int lazy_loads;                    /* resources loaded after objects_init (late) */
    int preloaded;
    int no_grind_check;                /* the player module does Player_CheckStartGrind itself */
    int no_underwater;                 /* the player module does Player_UpdateUnderwater itself */
} ObjGlobals;
extern ObjGlobals g_obj;

/* --------------------------------------------- obj_res.c / obj_anim.c */
int obj_res_load(ObjRes *r, const char *spr_dir, const char *obj_dir, const char *id);
void obj_res_free(ObjRes *r);
/* ------------------------------------------------------ obj_palanim.c */
int obj_palanim_load(const char *obj_dir);            /* palanim.bin (tools/objdata.py) */
void obj_palanim_reset(void);
void obj_palanim_start(Obj *w, u32 table, s32 type);  /* PaletteAnim_Create (+ the effect hook) */
void obj_palanim_release(Obj *w);                     /* PaletteAnim_Release */
void obj_palanim_main(void);                          /* PaletteAnim_Main, once per frame */
int obj_palanim_anim(int palanim);                    /* animation whose palette is shown, -1 own */
int obj_load_file(const char *path, u8 **out, u32 *size);
void spr_init(ObjSprite *s, const ObjRes *res, u16 anim, u32 flags);  /* AnimatorSprite_Init */
void spr_set_anim(ObjSprite *s, u16 anim);                            /* AnimatorSprite_SetAnimation */
void spr_process(ObjSprite *s, ObjSprCb cb, void *owner);             /* AnimatorSprite_ProcessAnimation */
const DsprImage *spr_image(const ObjSprite *s);
void obj_set_anim(Obj *o, u16 anim);                                  /* StageObj_SetAnimation */
void obj_draw2d(Obj *o);                                              /* StageObj_Draw2D (animation part) */
void obj_draw2d_ex(s32 x, s32 y, Obj *o, ObjSprite *s);               /* StageObj_Draw2DEx (animation part) */
void obj_sprite_cb_default(void *owner, const HboxEvent *ev);         /* MapObject_SpriteCallback_Default */

/* ------------------------------------------------------- obj_rect.c */
void rect_set_box2d(ObjBox *b, s16 l, s16 t, s16 r, s16 bb);            /* ObjRect_SetBox2D */
void rect_set_box3d(ObjBox *b, s16 l, s16 t, s16 r, s16 bb, s16 zn, s16 zf); /* ObjRect_SetBox3D */
void rect_set_atkdef(ObjBox *b, u32 atk, u32 def);                      /* ObjRect_SetAtkDefFlags */
dcp_bool rect_flag_check(u32 a, u32 b);                                 /* ObjRect_FlagCheck */
void rect_register(ObjBox *b);                                          /* ObjRect_Register */
void rect_check_init(void);                                             /* ObjRect_CheckInit */
void rect_check_all(void);                                              /* ObjRect_CheckAllGroup */
void rect_no_hit(ObjBox *a, ObjBox *b);                                 /* ObjRect_FuncNoHit */
dcp_bool rect_box_check(const ObjBox *a, s32 x, s32 y, s32 ox, s16 oy, u16 w, u16 h); /* ObjRect_BoxCheck */
s32 rect_hit_center_x(const ObjBox *a, const ObjBox *b);               /* ObjRect_HitCenterX */
s32 rect_hit_center_y(const ObjBox *a, const ObjBox *b);               /* ObjRect_HitCenterY */
void obj_handle_collider(DcPlayer *h, void *owner, int kind, ObjBox *b, int has_sprite); /* StageObj_HandleCollider */

/* ----------------------------------------------------- obj_colmap.c */
void colmap_register(ObjColMap *e);                                     /* ObjCollisionMap_Register */
void colmap_commit(void);                                               /* ObjCollisionMap_CommitFrame */
void colmap_reset(void);
s32 colmap_probe_for_obj(DcPlayer *a, s32 x, s32 y, u32 flags, u32 dir, u8 *angle, u8 *attr); /* ObjCollisionMap_ProbeForObj */
s32 colmap_probe_distance(s32 x, s32 y, u32 flags, u32 dir, u8 *angle, u8 *attr);             /* ObjCollisionMap_ProbeDistance */

/* --------------------------------------------------- obj_manager.c */
Obj *obj_alloc(u16 prio, ObjFn dtor);                                   /* Task_Create */
void obj_mapobject_init(Obj *o, ObjParam *prm, s32 x, s32 y);           /* MapObject_Init */
void obj_mapobject_dtor(Obj *o);                                        /* MapObject_Destructor */
void obj_stageobj_dtor(Obj *o);                                         /* StageObj_Destructor */
void obj_load_bac(Obj *o, int bac);                                     /* StageObj_Load2dBAC */
Obj *obj_mapobject_spawn(s32 type, s32 x, s32 y, s16 flags, s8 p6, s8 p7, u8 p8, u8 p9); /* MapObject_Spawn */
dcp_bool obj_view_out(s32 x, s32 y, s16 ofs, s16 l, s16 t, s16 r, s16 b);  /* StageObj_ViewOutCheck */
dcp_bool obj_pause_check(u32 f);                                        /* StageObj_PauseCheck */
void obj_mapobject_collide_default(Obj *o);                             /* MapObject_Collide_Default */
void obj_free_player_refs(Obj *o);
dcp_bool obj_gravity_flipped(s32 x, s32 y);                             /* Stage_IsGravityFlipped */
void obj_sfx(s32 id);
s32 obj_sfx_count(s32 id);                               /* NNS_SndPlayerCountPlayingSeqBySeqArcIdx(0, id) */
/* Sound_PlaySeqArcBankPrio(w->unk1DC, 0, id, prio, -1); prio < 0: Sound_PlaySeqArcEx(w->unk1DC, 0, id) */
void obj_sfx_own(const void *owner, s32 id, s32 prio);
void obj_sfx_own_stop(const void *owner, s32 fade);     /* NNS_SndPlayerStopSeq(w->unk1DC, fade); < 0 release only */
/* volume 0: SetTrackPan(w->unk1DC, 0xffff, Sound_CalcPan(x, px)); else Sound_SetPositional(w->unk1DC, ..) */
void obj_sfx_own_pos(const void *owner, s32 x, s32 y, s32 px, s32 py, s32 volume);
void obj_sfx_own_tempo(const void *owner, s32 ratio);   /* NNS_SndPlayerSetTempoRatio(w->unk1DC, ratio), 0x100 normal */
void obj_log(const char *ev, s32 a, s32 b);
void obj_disable_colliders_set_anim(Obj *o, u16 anim);                  /* Enemy_DisableCollidersSetAnim */
typedef Obj *(*ObjCreateFn)(ObjParam *p, s32 x, s32 y);
ObjCreateFn obj_create_fn(u16 type);                                    /* gMapObjectCreateTable */

/* player helpers (obj_player.c) */
ObjPlayerCtx *obj_player_ctx(const DcPlayer *p);
/* Battle Play (native): the water level that applies to player p (its own level once a water trigger
 * set it, else the camera's), and the per-player update the water triggers make */
int obj_vs_split_water(void);
u16 obj_water_of(const DcPlayer *p);
void obj_water_set_player(int k, u16 level, int on, int flag2);
DcPlayer *obj_box_player(const ObjBox *b);                              /* owner if it is a player */
void obj_player_install_hooks(void);
void obj_player_collide(ObjPlayerCtx *pc);                              /* Player_Collide + sprite timing */
void obj_player_on_hurt(ObjBox *body, ObjBox *attacker);                /* Player_OnHurt (coll2A0 callback) */
void obj_player_add_tension_combo(DcPlayer *p, s16 v);
void obj_player_attack_bounce(DcPlayer *p);                             /* Player_Action_AttackBounce */
void obj_player_attack_recoil(DcPlayer *p);                             /* Player_Action_AttackRecoil */
void obj_player_dash_ring(DcPlayer *p, s32 x, s32 y, s16 vx, s16 vy);   /* Player_Action_DashRing */
void obj_player_start_launch_trick(DcPlayer *p);                        /* Player_StartLaunchTrick */
void obj_player_set_source(DcPlayer *p, Obj *o);                        /* Player_SetSourceMapObject */

/* ------------------------------------------------------- obj_ring.c */
void ring_init(void);                                                   /* RingManager_Create */
void ring_destroy(void);
void obj_init_state(DcPlayer *p); /* Player_InitState: dcp_player_init_state + objects_player_init_state */                                                /* RingManager_Destructor */
void ring_place(u8 *flag, s32 x, s32 y);                                /* RingManager_PlaceRing */
void ring_spawn(s32 x, s32 y, s16 vx, s16 vy, u16 flags);               /* RingManager_SpawnRing */
void ring_main(void);                                                   /* RingManager_Main (+ RingEffect2D) */
int ring_count(void);
u8 *ring_scatter_level(void);
void ring_mark_scatter(void);
int ring_draw_count(void);
int ring_draw_get(int i, s32 *x, s32 *y, int *vflip);
const ObjSprite *ring_draw_sprite(void);
int ring_fx_get(int i, const ObjSprite **s);
int ring_fx_max(void);

/* ------------------------------------------------------ object kinds */
Obj *spring_create(ObjParam *p, s32 x, s32 y);
Obj *dashp_create(ObjParam *p, s32 x, s32 y);
Obj *dashc_create_by_type(ObjParam *p, s32 x, s32 y);
Obj *itembox_create(ObjParam *p, s32 x, s32 y);
Obj *needle_create(ObjParam *p, s32 x, s32 y);
Obj *goalring_create(ObjParam *p, s32 x, s32 y);
Obj *marker_create(ObjParam *p, s32 x, s32 y);
Obj *pawn_create(ObjParam *p, s32 x, s32 y);
Obj *mapobj40_create(ObjParam *p, s32 x, s32 y);
/* objdata v2 kinds (obj_leafstorm.c, obj_shared.c, obj_flapper.c, obj_decor.c) */
Obj *Trampoline_Create(ObjParam *p, s32 x, s32 y);
Obj *BendStick_Create(ObjParam *p, s32 x, s32 y);
Obj *CorkscrewTrigger_Create(ObjParam *p, s32 x, s32 y);
Obj *RockBreak_Create(ObjParam *p, s32 x, s32 y);
Obj *RopeC_Create(ObjParam *p, s32 x, s32 y);
Obj *RopeCLand_Create(ObjParam *p, s32 x, s32 y);
Obj *RopeCSpring_Create(ObjParam *p, s32 x, s32 y);
Obj *EggPawnM_Create(ObjParam *p, s32 x, s32 y);
Obj *Rope_Create(ObjParam *p, s32 x, s32 y);
Obj *JumpPedestal_Create(ObjParam *p, s32 x, s32 y);
Obj *PlayerStart_Create(ObjParam *p, s32 x, s32 y);
Obj *WallBreak_Create(ObjParam *p, s32 x, s32 y);
Obj *IronBall_Create(ObjParam *p, s32 x, s32 y);
Obj *GmkSpecial_Create(ObjParam *p, s32 x, s32 y);
Obj *GmkLand_Create(ObjParam *p, s32 x, s32 y);
Obj *MapObj73_Create(ObjParam *p, s32 x, s32 y);
Obj *MapObj75_Create(ObjParam *p, s32 x, s32 y);
Obj *EnemyFlapperGroup_Create(ObjParam *p, s32 x, s32 y);
Obj *GmkLandAA_Create(ObjParam *p, s32 x, s32 y);
Obj *WaterLevelTrigger_Create(ObjParam *p, s32 x, s32 y);
Obj *MapObj70_Create(ObjParam *p, s32 x, s32 y);
Obj *EffZakoBombPuff_Create(Obj *arg, s16 x, s16 y);
Obj *EffFound_Create(Obj *arg, s16 x, s16 y);
Obj *EffBrakeSmoke_Create(Obj *arg, s16 x, s16 y);
/* decorations (obj_decor.c) */
typedef Obj *(*ObjDecCreateFn)(ObjParam *p, s32 x, s32 y);
ObjDecCreateFn obj_dec_create_fn(u16 type);                              /* gDecorationCreateTable */
Obj *Decoration_Spawn(s32 type, s32 x, s32 y);
void dec_init(void);
void spawn_decorations_in_block(u16 x, u16 y, const void *rc, const void *o, u32 f); /* StageEvent_SpawnDecorationsInBlock */
/* lazy resources */
int obj_res_ensure(int bac);                                             /* load a BAC slot if needed */
const u8 *obj_df(const char *name, u32 *size);                           /* ObjData_Load of a .df */
/* fixed-point helpers (obj_fx.c): NitroSDK MTX_* / FX_*, ObjShift_* */
typedef struct { s32 m[3][3]; } ObjMtx33;
typedef struct { s32 x, y, z; } ObjVec;
void MTX_Identity33_(ObjMtx33 *m);
void MTX_RotX33_(ObjMtx33 *m, s32 sn, s32 cs);
void MTX_RotY33_(ObjMtx33 *m, s32 sn, s32 cs);
void MTX_RotZ33_(ObjMtx33 *m, s32 sn, s32 cs);
void MTX_Concat33(const ObjMtx33 *a, const ObjMtx33 *b, ObjMtx33 *ab);
void MTX_MultVec33(const ObjVec *v, const ObjMtx33 *m, ObjVec *dst);
s32 ObjShift_Set(s32 cur, s32 target, s32 shift, s32 max_step, s32 min_step);
s32 ObjShift_SetFromRef(s32 cur, s32 target, s32 ref, s32 shift, u16 max_step, s32 min_step);
s32 FX_DivS32(s32 n, s32 d);
s16 obj_sin(u16 a);                                                      /* data_0206e470 sine */
s16 obj_cos(u16 a);
/* player gimmick states (obj_pgimmick.c) */
void Player_Action_Rope(DcPlayer *w, Obj *parent, s32 tx, s32 ty);
void Player_Action_RopeC(DcPlayer *w, Obj *parent, s32 len);
void Player_Action_BendStick(DcPlayer *w, s32 a, s32 b);
void Player_Action_CorkscrewPath(DcPlayer *w, s32 x, s32 y, s32 flags);
void Player_Action_GmkSpecial(DcPlayer *w, Obj *o);
void Player_Action_PedestalLaunch(DcPlayer *w, s16 x, s16 y);
void Player_Action_Grind(DcPlayer *w);
dcp_bool Player_CheckStartGrind(DcPlayer *w);
void Player_FindHomingTarget(DcPlayer *w);
void Player_GiveMagnet(DcPlayer *w);
void Player_Action_GimmickLaunch(DcPlayer *w, s16 vx, s16 vy);
void obj_player_effect(DcPlayer *p, s32 fx, s32 a, s32 b);              /* obj_peffect.c: Eff* for DCP_FX_* */
void obj_player_effect_hook(DcPlayer *p, s32 fx, s32 a, s32 b);           /* AttackHitbox_Create (boost) */
void RingManager_AttractRings(DcPlayer *src);
void *rect_regist_get(u32 mask, s16 idx);                                /* ObjRect_RegistGet */
dcp_bool obj_is_attack_hitbox(const DcPlayer *h);
/* camera mirror helpers (obj_shared.c) */
u8 Camera_CheckPosInView(s32 x, s32 y);
void Camera_AddAreaObject(void);
void Camera_RemoveAreaObject(void);
void obj_screen_shake(s32 s);                                            /* ScreenShake_Start */
/* effects (obj_effects.c) */
void mapobject_on_hit_default(ObjBox *a, ObjBox *b);                     /* MapObject_OnHit_Default */
void mapobject_knock_away(Obj *o, DcPlayer *p);                         /* MapObject_Action_KnockAway */
void mapobject_spawn_destroy_burst(Obj *o);                             /* MapObject_SpawnDestroyBurst */
void mapobject_spawn_freed_animal(Obj *o);                              /* MapObject_SpawnFreedAnimal */
Obj *eff_zako_bomb_burst(Obj *src, s16 x, s16 y);                       /* EffZakoBombBurst_Create */
Obj *eff_zako_debris(Obj *src, s16 x, s16 y, s16 vx, s16 vy, u8 type);  /* EffZakoDebris_Create */
void obj_vs_reset(void);                     /* obj_vs.c */
void eff_set_move_until_anim_end(Obj *o);                               /* Effect_SetMoveUntilAnimEndState */

/* ------------------------------------------- objdata v3 (zones 2 and 3) */
Obj *AttackHitbox_Create(DcPlayer *parent, s16 a, s16 b, s16 c, s16 d, s16 e); /* obj_player.c */
void Decoration_InitFromMap(Obj *w, ObjParam *p, s32 x, s32 y);          /* obj_decor.c */
void Decoration_Destructor(Obj *w);
/* obj_water.c */
Obj *EffZakoBomb_Create(DcPlayer *arg, s16 x, s16 y);
void EffAirBubble_Create(DcPlayer *arg, s16 x, s16 y, s32 type, s32 t);
void EffAirBubble_Spawn(DcPlayer *a, s32 x, s32 y, s32 c);
void EffDrownCount_Create(DcPlayer *arg, s32 x, s32 y, s32 idx);
void EffDrownCount_Spawn(DcPlayer *a, s32 b, s32 c, s32 d);
void EffGmkBubble_Create(DcPlayer *arg, s32 x, s32 y, s32 dx, s32 unused, s32 t);
void EffWaterGmk_Create(DcPlayer *arg, s16 x, s16 y);
Obj *EffWaterGmk_CreateSplash(DcPlayer *arg, s32 x, s32 y);
void EffWaterRunSplash_Create(DcPlayer *arg, s32 x, s32 y, s32 base);
void EffWaterSplash_Create(DcPlayer *arg, s32 x, s32 y, s32 base);
void EffWaterSplash_Spawn(DcPlayer *p);
void dec_shared_reset(void);
Obj *dec_shared_create(int bac, s32 anim, u16 slot);                     /* DecShared_Create */
void dec_shared_ping(Obj *s);                                            /* DecShared_State_PingParent */
Obj *WaterRunTrigger_Create(ObjParam *p, s32 x, s32 y);
Obj *MapObj7D_Create(ObjParam *p, s32 x, s32 y);
Obj *MapObj80_Create(ObjParam *p, s32 x, s32 y);
Obj *LaunchZone_Create(ObjParam *p, s32 x, s32 y);
Obj *LandUnrest_Create(ObjParam *p, s32 x, s32 y);
Obj *Wheel2D_Create(ObjParam *p, s32 x, s32 y);
Obj *Mine_Create(ObjParam *p, s32 x, s32 y);
Obj *Bubble_Create(ObjParam *p, s32 x, s32 y);
Obj *BubbleSpawner_Create(ObjParam *p, s32 x, s32 y);
Obj *WaterJet_Create(ObjParam *p, s32 x, s32 y);
Obj *Kuragen_Create(ObjParam *p, s32 x, s32 y);
Obj *KuragenBomb_Create(ObjParam *p, s32 x, s32 y);
Obj *DecGroup41_Create(ObjParam *p, s32 x, s32 y);
Obj *DecGroup4D_Create(ObjParam *p, s32 x, s32 y);
/* obj_pwater.c (player gimmick states) */
void Player_ApplyAutoRunAcceleration(DcPlayer *w);
void Player_Action_WaterRun(DcPlayer *w);
void Player_Action_ZoneLaunch(DcPlayer *w, s16 x, s16 y);
void Player_Action_BreatheAirBubble(DcPlayer *w);
void Player_Action_Wheel2D(DcPlayer *w, Obj *o);
void Player_Action_MapObj80(DcPlayer *w, Obj *o);
void Player_Action_MapObj40(DcPlayer *w);
void Player_Action_AttachToObject(DcPlayer *w, Obj *o, s32 x, s32 y);
void Player_State_WaterRun(DcPlayer *w);
void Player_State_ZoneLaunch(DcPlayer *w);
void Player_State_BreatheAirBubble(DcPlayer *w);
void Player_State_Wheel2D(DcPlayer *w);
void Player_State_MapObj80(DcPlayer *w);
void Player_State_MapObj40(DcPlayer *w);
void Player_State_AttachedToObject(DcPlayer *w);
void Player_UpdateUnderwater(DcPlayer *w);
void obj_player_check_gimmick(DcPlayer *w);
/* obj_mirage.c */
void EffSand_Create(DcPlayer *arg, s16 x, s16 y);
void EffSandSlide_Create(DcPlayer *arg, s16 x, s16 y);
void EffShadow_Create(DcPlayer *arg);
void EffEntrance_Create(Obj *par, s32 dx, s32 dy);
void mirage_reset(void);
int obj_pawn_in_entrance(const Obj *o);                                  /* obj_enemy.c */
void objects_spawn_rect(s32 l, s32 t, s32 r, s32 b);                     /* StageEvent_SpawnInRect (obj_manager.c) */
Obj *Cargo_Create(ObjParam *p, s32 x, s32 y);
Obj *Hand2d_Create(ObjParam *p, s32 x, s32 y);
Obj *GmkTable_Create(ObjParam *p, s32 x, s32 y);
Obj *MapObj8C_CreateByType(ObjParam *p, s32 x, s32 y);
Obj *SandSlider_Create(ObjParam *p, s32 x, s32 y);
Obj *Rock_Create(ObjParam *p, s32 x, s32 y);
Obj *GmkRail_Create(ObjParam *p, s32 x, s32 y);
Obj *EggBishop_Create(ObjParam *p, s32 x, s32 y);
Obj *DecGroup45_Create(ObjParam *p, s32 x, s32 y);
/* obj_pmirage.c (player gimmick states of zone 3) */
void Player_Action_Cargo(DcPlayer *w, Obj *o, s32 x, s32 y);
void Player_Action_Hand2D(DcPlayer *w, Obj *o, s32 x, s32 y);
void Player_Action_SandSlide(DcPlayer *w);
void Player_Action_SandSlider(DcPlayer *w, Obj *o);
void Player_Action_SandSlider90(DcPlayer *w, Obj *o);
void Player_AttachGimmickTable(DcPlayer *w, Obj *o);
void Player_AttachGimmickTableNoInput(DcPlayer *w, Obj *o);
void Player_AttachGimmickTableReleased(DcPlayer *w, Obj *o);
void Player_DetachGimmick(DcPlayer *w, Obj *o);
void Player_State_Cargo(DcPlayer *w);
void Player_State_Hand2D(DcPlayer *w);
void Player_State_SandSlide(DcPlayer *w);
void Player_State_SandSlider(DcPlayer *w);
void Player_State_SandSlider90(DcPlayer *w);

/* ------------------------------------------- objdata v4 (zones 4..7) */
s32 obj_spd_accel_toward(DcPlayer *o, s32 v, s32 accel, s32 decel, s16 max); /* ObjSpd_AccelToward */
/* obj_carnival.c */
void carnival_reset(void);
s16 *obj_switch_timers(void);                                             /* gSwitchTimers */
Obj *CraneSlide_Create(ObjParam *p, s32 x, s32 y);
Obj *NeonRoute_Create(ObjParam *p, s32 x, s32 y);
Obj *Shooter_Create(ObjParam *p, s32 x, s32 y);
Obj *SwingSky_Create(ObjParam *p, s32 x, s32 y);
Obj *Switch_Create(ObjParam *p, s32 x, s32 y);
Obj *DecGroupAF_Create(ObjParam *p, s32 x, s32 y);
/* obj_pcarnival.c */
void Player_Action_GrabCraneSlide(DcPlayer *w, Obj *p, s32 a, s32 b);
void Player_Action_GrabCranePart(DcPlayer *w, Obj *p, s32 a, s32 b);
void Player_Action_SwingSky(DcPlayer *w, Obj *other, s32 flip);
void Player_State_SwingSky(DcPlayer *w);
void FixInformation_CreateSwingSky(DcPlayer *arg);
void FixInformation_CreateSwing(DcPlayer *arg);
/* obj_crisis.c */
void crisis_reset(void);
Obj *Crane_Create(ObjParam *p, s32 x, s32 y);
Obj *CranePartED_Create(ObjParam *p, s32 x, s32 y);
Obj *CranePartEE_Create(ObjParam *p, s32 x, s32 y);
Obj *GmkPipe_Create(ObjParam *p, s32 x, s32 y);
Obj *JetCatapult_Create(ObjParam *p, s32 x, s32 y);
Obj *Piston_Create(ObjParam *p, s32 x, s32 y);
Obj *Steam_Create(ObjParam *p, s32 x, s32 y);
Obj *Valve_Create(ObjParam *p, s32 x, s32 y);
Obj *DecGroup67_Create(ObjParam *p, s32 x, s32 y);
int obj_jet_catapult_flying(const Obj *o);
/* obj_pcrisis.c */
void Player_Action_Pipe(DcPlayer *w, Obj *o, u8 dir);
void Player_State_Pipe(DcPlayer *w);
void Player_Action_PipeExit(DcPlayer *w, s16 spd);
void Player_Action_JetCatapult(DcPlayer *w, Obj *o);
void Player_State_JetCatapult(DcPlayer *w);
void Player_Action_JetCatapultLaunch(DcPlayer *w);
void Player_State_JetCatapultLaunch(DcPlayer *w);
void AttackHitbox_SetJetCatapultState(Obj *s);                           /* obj_player.c */
/* obj_altitude.c */
void altitude_reset(void);
void itembox_reset(void);   /* obj_gimmicks.c: the item box popup pointer */
Obj *Glider_Create(ObjParam *p, s32 x, s32 y);
Obj *NeedleG_Create(ObjParam *p, s32 x, s32 y);
Obj *Parachute_Create(ObjParam *p, s32 x, s32 y);
Obj *Saucer_Create(ObjParam *p, s32 x, s32 y);
Obj *MapObj13_Create(ObjParam *p, s32 x, s32 y);
Obj *DecCloudSpawner_Create(ObjParam *p, s32 x, s32 y);
Obj *DecGroup95_Create(ObjParam *p, s32 x, s32 y);
void ParachuteReleased_Create(DcPlayer *arg);
/* obj_paltitude.c */
void Player_Action_Glider(DcPlayer *w, Obj *o, s32 a, s32 b);
void Player_State_Glider(DcPlayer *w);
void Player_Action_ResumeGlider(DcPlayer *w);
void Player_ClampToGliderArea(DcPlayer *w);
void Player_Action_GliderSequenceStart(DcPlayer *w, Obj *o);
void Player_Action_GliderSequenceEnd(DcPlayer *w, Obj *o);
void Player_State_GliderSequenceIntro(DcPlayer *w);
void Player_State_GliderSequence(DcPlayer *w);
void Player_State_GliderSequenceExit(DcPlayer *w);
void Player_UpdateGliderSequence(DcPlayer *w);
void Player_OnHit_GliderSequence(ObjBox *a, ObjBox *b);
void Player_Action_Parachute(DcPlayer *w, Obj *o, s32 v);
void Player_State_Parachute(DcPlayer *w);
void Player_Action_ParachuteRelease(DcPlayer *w);
void Player_State_ParachuteRelease(DcPlayer *w);
void Player_Action_Saucer(DcPlayer *w, Obj *o);
void Player_State_Saucer(DcPlayer *w);
/* obj_pdeadline.c */
void Player_Action_Swing(DcPlayer *w, Obj *o, s32 unused, s32 r);
void Player_State_Swing(DcPlayer *w);
void Player_SwingAroundPivot(DcPlayer *w);
void Player_SwingAroundPivotRetract(DcPlayer *w);
void Player_Action_Cannon(DcPlayer *w, Obj *o);
void Player_State_Cannon(DcPlayer *w);
void Player_Action_Rocket(DcPlayer *w, Obj *o, s32 x, s32 y);
void Player_State_Rocket(DcPlayer *w);
void Player_Action_Air(DcPlayer *w, Obj *o, s32 x0, s32 y, s32 x1);
void Player_State_Air(DcPlayer *w);
void Player_Unk_ov000_02350adc(DcPlayer *w, Obj *o);
/* obj_deadline.c */
Obj *Cannon_Create(ObjParam *p, s32 x, s32 y);
Obj *Duct_Create(ObjParam *p, s32 x, s32 y);
Obj *GmkAir_Create(ObjParam *p, s32 x, s32 y);
Obj *MapObj9C_Create(ObjParam *p, s32 x, s32 y);
Obj *Rocket_Create(ObjParam *p, s32 x, s32 y);
Obj *Swing_Create(ObjParam *p, s32 x, s32 y);
Obj *DecArrow_Create(ObjParam *p, s32 x, s32 y);
/* obj_flapper.c / obj_enemy2.c / obj_leafstorm.c (enemies of zones 4..7) */
Obj *EggFlapperLaser_Create(ObjParam *p, s32 x, s32 y);
Obj *FlapperCannon_Create(ObjParam *p, s32 x, s32 y);
Obj *EggBombHawk_Create(ObjParam *p, s32 x, s32 y);
Obj *EggSf_Create(ObjParam *p, s32 x, s32 y);
Obj *GunHunter_Create(ObjParam *p, s32 x, s32 y);
void Enemy_Action_StartPopOnContact(Obj *s);
/* obj_rect.c / obj_fx.c / obj_ring.c additions */
dcp_bool rect_point_check(const ObjBox *b, s32 px, s32 py);              /* ObjRect_PointCheck */
u32 FX_Sqrt(s32 v);
u16 ObjAngle_MoveToward16(u16 cur, u16 tgt, u16 step);
dcp_bool RingManager_SuckScatteredRings(const DcPlayer *w);

/* kinds (Obj.kind), for draw / debug */
enum { OBJK_NONE, OBJK_SPRING, OBJK_DASHP, OBJK_DASHC, OBJK_ITEMBOX, OBJK_ITEMPART, OBJK_NEEDLE,
       OBJK_GOAL, OBJK_GOALPART, OBJK_MARKER, OBJK_PAWN, OBJK_EFFECT, OBJK_TRIGGER,
       OBJK_GIMMICK, OBJK_ENEMY, OBJK_DECOR, OBJK_ATTACK, OBJK_CAMERA };

#endif

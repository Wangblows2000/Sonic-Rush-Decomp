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
    u8 needle_shape[12][8];            /* data_ov000_023661c0 */
    u8 animal[8][3];                   /* data_ov000_023661a8 */
    s8 shake[16];                      /* data_02077944 */
    char ids[OBJ_BAC_COUNT][48];       /* resolved sprite ids of this stage */
} ObjData;

typedef struct ObjPlayerCtx {          /* per player: the player's ObjRects + sprite timing */
    DcPlayer *p;
    ObjBox body;                       /* @0x2A0 coll2A0 (hurt box, Player_OnHurt) */
    ObjBox atk;                        /* @0x2CC coll2CC (attack box)             */
    ObjSprite spr;                     /* @0x94 son.bac timing (hit box events)   */
    s16 prev_invuln, prev_invinc;
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
} ObjGlobals;
extern ObjGlobals g_obj;

/* --------------------------------------------- obj_res.c / obj_anim.c */
int obj_res_load(ObjRes *r, const char *spr_dir, const char *obj_dir, const char *id);
void obj_res_free(ObjRes *r);
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
void obj_log(const char *ev, s32 a, s32 b);
void obj_disable_colliders_set_anim(Obj *o, u16 anim);                  /* Enemy_DisableCollidersSetAnim */
typedef Obj *(*ObjCreateFn)(ObjParam *p, s32 x, s32 y);
ObjCreateFn obj_create_fn(u16 type);                                    /* gMapObjectCreateTable */

/* player helpers (obj_player.c) */
ObjPlayerCtx *obj_player_ctx(const DcPlayer *p);
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
/* effects (obj_effects.c) */
void mapobject_on_hit_default(ObjBox *a, ObjBox *b);                     /* MapObject_OnHit_Default */
void mapobject_knock_away(Obj *o, DcPlayer *p);                         /* MapObject_Action_KnockAway */
void mapobject_spawn_destroy_burst(Obj *o);                             /* MapObject_SpawnDestroyBurst */
void mapobject_spawn_freed_animal(Obj *o);                              /* MapObject_SpawnFreedAnimal */
Obj *eff_zako_bomb_burst(Obj *src, s16 x, s16 y);                       /* EffZakoBombBurst_Create */
Obj *eff_zako_debris(Obj *src, s16 x, s16 y, s16 vx, s16 vy, u8 type);  /* EffZakoDebris_Create */
void eff_set_move_until_anim_end(Obj *o);                               /* Effect_SetMoveUntilAnimEndState */

/* kinds (Obj.kind), for draw / debug */
enum { OBJK_NONE, OBJK_SPRING, OBJK_DASHP, OBJK_DASHC, OBJK_ITEMBOX, OBJK_ITEMPART, OBJK_NEEDLE,
       OBJK_GOAL, OBJK_GOALPART, OBJK_MARKER, OBJK_PAWN, OBJK_EFFECT, OBJK_TRIGGER };

#endif

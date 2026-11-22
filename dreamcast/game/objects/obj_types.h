/*
 * Native stage object system (Dreamcast port of Sonic Rush) - internal types.
 *
 * Faithful translation of src/game/Stage/ObjectSystem/ (StageObj_*, MapObject_*,
 * ObjRect_*, ObjCollisionMap_*), src/game/Stage/StageEvent_*, src/game/Objects/
 * and src/game/Enemies/EnemyPawnGroup_*.  Field comments give the original
 * work-struct offset ("@0x5C") so each translated line can be compared with the
 * decompilation.  See docs/dreamcast/translation/OBJECTS.md.
 *
 * The StageObj header (position, velocity, flags, map hitbox, ride/touch) of
 * every object is a DcPlayer (dreamcast/game/player): the player module's
 * DcPlayer *is* the translated StageObj header, so StageObj_Move /
 * StageObj_Collision / StageObj_HandleRide are reused (dcp_obj_move,
 * dcp_obj_collision, dcp_handle_ride) instead of being translated twice.
 */
#ifndef OBJ_TYPES_H
#define OBJ_TYPES_H

#include "../player/dcp_player.h"

/* ------------------------------------------------------------ limits */
#define OBJ_MAX 384            /* object pool (DS: task heap); decorations
                                  share it (up to ~170 alive on z12)       */
#define OBJ_DEC_SLOTS 32       /* gDecorationSlots / gDecorationSlotMask    */
#define OBJ_SPAWN_ENTRIES 0x28 /* gMapObjectSpawnEntries                    */
#define OBJ_RECT_PER_LIST 0x30 /* gObjRectNextList[4][0x30]                 */
#define OBJ_COLMAP_MAX 0x20    /* data_022c4de4[0x20]                       */
#define OBJ_RING_MAX 64        /* RingManager parts[64]                     */
#define OBJ_MAX_VIEWS 2        /* gCamera[0], gCamera[1]                    */

/* task ids (Task_Create id argument = update order) */
#define OBJ_PRIO_STAGE 0x0010   /* Stage_Main (ObjRect_CheckAllGroup)       */
#define OBJ_PRIO_EVENT 0x0090   /* StageEvent manager                       */
#define OBJ_PRIO_PLAYER 0x0100  /* Player                                   */
#define OBJ_PRIO_MAPOBJ 0x0500  /* map objects / enemies                    */
#define OBJ_PRIO_EFFECT 0x0a00  /* effects, item box parts, goal ring parts  */
#define OBJ_PRIO_RING 0x3000    /* RingManager, RingEffect2D                */

/* ------------------------------------------------------- BAC slots */
/* order == SLOTS in tools/objdata.py */
enum ObjBac {
    OBJ_BAC_RING = 0,  /* ac_itm_ring.bac (act_com)        */
    OBJ_BAC_SPRING,    /* act/ac_gmk_spring.bac            */
    OBJ_BAC_DASH_P,    /* act/ac_gmk_dash_p.bac            */
    OBJ_BAC_DASH_C,    /* act/ac_gmk_dash_c.bac            */
    OBJ_BAC_DASH_CT,   /* act/ac_gmk_dash_ct.bac           */
    OBJ_BAC_ITEMBOX,   /* act/ac_itm_box.bac               */
    OBJ_BAC_NEEDLE,    /* act/ac_gmk_needle.bac            */
    OBJ_BAC_GOALRING,  /* act/ac_gmk_goalring.bac          */
    OBJ_BAC_MARKER,    /* act/ac_gmk_marker.bac            */
    OBJ_BAC_EGGPAWN,   /* act/ac_ene_eggpawn.bac           */
    OBJ_BAC_EGGPAWN_M, /* act/ac_ene_eggpawn_m.bac         */
    OBJ_BAC_ZAKO_BOMB, /* act/ac_eff_zako_bomb.bac (act_com) */
    OBJ_BAC_HAHEN,     /* act/ac_eff_hahen_zako.bac (act_com) */
    OBJ_BAC_PLAYER,    /* act/son.bac (player_lz7): hit boxes only */
    /* objdata v2: Leaf Storm + shared objects, decorations, effects */
    OBJ_BAC_TRAMPOLINE,   /* ac_gmk_trampoline.bac   */
    OBJ_BAC_BEND_STICK,   /* ac_gmk_bend_stick.bac   */
    OBJ_BAC_ROCK_BREAK,   /* ac_gmk_rock_break.bac   */
    OBJ_BAC_ROPE,         /* ac_gmk_rope.bac         */
    OBJ_BAC_ROPE_C_LAND,  /* ac_gmk_rope_c_land.bac  */
    OBJ_BAC_ROPE_C_SPRING,/* ac_gmk_rope_c_spring.bac */
    OBJ_BAC_ROPE_C,       /* ac_gmk_rope_c.bac       */
    OBJ_BAC_JUMP_PEDESTAL,/* ac_gmk_jump_pedestal.bac */
    OBJ_BAC_FLOOR_BREAK,  /* ac_gmk_floor_braek01.bac */
    OBJ_BAC_LAND,         /* ac_gmk_land.bac         */
    OBJ_BAC_WALL_BREAK,   /* ac_gmk_wall_braek01.bac */
    OBJ_BAC_IRON_BALL,    /* ac_gmk_iron_ball.bac    */
    OBJ_BAC_SPECIAL,      /* ac_gmk_special.bac      */
    OBJ_BAC_CARGO,        /* ac_gmk_cargo.bac (GmkSpecial second sprite, as the ROM names it) */
    OBJ_BAC_FLAPPER,      /* ac_ene_flapper.bac      */
    OBJ_BAC_EGG_DIVER,    /* ac_ene_egg_diver.bac    */
    OBJ_BAC_DEC_FLOWER,   /* ac_dec_flower.bac       */
    OBJ_BAC_DEC_VINE,     /* ac_dec_vine.bac         */
    OBJ_BAC_DEC_BIRD_NEST,/* ac_dec_bird_nest.bac    */
    OBJ_BAC_DEC_BIRD,     /* ac_dec_bird.bac         */
    OBJ_BAC_DEC_LEAF,     /* ac_dec_leaf.bac         */
    OBJ_BAC_EFF_FOUND,    /* ac_eff_found.bac (act_com) */
    OBJ_BAC_EFF_BRAKE,    /* ac_eff_kemuri_brake.bac (act_com) */
    OBJ_BAC_ANIMAL01,     /* ac_dec_animal%02d.bac, 01..16 (DecAnimal) */
    OBJ_BAC_ANIMAL16 = OBJ_BAC_ANIMAL01 + 15,
    /* objdata v3: Water Palace (zone 2) */
    OBJ_BAC_LAND_UNREST,  /* ac_gmk_land_unrest.bac  */
    OBJ_BAC_WHEEL2D,      /* ac_gmk_wheel2d.bac      */
    OBJ_BAC_MINE,         /* ac_gmk_mine.bac         */
    OBJ_BAC_BUBBLE,       /* ac_gmk_bubble.bac       */
    OBJ_BAC_WATER_JET,    /* ac_gmk_water_jet.bac    */
    OBJ_BAC_KURAGEN_BOMB, /* ac_ene_kuragen_bomb.bac */
    OBJ_BAC_KURAGEN_BLITZ,/* ac_ene_kuragen_blitz.bac */
    OBJ_BAC_EGG_LANCER,   /* ac_ene_egg_lancer.bac   */
    OBJ_BAC_EFF_WATER,    /* ac_eff_water.bac        */
    OBJ_BAC_EFF_WATER_GMK,/* ac_eff_water_gmk.bac    */
    OBJ_BAC_FIX_MOJI,     /* ac_fix_moji.bac (act_com, MineDigit: HUD digit glyphs) */
    OBJ_BAC_DEC_FOUNTAIN, /* ac_dec_fountain.bac     */
    OBJ_BAC_WATER_FALL,   /* ac_gmk_water_fall.bac   */
    OBJ_BAC_GMK_WHEEL,    /* ac_gmk_wheel.bac        */
    OBJ_BAC_DEC_ALGAE,    /* ac_dec_algae.bac        */
    OBJ_BAC_DEC_CORAL,    /* ac_dec_coral.bac        */
    OBJ_BAC_DEC_FISH,     /* ac_dec_fish.bac         */
    /* objdata v3: Mirage Road (zone 3) */
    OBJ_BAC_EGG_BISHOP, /* ac_ene_egg_bishop.bac */
    OBJ_BAC_EGG_HAMMER, /* ac_ene_egg_hammer.bac */
    OBJ_BAC_HAND2D, /* ac_gmk_hand2d.bac */
    OBJ_BAC_ROCK, /* ac_gmk_rock.bac */
    OBJ_BAC_SAND_SLIDER, /* ac_gmk_sand_slider.bac */
    OBJ_BAC_EFF_SAND, /* ac_eff_sand.bac */
    OBJ_BAC_EFF_SHADOW, /* ac_eff_shadow.bac */
    OBJ_BAC_GMK_RAIL, /* ac_gmk_rail.bac */
    OBJ_BAC_GMK_WALL, /* ac_gmk_wall.bac (GmkWall, enemy room gate) */
    OBJ_BAC_GMK_HAND, /* ac_gmk_hand.bac (DecGroup45 0x64..0x66) */
    OBJ_BAC_DEC_SAND, /* ac_dec_sand.bac */
    OBJ_BAC_DEC_DECORA, /* ac_dec_decora.bac */
    OBJ_BAC_DEC_RELIEF, /* ac_dec_relief.bac */
    OBJ_BAC_DEC_STATUE, /* ac_dec_statue.bac */
    OBJ_BAC_DEC_TORCH, /* ac_dec_torch.bac */
    OBJ_BAC_DEC_CANOPS, /* ac_dec_canops.bac */
    OBJ_BAC_DEC_BED, /* ac_dec_bed.bac */
    OBJ_BAC_DEC_BOX, /* ac_dec_box.bac */
    OBJ_BAC_DEC_TABLE_WATER, /* ac_dec_table_water.bac */
    OBJ_BAC_EFF_ENTRANCE, /* ac_eff_entrance.bac (zone archive, EnemyPawnGroup entrance) */
    OBJ_BAC_GMK_RAIL6, /* ac_gmk_rail6.bac (GmkRail when gGameState.unk1C == 5) */
    OBJ_BAC_CRANE_SLIDE, /* ac_gmk_crane_slide.bac */
    OBJ_BAC_NEON_ROUTE, /* ac_gmk_neon_route.bac */
    OBJ_BAC_SHOOTER, /* ac_gmk_shooter.bac */
    OBJ_BAC_SWING_SKY, /* ac_gmk_swing_sky.bac */
    OBJ_BAC_SWITCH, /* ac_gmk_switch.bac */
    OBJ_BAC_FIX_INFORMATION, /* ac_fix_information.bac */
    OBJ_BAC_DEC_SONIC_NEON, /* ac_dec_sonic_neon.bac */
    OBJ_BAC_DEC_NEON_FLOOR, /* ac_dec_neon_floor.bac */
    OBJ_BAC_DEC_NEON_LAND, /* ac_dec_neon_land.bac */
    OBJ_BAC_DEC_SWING_SKY_HANG, /* ac_dec_swing_sky_hang.bac */
    OBJ_BAC_DEC_NEON_BALL, /* ac_dec_neon_ball.bac */
    OBJ_BAC_DEC_NEON_PILLAR, /* ac_dec_neon_pillar.bac */
    OBJ_BAC_DEC_LIGHT4, /* ac_dec_light4.bac */
    OBJ_BAC_DEC_NEON_BALL2, /* ac_dec_neon_ball2.bac */
    OBJ_BAC_DEC_NEON_STAR, /* ac_dec_neon_star.bac */
    OBJ_BAC_GLIDER, /* ac_gmk_glider.bac */
    OBJ_BAC_NEEDLE_G, /* ac_gmk_needle_g.bac */
    OBJ_BAC_PARACHUTE, /* ac_gmk_parachute.bac */
    OBJ_BAC_SAUCER, /* ac_gmk_saucer.bac */
    OBJ_BAC_CLOUD_3, /* ac_gmk_cloud_3.bac */
    OBJ_BAC_RING_GRAIDER, /* ac_itm_ring_graider.bac */
    OBJ_BAC_FLAPPER3D, /* ac_ene_flapper3d.bac */
    OBJ_BAC_DEC_CLOUD, /* ac_dec_cloud.bac */
    OBJ_BAC_DEC_CLOUD_W, /* ac_dec_cloud_w.bac */
    OBJ_BAC_DEC_INNER_PILLAR, /* ac_dec_inner_pillar.bac */
    OBJ_BAC_DEC_PROPELLER, /* ac_dec_propeller.bac */
    OBJ_BAC_DEC_JET, /* ac_dec_jet.bac */
    OBJ_BAC_DEC_LIGHT, /* ac_dec_light.bac */
    OBJ_BAC_DEC_LASER, /* ac_dec_laser.bac */
    OBJ_BAC_CRANE, /* ac_gmk_crane.bac */
    OBJ_BAC_PIPE, /* ac_gmk_pipe.bac */
    OBJ_BAC_PISTON, /* ac_gmk_piston.bac */
    OBJ_BAC_VALVE, /* ac_gmk_valve.bac */
    OBJ_BAC_JET_CATAPULT, /* ac_gmk_jet_catapult.bac */
    OBJ_BAC_EFF_JET, /* ac_eff_jet.bac */
    OBJ_BAC_EFF_STEAM, /* ac_eff_steam.bac */
    OBJ_BAC_DEC_PIPE, /* ac_dec_pipe.bac */
    OBJ_BAC_DEC_PLANE, /* ac_dec_plane.bac */
    OBJ_BAC_AIR, /* ac_gmk_air.bac */
    OBJ_BAC_CANNON, /* ac_gmk_cannon.bac */
    OBJ_BAC_DUCT, /* ac_gmk_duct.bac */
    OBJ_BAC_ROCKET, /* ac_gmk_rocket.bac */
    OBJ_BAC_SWING, /* ac_gmk_swing.bac */
    OBJ_BAC_DEC_ARROW, /* ac_dec_arrow.bac */
    OBJ_BAC_EGG_SF, /* ac_ene_egg_sf.bac */
    OBJ_BAC_EGG_SOLID, /* ac_ene_egg_solid.bac */
    OBJ_BAC_GUN_HUNTER, /* ac_ene_gun_hunter.bac */
    OBJ_BAC_RHINO, /* ac_ene_rhino.bac */
    OBJ_BAC_EGG_BUNNY, /* ac_ene_egg_bunny.bac */
    OBJ_BAC_EGG_FLAPPER_LASER, /* ac_ene_egg_flapper_laser.bac */
    OBJ_BAC_BOMB_HAWK, /* ac_ene_bomb_hawk.bac */
    OBJ_BAC_FLAPPER_BAT, /* ac_ene_flapper_bat.bac */
    OBJ_BAC_FLAPPER_CANNON, /* ac_ene_flapper_cannon.bac */
    OBJ_BAC_FALCO, /* ac_ene_falco.bac */
    OBJ_BAC_COUNT
};

/* ------------------------------------------------- sprite resources */
/* DSPR v1 (tools/dreamcast/sprites/bac2dc.py, docs/dreamcast/formats/SPRITES.md) */
typedef struct { u16 w, h; } DsprPage;
typedef struct { u8 page, pad; u16 x, y, w, h; s16 origin_x, origin_y; u16 pad2; } DsprImage;
typedef struct { u16 first_frame, frame_count; s16 loop_frame, next_anim; u8 format, pad; u16 pad2; } DsprAnim;
typedef struct { u16 image, duration; } DsprFrame;
/* HBOX v1 (tools/objdata.py): BAC command 7 per frame */
typedef struct { u16 first_event, count; } HboxFrame;
typedef struct { u8 slot, pad; u16 flags; s16 l, t, r, b; } HboxEvent;

typedef struct ObjRes {
    char id[48];
    int loaded;
    u8 *dspr;                /* whole frames.bin */
    u16 n_pages, n_images, n_anims, n_frames;
    const DsprPage *pages;
    const DsprImage *images;
    const DsprAnim *anims;
    const DsprFrame *frames;
    u8 *hbx;                 /* whole .hbx (may be NULL) */
    u16 hbx_frames;
    const HboxFrame *hframes;
    const HboxEvent *hevents;
    void *tex[4];            /* renderer textures (obj_draw.c), one per page */
} ObjRes;

/* AnimatorSprite (0x48-byte block, src/game/Graphics/Sprite/) */
#define SPR_STOPPED 0x00000001u  /* flags bit0: non-looping animation ended   */
#define SPR_ENDED 0x00000002u    /* bit1: last frame's wait ran out this call */
#define SPR_LOOP 0x00000004u     /* bit2: loop (from obj 5C & 4)              */
#define SPR_HFLIP 0x00000080u
#define SPR_VFLIP 0x00000100u
typedef struct ObjBox ObjBox;
typedef void (*ObjSprCb)(void *owner, const HboxEvent *ev);
typedef struct ObjSprite {
    const ObjRes *res;  /* @0x10 resource                            */
    u16 anim;           /* @0x08 animation index                     */
    u16 frame_no;       /* @0x0A EndFrame frameNo (1-based)          */
    u16 frame_cnt;      /* @0x0C EndFrame frameCount                 */
    s16 frame_t;        /* @0x0E elapsed ticks (x16)                 */
    u16 pos;            /* @0x1C command position -> next frame index */
    u16 cur;            /* frame index shown (absolute in frames[])  */
    s16 wait;           /* @0x28 */
    s16 speed;          /* @0x2A 0x10 = one 60 Hz tick per frame     */
    u32 flags;          /* @0x2C SPR_*, bits 17-18 = OAM priority    */
    u16 order;          /* @0x46 OAM order                           */
    s16 x, y;           /* draw position (world px, or screen px)    */
    u8 valid;           /* a frame has been reached                  */
    u8 screen;          /* x,y are screen coordinates (obj 5C & 0x80) */
    u8 hidden;          /* obj 5C & 0x20 at the last draw step        */
    u8 rot;             /* rotation (obj angle + grav_rot)             */
    u8 rotscale;        /* drawn as rot-scale OAM (obj 5C & 0x100, rot != 0, or scale) */
    s16 sx, sy;         /* rot-scale size (0x1000 = 1.0), from obj @0x44/@0x46 */
    /* StageObj_Draw2DEx with a piece-offset table (StageObj_DrawSpriteAlt):
     * per-piece offsets in pixels, read at draw time (WallBreak, GmkLand) */
    const s32 (*ppos)[2]; /* [16][2] 24.8 offsets (the object's own array)  */
    /* the same frame drawn again at more positions (Rope_Draw) */
    const s16 (*multi)[2];
    u8 n_multi;
    /* StageObj_SetSharedSpriteVram / spr flags 8: the image shown is the
     * current frame of another sprite (DecShared animation driver) */
    const struct ObjSprite *vram;
} ObjSprite;

/* ------------------------------------------------------ ObjRect */
/* 0x2C-byte collision box (ObjRect_*).  flags (@0x0A):
 *   1 mirror x, 2 mirror y, 4 box set, 8 callback as attacker,
 *   0x20 stays in the list after a hit, 0x40 touched this frame,
 *   0x80 in contact, 0x100 was hit, 0x200 has hit, 0x400 hit now,
 *   0x800 call back on contact start only, 0x1000 disabled */
enum { OBJ_OWNER_NONE = 0, OBJ_OWNER_PLAYER, OBJ_OWNER_OBJECT };
struct ObjBox {
    s32 x, y;           /* @0x00 pixels                              */
    s16 z;              /* @0x08 */
    u16 flags;          /* @0x0A */
    s16 l, t, r, b;     /* @0x0C */
    s16 zn, zf;         /* @0x14 */
    void *owner;        /* @0x18 Obj* or DcPlayer*                   */
    void (*cb)(ObjBox *a, ObjBox *b); /* @0x1C defender callback     */
    u32 atk;            /* @0x20 */
    u32 def;            /* @0x24 */
    u8 mask;            /* @0x28 low nibble: list, high nibble: lists checked */
    u8 owner_kind;      /* native: OBJ_OWNER_*                        */
};

/* ------------------------------------------------- ObjCollisionMap */
typedef struct Obj Obj;
typedef struct ObjColMap {  /* @0x1B8 in the StageObj header           */
    Obj *obj;           /* +0x00 */
    DcPlayer *rider;    /* +0x04 actor standing on it                 */
    DcPlayer *toucher;  /* +0x08 actor touching it                    */
    u16 flags;          /* +0x0C bit0: rotate with obj angle          */
    const u8 (*map)[8]; /* +0x10 per tile: 8 x (height<<4 | width)    */
    const u8 *angles;   /* +0x14 per tile angle; see angles_ff          */
    u8 angles_ff;       /* native: angles == (u8*)-1 in the original  */
    const u8 *attr;     /* +0x18 */
    s16 ox, oy;         /* +0x1C offset of the tile grid (px)          */
    u16 w, h;           /* +0x20 size in 8 px tiles                   */
} ObjColMap;

/* --------------------------------------------- map object parameters */
/* Mutable copy of one .ev entry (UnkStruct_ObjParam / UnkStruct_ObjSpawn). */
typedef struct ObjParam {
    u8 x0;              /* +0 region-local x; 0xff = spawned (MapObject_Init) */
    u8 y0;              /* +1 */
    u16 type;           /* +2 gMapObjectCreateTable index */
    u16 flags;          /* +4 */
    s8 p6, p7;          /* +6 +7 */
    u8 p8, p9;          /* +8 +9 */
    u8 pA, pB;          /* +A +B runtime (B: hits taken) */
    u16 ax, ay;         /* native: absolute position (px) */
} ObjParam;

/* --------------------------------------------------------- object */
typedef void (*ObjFn)(Obj *o);
struct Obj {
    DcPlayer h;         /* StageObj header @0x00..@0x8C (see top of file):
                           type@00 angle@01 grav_rot@05 shake@06 x@10 px@1C
                           move_x@28 vel_x@2E vel_y@30 gnd_vel@34 push@36
                           gravity@40 fall_max@42 obj_flags@54 move_flags@58
                           disp_flags@5C hitstop@62 box@64 ride@80 touch@84 */
    s32 w8;             /* @0x08 work */
    s32 wC;             /* @0x0C work */
    s16 scale_x, scale_y; /* @0x44 @0x46 */
    s16 out_ofs;        /* @0x4A ViewOutCheck margin                  */
    s16 out_l, out_t, out_r, out_b; /* @0x4C..@0x52                     */
    ObjFn state;        /* @0x68 */
    ObjFn fn_in;        /* @0x6C */
    ObjFn fn_draw;      /* @0x70 */
    ObjSprCb spr_cb;    /* @0x74 sprite (hit box) callback              */
    ObjFn fn_collide;   /* @0x78 */
    ObjFn fn_last;      /* @0x7C */
    ObjFn dtor;         /* task destructor                             */
    Obj *parent;        /* @0x88 */
    DcPlayer *ppar;     /* @0x88 when the parent is a player (effects that follow the player) */
    ObjSprite spr;      /* @0x94 spr0 (spr1 = second DS screen, not kept) */
    ObjSprite sprA;     /* extra sprite pair of DashC/DashCT (@0x20C)   */
    u8 has_spr, has_sprA;
    ObjBox coll[3];     /* @0x134 @0x160 @0x18C */
    ObjColMap cmap;     /* @0x1B8 */
    u8 has_cmap;        /* @0x1B8 != NULL */
    u32 snd;            /* @0x1DC sound handle (native: 1 = allocated) */
    ObjParam *prm;      /* @0x1E0 */
    u8 saved_x;         /* @0x1E4 */
    u8 vitality;        /* @0x1E5 */
    s32 home_x, home_y; /* @0x1E8 @0x1EC spawn position (px) */
    s16 flash;          /* @0x1F0 */
    u32 f1F4;           /* @0x1F4 */
    u16 anim1F8;        /* @0x1F8 */
    s32 u1FC, u200;     /* @0x1FC @0x200 (objdata v4: EnemyFlapperGroup swoop start) */
    void *target;       /* @0x204 DcPlayer* chased */
    DcpObjRef ref;      /* what the player module sees in ride/touch/homing */
    /* per-kind extra work (original offsets beyond @0x20C) */
    ObjBox detect;      /* EnemyPawnGroup @0x20C */
    s16 u238, u23A, u23C; /* EnemyPawnGroup @0x238 (long attack spins) @0x23A (offset to the ridden object) @0x23C */
    u32 u2A0;           /* DashC/DashCT @0x2A0 (bit0: DashCT) */
    s32 u2A4;           /* DashC/DashCT @0x2A4 direction 0..7 */
    /* work beyond @0x20C of the kinds added in objdata v2 (original offsets) */
    union {
        struct { s32 pos[16][2]; s16 vel[16][2]; } brk;  /* WallBreak / GmkLand @0x20C pos, @0x28C vel */
        struct { u32 f20C[2]; s32 r214[2][4]; void (*pstate[2])(Obj *, s32); s8 pidx[2]; } mo75; /* MapObj75 */
        struct { s32 u210, u214, u218; s8 pidx[2]; u8 n; } mo73;  /* MapObj73 @0x210..@0x21E */
        struct { s8 pidx[2]; u16 value; } cam;           /* WaterLevelTrigger @0x20C/@0x20E, MapObj70 @0x214/@0x212 */
        struct { s16 pts[8][2]; } rope;                  /* Rope_Draw points (native) */
        struct { s32 mtx[9]; s32 scale[3]; u8 shape; } mdl; /* RopeC model @0x128 (matrix, scale, shape) */
        struct { s16 touch; } ani;                       /* DecAnimal @0x198 */
        struct { u8 playing; } snd;                      /* IronBall: looping sound state (native) */
        struct { DcPlayer *par; } atk;                   /* AttackHitbox @0x88 parent (a player) */
        /* objdata v3 kinds (Water Palace / Mirage Road), original offsets */
        struct { u32 u238; s32 u23C; void (*cb)(ObjBox *, ObjBox *); } kur; /* Kuragen @0x238 @0x23C (type 6: saved coll0 callback) */
        struct { u32 f20C; s32 u210; u16 u214; u8 u216; } bub; /* Bubble / BubbleSpawner / WaterJet @0x20C.. */
        struct { ObjSprite s0, s2; s16 u32C; s32 pts[16][2]; s16 cnt[16];
                 s16 mpts[16][2]; u8 n; } mine;          /* Mine @0x20C/@0x29C sprites, @0x32C, @0x330 pts, cnt */
        struct { ObjSprite a, c; s16 u32C, u32E; s16 pts[5][2]; } cargo; /* Cargo @0x20C/@0x29C, @0x32C */
        struct { u32 u364; u16 u41C, u41E; s32 u420; s16 u424, u426, u428, u42A; s32 mtx2[9]; } tbl; /* GmkTable */
        struct { u32 f20C; s8 pidx[2]; s8 need, got; u8 room; u16 b216, b218, b21A, b21C;
                 s32 c220, c224, c228, c22C; s16 u230; } room; /* MapObj8C (enemy room) @0x20C..@0x230 */
        struct { s32 u20C, u210; s32 pos[2][2]; u32 u224; } mo8f; /* MapObj8F (room / camera bounds) */
        /* objdata v4 kinds (zones 4..7), original offsets */
        struct { s16 pts[64][2]; } neon;                 /* NeonRoute_Draw positions (native) */
        struct { ObjSprite s0, s2, s4, s6; s16 chain[20]; u16 u4A0; s16 u4A2; u32 u4A4;
                 s16 p0[20][2], p2[20][2]; } sky;        /* SwingSky @0x20C..@0x404 sprites, @0x478 chain, @0x4A0.. */
        struct { s16 idx; u16 kind; u16 u4, u6; } swt;   /* SwitchTimer work (@0x0 @0x2 @0x4 @0x6) */
        struct { ObjSprite mirror; } afc;                /* DecGroupAFChild_Draw second (mirrored) draw */
        struct { ObjSprite a; s16 pts[48][2]; } crane;   /* CranePartED @0x99C chain sprite / CranePartEE / Piston shaft */
        struct { ObjSprite s; u32 u2A0; s32 u2A4; } valve; /* Valve @0x20C steam sprite, @0x2A4 kind */
        struct { u32 u2A8; } steam;                      /* Steam @0x2A8 */
        struct { ObjSprite a, c; } cannon;               /* Cannon @0x20C sprA, @0x29C sprC (barrel) */
        struct { ObjSprite s0, s2, m0, m2; } swing;      /* Swing @0x20C / @0x29C, + the mirrored draws */
    } k;
    /* extra sprite pairs drawn after spr (Mine, Cargo, ...): point into k */
    ObjSprite *xspr[4];
    u8 n_xspr;
    /* objdata v4: a second collision map entry inside the work (NeonRoute
     * @0x20C, ...), registered by the object itself */
    ObjColMap cmap2;
    u8 has_cmap2;
    /* objdata v4: shapes of the model drawn (StageObj_LoadModelSimple blocks
     * whose @0xA8 & 0x80 is clear); 0 = every shape */
    u32 shapes;
    u8 mreps;           /* the model drawn mreps times (0 = once), each mrep_dy lower (CranePartEE_Draw) */
    s32 mrep_dy;
    /* objdata v4: the 2D sprite stands for a 3D BAC (StageObj_Load3dBAC):
     * drawn at x/y by obj_draw.c as before (deviation 23), listed by
     * objects_sprite3d_* with its depth z for a 3D draw */
    u8 bac3d;
    /* 3D model of the object (StageObj_LoadModelSimple @0x128): converted
     * model id, rotation (4.12, row-major) and scale; see objects_model_get */
    const char *model;
    s32 mtx[9];
    s32 mscale[3];
    u8 is_dec;          /* decoration (Decoration_InitFromMap): prm is a .dc entry */
    /* native bookkeeping */
    u16 prio;           /* task id */
    u8 used;
    u8 kind;
    Obj *next;
};

#endif

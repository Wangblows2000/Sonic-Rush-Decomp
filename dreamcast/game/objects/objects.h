/*
 * Native stage object module (Dreamcast port of Sonic Rush) - public API.
 *
 * Frame order (matches the DS task order, see OBJECTS.md section 2):
 *
 *     objects_frame_begin();            // Stage_Main: ObjRect_CheckAllGroup,
 *                                       // ObjCollisionMap_CommitFrame;
 *                                       // StageEvent_Main: spawn in view
 *     dcp_player_frame(&player, ...);   // player task (id 0x100)
 *     objects_update(&player);          // Player_Collide, map objects (0x500),
 *                                       // effects (0xa00), RingManager (0x3000)
 *     ... camera update, then objects_set_view() for the next frame ...
 *     objects_draw();                   // inside the PT list (KOS build only)
 *
 * Positions are 24.8 fixed point like the player (DcPlayer.x).  The view
 * (DS gCamera[n] x/y) is the top-left of the 256x192 world window.
 */
#ifndef OBJECTS_H
#define OBJECTS_H

#include <stdint.h>
#include "obj_types.h"

/* -------------------------------------------------- side effects */
/* Everything outside the object system goes out through these hooks
 * (all optional).  Each sits where the original call was. */
enum {
    OBJ_EXIT_RESULTS = 0,   /* StageShutdown_Create(ResultsScreen_Create) */
    OBJ_EXIT_CLEAR_DEMO = 1,/* StageShutdown_Create(ClearDemo_Create)     */
    OBJ_EXIT_SPECIAL_STAGE = 2 /* special ring entered (pf |= 0x40000000; Player_Last starts it) */
};
enum {                      /* g_obj_hooks.effect kinds: effects without a module yet */
    OBJ_FX_TRICK_AT = 1,    /* EffTrick_CreateAtOffset(p, a, b)               */
    OBJ_FX_SPECIAL,         /* EffSpecial_Create(p)                           */
    OBJ_FX_GRIND,           /* EffGrind_Create(p, a, b)                       */
    OBJ_FX_TRICK_SPARKLE,   /* EffTrickSparkle_Create(a)                      */
    OBJ_FX_ENTRANCE,        /* EffEntrance_Create(obj, a, b) (p = NULL)       */
    OBJ_FX_WATER_SPLASH     /* EffWaterGmk_CreateSplash (p = the toucher)     */
};
enum {                      /* ItemBox_GiveItem kinds the player module lacks */
    OBJ_ITEM_INVINCIBLE = 5, OBJ_ITEM_BARRIER = 6, OBJ_ITEM_MAGNET = 7,
    OBJ_ITEM_VS_SLOW = 8, OBJ_ITEM_VS_CONFUSE = 9, OBJ_ITEM_VS_DRAIN = 10, OBJ_ITEM_VS_11 = 11
};
typedef struct ObjHooks {
    void *ctx;
    void (*sfx)(void *ctx, s32 id);                       /* Sound_PlaySeqArcEx(.., 0, id) */
    void (*goal)(void *ctx, DcPlayer *p);                 /* Player_Action_ReachGoal (not in the player module) */
    void (*stage_exit)(void *ctx, s32 kind);              /* StageShutdown_Create */
    void (*fade)(void *ctx, s32 kind, s32 speed);         /* BrightnessFade_Create */
    void (*item)(void *ctx, DcPlayer *p, s32 item);       /* Player_GiveInvincibility/Barrier/Magnet, VS items */
    void (*checkpoint_popup)(void *ctx, DcPlayer *p, u32 time); /* CheckpointTimePopup_Create (time attack) */
    void (*log)(void *ctx, const char *event, s32 a, s32 b);    /* native: test/debug trace */
    void (*effect)(void *ctx, DcPlayer *p, s32 kind, s32 a, s32 b); /* OBJ_FX_* */
    void (*shake)(void *ctx, s32 strength);               /* ScreenShake_Start (falls back to g_dcp_hooks.shake) */
} ObjHooks;
extern ObjHooks g_obj_hooks;

/* data_022c4590 / gGameState fields written by the objects (score tally,
 * checkpoint).  The checkpoint position is also copied to
 * g_dcp_world.check_x/check_y (the player module's respawn point). */
typedef struct ObjStage {
    s32 speed_bonus;   /* data_022c4590.unk8  (goal / marker speed bonus sum)   */
    s32 bonus_count;   /* data_022c4590.unkC                                    */
    s32 ring_bonus;    /* data_022c4590.unk10 (rings * 10 at the goal)          */
    u8 check_idx;      /* data_022c4590.unk15 last marker index                 */
    u32 check_time;    /* data_022c4590.unk18 timer at the marker               */
    s32 check_x, check_y; /* data_022c4590.unk1C / unk20 respawn point (px)     */
    s32 goal_state;    /* data_022c4590.unk40 (3 = goal touched again, TA mode) */
    u8 goal_reached;   /* native: goal touched                                  */
    u32 save_flags;    /* gSaveData.unk8 (bit0: easy option)                    */
} ObjStage;
extern ObjStage g_obj_stage;

/* ------------------------------------------------------------ API */
/* Where objdata.bin + hbx/ and the sprite directories live
 * (defaults "/cd/objects" and "/cd/sprites"). Call before objects_init. */
void objects_set_paths(const char *obj_dir, const char *sprite_dir);
/* Bind the stage: map = bound rlvl_map_t (the same one the player uses),
 * level_data = the whole level.bin.  Loads objdata.bin, the stage's sprite
 * resources, installs the player hooks (g_dcp_hooks.spawn_ring / atkdef /
 * anim / anim_speed, chained to the previous ones) and the solid object probe
 * (g_dcp_col.obj_probe).  Returns 0 on success. */
int objects_init(const rlvl_map_t *map, const uint8_t *level_data);
void objects_shutdown(void);
/* gPlayers[]: the players the objects interact with (max 2). Call after
 * dcp_player_create. */
void objects_add_player(DcPlayer *p);
/* DS gCamera[idx] position (24.8). n views (1 on DC, 2 = DS dual screen). */
void objects_set_view(int idx, s32 x, s32 y);
void objects_set_view_count(int n);
/* Re-run StageEvent_InitialSpawn (spawn everything in view) on the next
 * objects_frame_begin: call after a respawn or any camera jump.  The normal
 * update only spawns in the band entering the view, like the DS. */
void objects_respawn_view(void);
/* see the frame order above */
void objects_frame_begin(void);
void objects_update(DcPlayer *p);
/* Rendering (obj_draw.c, KOS only): draw every visible object, ring and
 * effect into the current list (PVR_LIST_PT_POLY). obj_draw takes the list. */
void objects_draw(void);
void obj_draw(int pvr_list);

/* rings currently alive in the ring manager (placed + scattered) */
int objects_ring_count(void);
/* RingManager unk9D5: scatter spread level, reset when all scattered rings
 * are gone.  Pass this to dcp_scatter_rings (see OBJECTS.md, deviations). */
u8 *objects_ring_scatter_level(void);
/* Set DCP_DF_ANIM_DONE on the player when its (son.bac) sprite animation
 * timing ends a non-looping animation (off by default; the renderer of the
 * 3D model may own this flag instead). */
void objects_set_player_anim_done(int on);

/* ------------------------------------------------- camera mirror */
/* The gCamera fields the camera-area objects (MapObj73 / MapObj75) and the
 * view checks write (src/game/Camera).  The DC camera module (not written
 * yet, CAMERA.md) reads bound[] / lock[] / flags / ofs_* from here.
 * g_obj_camera[2] is gCamera[2]: flags bit0 = single-screen mode, bound[] =
 * the stage default bounds.  objects_set_view() fills x/y of [0] (and [1]
 * when one view is used). */
typedef struct ObjCamera {
    u32 flags;             /* @0x00 0x10/0x20 bound pushed out (x / y), 0x1000/0x2000 offset
                              override (MapObj73), 0x4000000 an area object is in view */
    s32 x, y;              /* @0x04 @0x08 */
    s16 w, h;              /* @0x1E @0x20 view size (0x100, 0xc0) */
    u32 flags34;           /* @0x34 0x10 camera limit on (MapObj70), 0x20 limit object in view */
    u16 limit;             /* @0x38 camera limit value (MapObj70: p6 * 10000 + p7 * 100 + p8) */
    s8 player;             /* @0x3A player index */
    s32 ofs_x, ofs_y;      /* @0x3C @0x40 camera offset target */
    s32 def_ofs_x, def_ofs_y; /* @0x44 @0x48 default offset */
    s32 bound[4];          /* @0x4C left, top, right, bottom (24.8) */
    s8 lock[4];            /* @0x5C bound reference counts */
    s8 area_objs;          /* @0x60 Camera_AddAreaObject count */
    u16 water;             /* @0x62 water level (WaterLevelTrigger; flags 0x1000000 on, 0x2000000) */
} ObjCamera;
extern ObjCamera g_obj_camera[3];

/* 3D models of objects (RopeC: mod/gmk_rope_c.nsbmd, two shapes).  The
 * object module keeps the transform; the integrator draws them with the
 * model runtime (dcm_*), see OBJECTS.md section 2. */
typedef struct ObjModelView {
    const char *model;     /* converted model id, e.g. "mod__gmk_rope_c__gmk_rope_c01" */
    s32 x, y, z;           /* position (24.8 px) */
    const s32 *mtx;        /* 3x3 rotation, 4.12 fixed point, row-major */
    const s32 *scale;      /* x, y, z scale, 4.12 */
    int visible;
} ObjModelView;
int objects_model_count(void);
int objects_model_get(int i, ObjModelView *out);

/* Player_CheckStartGrind runs in objects_frame_begin (default on). Turn it
 * off when the player module calls it in Player_In itself (OBJECTS.md). */
void objects_set_grind_check(int on);

/* statistics for tests */
typedef struct ObjStats {
    u32 spawned, despawned, destroyed, live;
    u32 rings_placed, rings_collected, rings_scattered, rings_scatter_collected;
    u32 springs, dash_panels, dash_rings, item_boxes, hurts, enemies_destroyed, checkpoints, goals;
    /* objdata v2 */
    u32 gimmicks;          /* gimmick triggers (trampoline, ropes, bend stick, pedestal, corkscrew, ...) */
    u32 dec_spawned, dec_despawned; /* decorations */
    u32 walls_broken, rocks_broken, knock_aways, grinds, homing_targets, rings_attracted;
} ObjStats;
const ObjStats *objects_stats(void);

#endif

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
    OBJ_FX_WATER_SPLASH,    /* EffWaterGmk_CreateSplash (p = the toucher)     */
    /* objdata v3 (zone 2 / 3): services outside the object system */
    OBJ_FX_SCREEN_FLASH,    /* ScreenFlash_Create(a) (mine / Kuragen bomb explosion) */
    OBJ_FX_BGM_LAYER,       /* Stage_SelectBGMLayer(a): 1 = underwater mix, 0 = normal */
    OBJ_FX_JINGLE,          /* StageJingle_Create(p, a) (0x17: drowning countdown music) */
    OBJ_FX_RIPPLE3D,        /* EffRipple3D_SpawnForPlayer(p) (3D water surface ripple) */
    OBJ_FX_ENEMY_ROOM,      /* EnemyRoomHud / EffEnemyRoomFix (MapObj8C): a = 0 start, 1 count, 2 cleared */
    /* objdata v4 (zones 4..7) */
    OBJ_FX_ZONE4_BLDY,      /* ZoneFx_Zone4: a = 0 StartBldyFlash(b), a = 1 StartBldyRampUp, a = 2 StartBldyRampDown (b >> 8, b & 0xff)
                               (Night Carnival switch darkness, SwitchTimer) */
    OBJ_FX_PALETTE_ANIM     /* PaletteAnim_Create(a = table id, b = object type): palette cycling of
                               a decoration (neon); the converted atlases have fixed colours */
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
    u8 info_shown;     /* data_022c4590.unk45 (objdata v4: SwingSky hint shown once) */
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
/* Special stage round trip (gGameState.unk18 & 0x40).
 * objects_save_event_state: StageExit_Main's Stage_DestroyTasks (every object
 *   destructor and RingManager_Destructor write their spawn state back),
 *   then Stage_SaveEventState (copy of the map-object list and the ring list:
 *   destroyed objects and collected rings stay gone) and
 *   Stage_SaveRestartCamera (gCamera x, y and gCamera[2] flags & 2).  The
 *   data_022c4590 fields of g_obj_stage (checkpoint index / time / position,
 *   score tally, hint shown) are kept with it: the DS does not clear them
 *   for the return.  The copy lives in static storage and survives
 *   objects_shutdown / objects_init.  Call it on
 *   stage_exit(OBJ_EXIT_SPECIAL_STAGE), before objects_shutdown.
 * objects_restore_event_state: StageStart_State_SetupResources with
 *   unk18 & 0x40 (Stage_RestoreEventState + Stage_RestoreRestartCamera),
 *   applied before StageStart_State_CreateStage spawns anything.  Call it
 *   either just before objects_init (it is applied inside objects_init) or
 *   just after it (the objects objects_init created are recreated from the
 *   restored list), in both cases before the first objects_frame_begin.  The
 *   saved state is used once; it is ignored for another stage.
 * objects_restart_camera: the restored camera (24.8, gCamera[0]) and flag 2,
 *   1 if a restore set it since the last objects_init, for the camera module. */
void objects_save_event_state(void);
/* Player_InitState, collision box part: coll2A0.unk1C = Player_OnHurt (the
 * glider sequence replaces it), box flags 0x300 cleared, atk/def reset.  The
 * player module's dcp_player_init_state should call it (OBJECTS.md 2.2) */
void objects_player_init_state(DcPlayer *p);
void objects_restore_event_state(void);
int objects_restart_camera(s32 *x, s32 *y, u32 *flags);
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
void objects_draw_tr(void);   /* semi-transparent sprites (player effects): inside the TR list */
void obj_draw(int pvr_list);
/* EffTrail (Sonic's boost ribbon): the quads of the live trails, newest segment first; corners
 * 0/1 = the newer node's two ends, 2/3 = the older node's (24.8 world px). The DS draws them flat
 * in the 3D layer with polygon alpha `alpha` (0..31) and vertex colour `bgr555`. */
typedef struct ObjTrailQuad {
    s32 x[4], y[4];
    u16 bgr555;
    u8 alpha;
} ObjTrailQuad;
int objects_trail_quads(ObjTrailQuad *q, int max);

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
    int visible;           /* objdata v4: !(disp_flags & 0x20) (before v4: disp_flags & 0x200) */
    /* objdata v4 */
    u32 shapes;            /* shapes to draw (dcm_draw_t.shape bit set); 0xffffffff = all.
                              StageObj_LoadModelSimple blocks hidden by @0xA8 & 0x80 are cleared */
    int persp;             /* disp_flags & 0x200: StageObj_Draw3DEx draws with the off-axis
                              frustum (data_022c4ef4) centred on the object, else orthographic */
    int reps;              /* draw the model reps times (>= 1) ... */
    s32 rep_dy;            /* ... each one rep_dy (24.8) lower than the one before (CranePartEE) */
} ObjModelView;
/* objdata v4: objects with a model view keep drawing their 2D sprite as a
 * stand-in (default 1).  Call objects_set_model_sprites(0) once the
 * integrator draws the model views: StageObj_Draw2DEx does not draw the
 * main sprite of an object that has a model (KOS build, obj_draw.c). */
void objects_set_model_sprites(int on);
/* KOS: PVR texture bytes the loaded sprite resources hold (VRAM budget probe) */
u32 objects_vram_used(void);
int objects_model_count(void);
int objects_model_get(int i, ObjModelView *out);

/* Player_CheckStartGrind runs in objects_frame_begin (default on). Turn it
 * off when the player module calls it in Player_In itself (OBJECTS.md). */
void objects_set_grind_check(int on);
/* Player_UpdateUnderwater (water level, drowning count, air bubbles) runs
 * in objects_frame_begin (default on). Turn it off when the player module
 * calls it in Player_In itself (OBJECTS.md section 2.2). */
void objects_set_underwater_check(int on);

/* statistics for tests */
typedef struct ObjStats {
    u32 spawned, despawned, destroyed, live;
    u32 rings_placed, rings_collected, rings_scattered, rings_scatter_collected;
    u32 springs, dash_panels, dash_rings, item_boxes, hurts, enemies_destroyed, checkpoints, goals;
    /* objdata v2 */
    u32 gimmicks;          /* gimmick triggers (trampoline, ropes, bend stick, pedestal, corkscrew, ...) */
    u32 dec_spawned, dec_despawned; /* decorations */
    u32 walls_broken, rocks_broken, knock_aways, grinds, homing_targets, rings_attracted;
    /* objdata v3 (zones 2 and 3) */
    u32 water_entries, water_exits, water_splashes, water_runs, zone_launches, drownings, air_bubbles,
        bubbles_spawned, mines_exploded, kuragen_drops, fish_flee;
    u32 sand_slides, cargo_rides, table_rides, rails, rocks_fallen, enemy_rooms, bishops_spawned;
} ObjStats;
const ObjStats *objects_stats(void);

#endif

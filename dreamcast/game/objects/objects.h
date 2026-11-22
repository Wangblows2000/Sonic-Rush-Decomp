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
    OBJ_EXIT_CLEAR_DEMO = 1 /* StageShutdown_Create(ClearDemo_Create)     */
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

/* statistics for tests */
typedef struct ObjStats {
    u32 spawned, despawned, destroyed, live;
    u32 rings_placed, rings_collected, rings_scattered, rings_scatter_collected;
    u32 springs, dash_panels, dash_rings, item_boxes, hurts, enemies_destroyed, checkpoints, goals;
} ObjStats;
const ObjStats *objects_stats(void);

#endif

/*
 * Native collision interface between the player module and the stage module.
 *
 * The map side sits directly on the stage package queries of
 * tools/dreamcast/stages/level_format.h (rlvl_tile_height / rlvl_tile_width /
 * rlvl_probe_distance / rlvl_probe_distance_long, bit-exact equivalents of
 * MapCollision_GetTileHeight / _GetTileWidth / _ProbeDistance /
 * _ProbeDistanceLong). The player module contains the faithful translations of
 * everything layered on top of them: StageObj_ProbeCollision,
 * StageObj_CollideMap{Impl,Horizontal,Vertical}, StageObj_CheckCeiling,
 * StageObj_Collision.
 *
 * Solid objects (moving platforms etc., ObjCollisionMap_ProbeForObj) are an
 * optional callback implemented by the object module.
 *
 * Coordinates passed to the probes are whole pixels (position >> 8).
 * See docs/dreamcast/translation/PLAYER.md "Collision interface".
 */
#ifndef DCP_COLLISION_H
#define DCP_COLLISION_H

#include "dcp_types.h"
#include "../../../tools/dreamcast/stages/level_format.h"

/* Probe direction ('dir'/'mode' argument of the original probes) = RLVL_DIR_* */
enum {
    DCP_DIR_RIGHT = RLVL_DIR_RIGHT, /* +x, horizontal (tile width masks) */
    DCP_DIR_LEFT = RLVL_DIR_LEFT,   /* -x */
    DCP_DIR_DOWN = RLVL_DIR_DOWN,   /* +y, vertical (tile height masks) */
    DCP_DIR_UP = RLVL_DIR_UP        /* -y */
};

/* Probe flag byte (StageObj_GetMapProbeFlags) = RLVL_Q_* */
#define DCP_PF_LAYER1 RLVL_Q_LAYER1    /* collision layer B (player default) */
#define DCP_PF_RECT RLVL_Q_ARENA       /* distance to the arena rect, clamped +-31 */
#define DCP_PF_OOB RLVL_Q_EDGE_SOLID   /* outside the map is solid (else clamped) */
#define DCP_PF_NOTHRU RLVL_Q_SKIP_TOP  /* top-solid chars (attr bit0) are empty */

typedef struct DcPlayer DcPlayer;

/* Solid-object probe. Original: ObjCollisionMap_ProbeForObj(obj, x, y, flags,
 * dir, angle, attr). Same return convention as rlvl_probe_distance_long
 * (signed distance), 0x18 when nothing is near. It may set p->ride (object
 * stood on, when d <= 0/1 in the floor direction) and p->touch. */
typedef s32 (*DcpObjProbeFn)(void *ctx, DcPlayer *p, s32 x, s32 y, u32 flags, u32 dir, u8 *angle, u8 *attr);

typedef struct DcpCollision {
    const rlvl_map_t *map;   /* required: bound stage (rlvl_bind) */
    void *obj_ctx;
    DcpObjProbeFn obj_probe; /* optional (NULL = no solid objects -> 0x18) */
} DcpCollision;

extern DcpCollision g_dcp_col;

/* Bind the stage collision map for the player probes. */
void dcp_bind_map(const rlvl_map_t *m);

/* MapCollision_ProbeDistance / _ProbeDistanceLong on the bound map. */
static inline s32 dcp_probe_tile(s32 x, s32 y, u32 flags, u32 dir, u8 *angle, u8 *attr) {
    return rlvl_probe_distance(g_dcp_col.map, x, y, flags, dir, angle, attr);
}
static inline s32 dcp_probe_long(s32 x, s32 y, u32 flags, u32 dir, u8 *angle, u8 *attr) {
    return rlvl_probe_distance_long(g_dcp_col.map, x, y, flags, dir, angle, attr);
}

#endif

/*
 * Map binding for the player probes. The probes themselves are the stage
 * package reference queries (tools/dreamcast/stages/level_format.h).
 */
#include "dcp_collision.h"
#include "dcp_player.h"

DcpCollision g_dcp_col;

/* binding a stage map also sets the stage bounds the player module reads:
 * data_ov000_0236abd8.unk10 = map height in px (StageLoader_OnMapArchiveLoaded: blocks << 6), the
 * pit-death line of Player_In, and gStageMapCollision.unk1C / unk24 = 0 / width (the defaults of
 * Player_UpdateGimmickStageBounds) */
void dcp_bind_map(const rlvl_map_t *m) {
    g_dcp_col.map = m;
    if (m != NULL) {
        g_dcp_world.death_y = m->bottom;
        g_dcp_world.map_left = 0;
        g_dcp_world.map_right = m->right;
    }
}

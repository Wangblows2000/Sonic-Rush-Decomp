/*
 * Map binding for the player probes. The probes themselves are the stage
 * package reference queries (tools/dreamcast/stages/level_format.h).
 */
#include "dcp_collision.h"

DcpCollision g_dcp_col;

void dcp_bind_map(const rlvl_map_t *m) { g_dcp_col.map = m; }

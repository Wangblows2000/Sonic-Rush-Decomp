#pragma opt_common_subs off
#include <nitro/types.h>

typedef struct UnkStruct_02304064_Area {
    u8 pad[6];
    s8 left;
    s8 top;
    s8 right;
    s8 bottom;
} UnkStruct_02304064_Area;

typedef struct UnkStruct_02304064_Player {
    u8 pad[0x10];
    s32 x;
    s32 y;
} UnkStruct_02304064_Player;

typedef struct UnkStruct_02304064 {
    u8 pad0[8];
    s32 id;
    u8 pad1[4];
    s32 x;
    s32 y;
    u8 pad2[0x1e0 - 0x18];
    UnkStruct_02304064_Area *area;
} UnkStruct_02304064;

extern UnkStruct_02304064_Player *gPlayers[];

BOOL MapObj74_IsPlayerInArea(UnkStruct_02304064 *work)
{
    s32 lo;
    BOOL inX;
    UnkStruct_02304064_Area *area;
    BOOL inY;
    BOOL result;
    UnkStruct_02304064_Player *player;

    result = FALSE;
    area = work->area;
    player = gPlayers[work->id];
    inX = FALSE;
    lo = work->x + (area->left << 11);
    if (lo <= player->x && lo + ((area->right - area->left) << 11) >= player->x)
        inX = TRUE;
    if (inX) {
        inY = FALSE;
        lo = work->y + (area->top << 11);
        if (lo <= player->y && lo + ((area->bottom - area->top) << 11) >= player->y)
            inY = TRUE;
        if (inY)
            result = TRUE;
    }
    return result;
}

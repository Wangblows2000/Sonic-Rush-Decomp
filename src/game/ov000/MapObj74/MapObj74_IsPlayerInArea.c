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

static inline UnkStruct_02304064_Player *GetPlayer_02304064(s32 id) { return gPlayers[id]; }

static inline BOOL InRange(s32 v, s32 base, const s8 *a, const s8 *b)
{
    BOOL r;
    s32 lo;
    r = FALSE;
    lo = base + (*a << 11);
    if (lo <= v && lo + ((*b - *a) << 11) >= v)
        r = TRUE;
    return r;
}

BOOL MapObj74_IsPlayerInArea(UnkStruct_02304064 *work)
{
    BOOL result;
    UnkStruct_02304064_Area *area;
    UnkStruct_02304064_Player *player;

    result = FALSE;
    area = work->area;
    player = GetPlayer_02304064(work->id);

    if (InRange(player->x, work->x, &area->left, &area->right)) {
        if (InRange(player->y, work->y, &area->top, &area->bottom))
            result = TRUE;
    }
    return result;
}

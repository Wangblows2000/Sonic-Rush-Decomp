#include <nitro/types.h>

typedef struct {
    u8 pad0[0xC];
    s32 offX;
    s32 offY;
    u8 pad14[4];
    s32 w;
    s32 h;
} UnkHitbox;

typedef struct {
    u8 pad0[0x18];
    s32 x;
    s32 y;
    u8 pad20[0xB0 - 0x20];
    UnkHitbox box;
} UnkPlayer;

extern UnkPlayer *ExtraBoss_GetPlayer(void);

BOOL ExtraBoss_IsRingTouchingPlayer(s32 *pos)
{
    UnkPlayer *player = ExtraBoss_GetPlayer();
    UnkHitbox *box = &player->box;
    s32 cx, cy, pl, pt, pr, pb;
    cx = box->offX;
    cx = (s32)((u32)player->x + (u32)cx);
    pr = cx + box->w;
    pl = cx - box->w;
    cy = box->offY;
    cy = (s32)((u32)player->y + (u32)cy);
    pt = cy - box->h;
    pb = cy + box->h;
    s32 rx = pos[0];
    s32 rr = rx + 0x8000;
    s32 rl = rx - 0x8000;
    s32 ry = pos[1];
    s32 rt = ry - 0x10000;

    if (pl > rl && pl > rr)
        return FALSE;
    if (rl > pl && rl > pr)
        return FALSE;
    if (pt > rt && pt > ry)
        return FALSE;
    if (rt > pt && rt > pb)
        return FALSE;
    return TRUE;
}

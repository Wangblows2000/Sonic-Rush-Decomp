#include <nitro/types.h>

typedef struct { s32 x; s32 y; } UnkVec_Mine;

typedef struct UnkStruct_Mine {
    u8 pad0[6];
    u8 unk6;
    u8 pad7[0xc - 7];
    s32 timer;
    s32 x;
    s32 y;
    u8 pad18[0x54 - 0x18];
    u32 flags54;
    u8 pad58[0x330 - 0x58];
    UnkVec_Mine segs[16];
} UnkStruct_Mine;

s32 MapCollision_ProbeDistance(s32 x, s32 y, s32 a, s32 b, s32 c, s32 d);

void Mine_State_Main(UnkStruct_Mine *w)
{
    s32 dist = 0;
    s16 i;
    s16 idx;

    w->timer++;
    idx = ((w->timer >> 4) - 1) & 7;
    w->unk6 = (0x9515 >> (idx * 2)) & 3;

    for (i = 0; i < 16; i++) {
        
        w->segs[i].x = w->x;
        idx = (i + (w->timer >> 4)) & 7;
        idx = (0x9515 >> (idx * 2)) & 3;
        w->segs[i].x += idx << 8;
        if (i == 3) {
            w->segs[i].y = w->segs[i - 1].y;
            continue;
        }
        if (dist < 0) {
            w->segs[i].x = 0;
            continue;
        }
        if (i == 4) {
            w->segs[i].y = w->segs[i - 1].y + 0x1e00;
        } else if (i != 0) {
            w->segs[i].y = w->segs[i - 1].y + 0xa00;
        } else {
            w->segs[i].y = w->y + 0x1800;
        }
        dist = MapCollision_ProbeDistance(w->segs[i].x >> 8, w->segs[i].y >> 8, 1, 2, 0, 0);
        if (dist < 0)
            w->segs[i].x = 0;
    }
    if (w->flags54 & 2)
        w->flags54 |= 4;
}

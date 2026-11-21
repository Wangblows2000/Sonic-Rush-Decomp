#include <nitro/types.h>

typedef struct { u8 pad[4]; s16 x0; s16 y0; s16 x1; s16 y1; } UnkStruct_022c678c_e;
typedef struct { u8 pad[0xc]; s32 off; } UnkStruct_022c678c_q;

BOOL Ending_HitTestSprite(u8 *p, s32 x, s32 y) {
    UnkStruct_022c678c_q *q = *(UnkStruct_022c678c_q **)(p + 0x10);
    UnkStruct_022c678c_e *e = (UnkStruct_022c678c_e *)(*(s32 *)(p + 0x24) + (s32)((u8 *)q + q->off));
    s16 px = *(s16 *)(p + 4);
    s16 py = *(s16 *)(p + 6);
    s32 x1 = px + e->x1;
    s32 x0 = px + e->x0;
    s32 y1 = py + e->y1;
    s32 y0 = py + e->y0;
    if (x1 < x || x0 > x || y0 > y || y1 < y) {
        return FALSE;
    }
    return TRUE;
}

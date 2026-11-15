#include <nitro/types.h>

typedef struct { u8 pad[4]; s16 x0; s16 y0; s16 x1; s16 y1; } UnkStruct_022cfb90_e;
typedef struct { u8 pad[0xc]; s32 off; } UnkStruct_022cfb90_q;

#pragma thumb on
void func_ov001_022cfb90(u8 *p, s32 *w, s32 *h) {
    UnkStruct_022cfb90_q *q = *(UnkStruct_022cfb90_q **)(p + 0x10);
    UnkStruct_022cfb90_e *e = (UnkStruct_022cfb90_e *)((UnkStruct_022cfb90_e *)(*(s32 *)(p + 0x24) + (s32)((u8 *)q + q->off)));
    if (w != NULL) {
        *w = e->x1 - e->x0;
    }
    if (h != NULL) {
        *h = e->y1 - e->y0;
    }
}
#pragma thumb reset

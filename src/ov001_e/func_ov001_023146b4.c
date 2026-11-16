#include <nitro/types.h>

typedef struct UnkStruct_023146b4_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_023146b4_v;

typedef struct UnkStruct_023146b4_pt {
    u16 x;
    u16 y;
} UnkStruct_023146b4_pt;

typedef struct UnkStruct_023146b4_e {
    UnkStruct_023146b4_pt pts[8];
    u16 unk20;
    u16 count;
    u8 pad24[4];
} UnkStruct_023146b4_e;

typedef struct UnkStruct_023146b4_g {
    u8 pad0[0x10c];
    UnkStruct_023146b4_e *entries;
} UnkStruct_023146b4_g;

typedef struct UnkStruct_023146b4_tp {
    u16 x;
    u16 y;
    u8 pad4[0xc];
    u32 flags;
} UnkStruct_023146b4_tp;

extern UnkStruct_023146b4_g *data_ov001_0231f6a0;
extern UnkStruct_023146b4_tp gTouchState;
extern u8 data_ov001_02317e80[];

BOOL func_ov001_0230adcc(void *a, UnkStruct_023146b4_v *v, s32 x, s32 y);

s32 func_ov001_023146b4(s32 idx, s32 n, s32 cur) {
    UnkStruct_023146b4_e *e = &data_ov001_0231f6a0->entries[idx];
    s32 i;
    UnkStruct_023146b4_v v;

    if (e->unk20 < n)
        return -1;
    if (gTouchState.flags & 0x10) {
        for (i = cur; i < e->count; i++) {
            v.x = e->pts[i].x << 12;
            v.y = e->pts[i].y << 12;
            if (func_ov001_0230adcc(data_ov001_02317e80, &v, gTouchState.x << 12, gTouchState.y << 12)) {
                if (i == cur)
                    return cur < e->count - 1 ? cur + 1 : 0x1234;
                return -1;
            }
        }
    }
    return cur;
}

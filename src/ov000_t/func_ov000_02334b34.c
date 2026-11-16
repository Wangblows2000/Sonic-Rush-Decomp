#include <nitro/types.h>

typedef struct {
    u32 l;
    u32 t;
    u32 r;
    u32 b;
} UnkStruct_02334b34_R;

typedef struct {
    u8 pad0[0x20];
    u8 *unk20;
} UnkStruct_02334b34;

typedef struct {
    u8 pad0[0xE];
    u16 unkE;
} UnkStruct_02334b34_M;

extern UnkStruct_02334b34_M data_ov000_0236abd8;

void func_ov000_0235b0c8(u8 *e, s32 x, s32 y);

void func_ov000_02334b34(UnkStruct_02334b34 *w, u16 x, u16 y, UnkStruct_02334b34_R *rc) {
    u32 off; u8 *e; u16 n; u8 *base; u16 i; s32 bx; s32 by; u32 px; u32 py;
    base = w->unk20;
    off = *(u32 *)(base + 4 + (u16)(y * (((data_ov000_0236abd8.unkE - 1) >> 8) + 1) + x) * 4);
    bx = x << 8;
    e = base + off + 2;
    by = y << 8;
    n = *(u16 *)(base + off);
    for (i = 0; i < n; i++, e += 2) {
        if (e[0] != 0xff) {
            px = (u32)e[0] + bx;
            py = (u32)e[1] + by;
            if (px >= rc->l && px <= rc->r && py >= rc->t && py <= rc->b)
                func_ov000_0235b0c8(e, (bx + e[0]) << 8, (by + e[1]) << 8);
        }
    }
}

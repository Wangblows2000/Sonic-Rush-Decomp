#include <nitro/types.h>

typedef struct {
    u32 flags;
    s32 unk4;
    s32 unk8;
    u8 padC[0x3a - 0xc];
    s8 unk3A;
    u8 pad3B;
    s32 unk3C;
    s32 unk40;
    u8 pad44[0x64 - 0x44];
} UnkPlayer_02339110;

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
} UnkObj_02339110;

extern UnkPlayer_02339110 data_ov000_0236aad8[];
extern UnkObj_02339110 *data_ov000_0236ac60[];

void func_ov000_02339110(void *unused, s32 idx) {
    UnkPlayer_02339110 *p = &data_ov000_0236aad8[idx];

    if (p->flags & 0x10000)
        p->unk4 = data_ov000_0236ac60[p->unk3A]->x - p->unk3C;
}

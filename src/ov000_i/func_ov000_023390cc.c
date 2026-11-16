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
} UnkPlayer_023390cc;

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
} UnkObj_023390cc;

extern UnkPlayer_023390cc gCamera[];
extern UnkObj_023390cc *gPlayers[];

void func_ov000_023390cc(void *unused, s32 idx) {
    UnkPlayer_023390cc *p = &gCamera[idx];

    if (p->flags & 0x20000)
        p->unk8 = gPlayers[p->unk3A]->y - p->unk40;
}

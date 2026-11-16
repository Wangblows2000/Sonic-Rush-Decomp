#include <nitro/types.h>

typedef struct {
    u8 pad0[0x8c];
    void **unk8c;
    u8 pad90[0x30];
    u32 unkc0;
    u8 padc4[0x14];
    u16 unkd8;
    u8 padda[0x2e];
    u32 unk108;
    u8 pad10c[0x14];
    u16 unk120;
} UnkObj_0206a054;

u16 func_0206d2d0(void *p, u32 a, u32 b);

void func_0206a054(UnkObj_0206a054 *o, u32 a, u32 b) {
    u16 v = 0;

    if (o->unk8c != NULL && *o->unk8c != NULL)
        v = func_0206d2d0(*o->unk8c, a, b);
    o->unkd8 = v;
    o->unk120 = v;
    o->unkc0 |= 0x10;
    o->unk108 |= 0x10;
}

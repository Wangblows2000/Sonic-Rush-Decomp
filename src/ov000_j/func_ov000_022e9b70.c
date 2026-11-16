#include <nitro/types.h>

typedef struct {
    u8 pad0[0xa];
    u16 unkA;
    u8 padC[0x2c - 0xc];
} UnkStruct_022e9b70_c;

typedef struct {
    u8 pad0[0x378];
    UnkStruct_022e9b70_c c0;
    UnkStruct_022e9b70_c c1;
} UnkStruct_022e9b70;

void func_ov000_022e9b70(UnkStruct_022e9b70 *p, s32 a, s32 b) {
    UnkStruct_022e9b70_c *c;

    c = &p->c0;
    if (a)
        c->unkA |= 4;
    else
        c->unkA &= ~4;
    c = &p->c1;
    if (b)
        c->unkA |= 4;
    else
        c->unkA &= ~4;
}

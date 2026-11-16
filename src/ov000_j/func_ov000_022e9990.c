#include <nitro/types.h>

typedef struct {
    u8 pad0[0x114];
    s32 unk114;
} UnkStruct_022e9990_124;

typedef struct {
    u8 pad0[0x34];
    s16 unk34;
    u8 pad36[0xbe - 0x36];
    s16 unkBE;
    u8 padC0[0x106 - 0xc0];
    s16 unk106;
    u8 pad108[0x124 - 0x108];
    UnkStruct_022e9990_124 *unk124;
} UnkStruct_022e9990;

void func_ov000_022e9990(UnkStruct_022e9990 *p, s32 a) {
    s32 t;
    s16 v;

    t = (a >> 5) + (p->unk34 >> 6);
    if (t < 0)
        t = -t;
    v = t;
    if (v <= 0x10)
        v = 0x10;
    if (v >= 0x80)
        v = 0x80;
    p->unkBE = v;
    p->unk106 = v;
    if (p->unk124 != NULL)
        p->unk124->unk114 = (v << 12) >> 4;
}

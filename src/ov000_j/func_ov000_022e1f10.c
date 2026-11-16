#include <nitro/types.h>

typedef struct {
    u8 pad0[0xd54];
    s32 unkD54;
    s32 unkD58;
    u8 padD5C[2];
    u16 unkD5E;
    u8 padD60[0xf68 - 0xd60];
    s16 unkF68;
} UnkStruct_022e1f10;

typedef struct {
    u8 pad0[0x20c];
    u32 unk20C;
} UnkStruct_022e1f10_g;

extern UnkStruct_022e1f10_g *data_ov000_02369b4c;

void func_ov000_022e1f10(UnkStruct_022e1f10 *p) {
    s32 v;
    s32 d;

    v = p->unkF68;
    if (v < 0)
        v = -v;
    d = v * 2;
    if (p->unkD5E != 0) {
        if (data_ov000_02369b4c->unk20C & 0x100)
            d = -d;
    } else {
        if (!(data_ov000_02369b4c->unk20C & 0x100))
            d = -d;
    }
    p->unkD54 = d - 0x17ff;
    p->unkD58 = d + 0x17ff;
}

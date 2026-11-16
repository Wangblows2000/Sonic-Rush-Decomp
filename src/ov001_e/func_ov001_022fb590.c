#include <nitro/types.h>

typedef struct UnkStruct_022fb590_o {
    u8 pad0[0x110];
    u8 unk110;
} UnkStruct_022fb590_o;

typedef struct UnkStruct_022fb590 {
    u8 pad0[0x14];
    u32 flags;
    u8 pad18[0x84 - 0x18];
    UnkStruct_022fb590_o *unk84;
    u8 pad88[0x1d0 - 0x88];
    s32 unk1D0;
} UnkStruct_022fb590;

extern u8 *data_ov001_0231f094;

void func_ov001_022fb590(UnkStruct_022fb590 *p) {
    UnkStruct_022fb590_o *o = p->unk84;

    if ((p->unk1D0 == 3 && (o->unk110 & 2)) || data_ov001_0231f094[1] != 0)
        p->flags |= 8;
}

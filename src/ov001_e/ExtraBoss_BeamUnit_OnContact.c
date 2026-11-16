#include <nitro/types.h>

typedef struct UnkStruct_022fb918_o {
    u8 pad0[4];
    s32 type;
} UnkStruct_022fb918_o;

typedef struct UnkStruct_022fb918 {
    u8 pad0[0x14];
    u32 flags;
    u8 pad18[0x1c8 - 0x18];
    s32 unk1C8;
} UnkStruct_022fb918;

extern u8 *data_ov001_0231f094;

void ExtraBoss_BeamUnit_OnContact(UnkStruct_022fb918 **self, UnkStruct_022fb918_o **other) {
    UnkStruct_022fb918_o *o = *other;
    UnkStruct_022fb918 *p = *self;

    if (o->type != 2)
        return;
    p->flags |= 8;
    if (p->unk1C8 == 8)
        *data_ov001_0231f094 &= ~1;
    else
        *data_ov001_0231f094 &= ~2;
}

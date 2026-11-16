#include <nitro/types.h>

typedef struct UnkStruct_022f4028_o {
    u8 pad0[0x114];
    s32 unk114;
} UnkStruct_022f4028_o;

typedef struct UnkStruct_022f4028 {
    u8 pad0[0x14];
    u32 flags;
    u8 pad18[0x7c - 0x18];
    struct UnkStruct_022f4028 *parent;
    u8 pad80[4];
    UnkStruct_022f4028_o *unk84;
    u8 pad88[0x1c8 - 0x88];
    s32 timer;
} UnkStruct_022f4028;

void ExtraBossObj_State_Unk_ov001_022f4028(UnkStruct_022f4028 *p) {
    p->flags ^= 0x30000;
    p->timer--;
    if (p->timer > 0)
        return;
    p->parent->unk84->unk114 = 0x2000;
    p->unk84->unk114 = 0x2000;
}

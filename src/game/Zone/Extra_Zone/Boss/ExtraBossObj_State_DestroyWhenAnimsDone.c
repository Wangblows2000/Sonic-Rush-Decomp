#include <nitro/types.h>

typedef struct UnkStruct_022fd220_o {
    u8 pad0[0xe4];
    void *parts[5];
    u8 padF8[0x10c - 0xf8];
    u8 flags[5];
} UnkStruct_022fd220_o;

typedef struct UnkStruct_022fd220 {
    u8 pad0[0x14];
    u32 flags;
    u8 pad18[0x84 - 0x18];
    UnkStruct_022fd220_o *unk84;
} UnkStruct_022fd220;

void ExtraBossObj_State_DestroyWhenAnimsDone(UnkStruct_022fd220 *p) {
    s32 i;
    UnkStruct_022fd220_o *o = p->unk84;

    for (i = 0; i < 5; i++) {
        if (o->parts[i] != NULL && !(o->flags[i] & 2))
            break;
    }
    if (i == 5)
        p->flags |= 8;
}

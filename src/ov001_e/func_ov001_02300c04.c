#include <nitro/types.h>

typedef struct UnkStruct_02300c04_o {
    u8 pad0[0x14];
    u32 flags;
} UnkStruct_02300c04_o;

typedef struct UnkStruct_02300c04 {
    u8 pad0[0xbc];
    UnkStruct_02300c04_o *objs[0x2e];
} UnkStruct_02300c04;

extern UnkStruct_02300c04 *gExtraBossPlayerManager;

void func_ov001_02301118(UnkStruct_02300c04_o *o);

void func_ov001_02300c04(void) {
    s32 i;

    for (i = 0; i < 0x2e; i++) {
        if (gExtraBossPlayerManager->objs[i] != NULL && (gExtraBossPlayerManager->objs[i]->flags & 8))
            func_ov001_02301118(gExtraBossPlayerManager->objs[i]);
    }
}

#include <nitro/types.h>

typedef struct UnkStruct_0235f8cc {
    u8 pad[0x8];
    u16 unk8;
} UnkStruct_0235f8cc;

extern UnkStruct_0235f8cc gDisplayMain;
extern UnkStruct_0235f8cc gDisplaySub;

#pragma thumb on
void func_ov000_0235f8cc(void) {
    gDisplayMain.unk8 += 2;
    gDisplaySub.unk8 += 2;
}
#pragma thumb reset

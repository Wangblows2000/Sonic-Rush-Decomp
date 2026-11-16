#include <nitro/types.h>

extern u32 gSaveData[];

void func_0205a4ec(s32 a);

#pragma thumb on
void func_ov000_023563b4(void) {
    gSaveData[2] |= 0x200000;
    func_0205a4ec(0);
}
#pragma thumb reset

#include <nitro/types.h>

typedef struct { u8 unk0[0x10]; u32 unk10; u8 unk14; } UnkStruct_02037cac;
extern UnkStruct_02037cac gTouchState;
extern u32 gSystemFlags;
void func_0200d554(void);
void func_0200d18c(u32 a);
BOOL func_0200d178(u32 a);

void Touch_StopAuto(void) {
    if (gTouchState.unk10 & 2) {
        func_0200d554();
        func_0200d18c(4);
        if (func_0200d178(4)) return;
        gTouchState.unk10 &= ~1;
        gTouchState.unk10 &= ~2;
        gTouchState.unk14 = 0;
        gSystemFlags &= ~0x8000;
    } else {
        gTouchState.unk10 &= ~1;
    }
}

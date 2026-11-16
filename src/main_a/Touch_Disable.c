#include <nitro/types.h>

typedef struct { u8 unk0[0x10]; u32 unk10; u8 unk14; } UnkStruct_02037d40;
extern UnkStruct_02037d40 gTouchState;

void Touch_Disable(void) {
    gTouchState.unk14 = 0;
    gTouchState.unk10 &= ~1;
}

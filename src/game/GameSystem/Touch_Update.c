#include <nitro/types.h>

typedef struct { u8 unk0[0x10]; u32 unk10; } UnkStruct_02037c60;
extern UnkStruct_02037c60 gTouchState;
void Touch_UpdateAuto(void);
void Touch_UpdateSingle(void);

void Touch_Update(void) {
    u32 f = gTouchState.unk10;
    if (!(f & 1)) return;
    if (f & 2) {
        Touch_UpdateAuto();
    } else {
        Touch_UpdateSingle();
    }
}

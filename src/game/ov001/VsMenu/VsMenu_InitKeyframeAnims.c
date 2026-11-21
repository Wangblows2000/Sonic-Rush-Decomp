#include <nitro/types.h>

typedef struct { u8 data[0x2c]; } UnkStruct_022ddacc;
void KeyframeAnim_Init(void *p, s32 a, s32 b);

#pragma thumb on
void VsMenu_InitKeyframeAnims(u8 *p) {
    s32 v = *(s32 *)(p + 0xdc8);
    s32 i;
    for (i = 0; i < 0xe; i++) {
        KeyframeAnim_Init(&((UnkStruct_022ddacc *)(p + 0xb44))[i], v, 0);
    }
}
#pragma thumb reset

#include <nitro/types.h>

typedef struct UnkStruct_0230b554 {
    u8 pad0[0x2c];
    s32 timer;
} UnkStruct_0230b554;

extern UnkStruct_0230b554 *gSpecialStageWork;

void func_ov001_0230b554(void) {
    if (gSpecialStageWork->timer == 0x19) {
        u32 v = (*(vu32 *)0x4000000 & 0x1f00) >> 8;
        *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | ((v | 2) << 8);
    }
}

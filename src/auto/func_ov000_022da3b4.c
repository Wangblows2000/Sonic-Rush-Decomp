/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gTaskCurrent[];
s32 Model_Animate();
s32 Model_Draw();

void func_ov000_022da3b4(void) {
    if ((*(u16 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x5c) & 0x20) != 0) {
        return;
    }
    s32 t0 = (*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x23c);
    Model_Animate((*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x23c), *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    Model_Draw(t0);
}

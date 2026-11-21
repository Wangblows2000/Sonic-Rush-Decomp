/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gTaskCurrent[];
s32 Model_Draw();

void Boss1_Draw(void) {
    s32 t0 = (*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x23c);
    s32 t1 = (*(u16 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x5c) & 0x20);
    if (t1 != 0) {
        return;
    }
    Model_Draw(t0, t1);
}

/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gTaskCurrent[];
s32 Model_Animate();
s32 Model_Draw();

void Boss6PartD4_Draw(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t1 = (*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x20c);
    *(u32 *)((u8 *)t1 + 0x48) = (*(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x10) << 4);
    *(u32 *)((u8 *)t1 + 0x4c) = (-*(u32 *)((u8 *)t0 + 0x14) << 4);
    s32 t2 = (*(u32 *)((u8 *)t0 + 0x18) << 4);
    *(u32 *)((u8 *)t1 + 0x50) = t2;
    Model_Animate(t1, t2, t0);
    Model_Draw(t1);
}

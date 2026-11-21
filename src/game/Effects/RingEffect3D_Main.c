/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gTaskCurrent[];
s32 Task_Destroy();
s32 Model_Animate();
s32 Model_Draw();

void RingEffect3D_Main(void) {
    s32 t0 = *(u32 *)gTaskCurrent;
    s32 t1 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t2 = (*(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0xbc) & 2);
    if (t2 != 0) {
        Task_Destroy(t0, t2);
        return;
    }
    Model_Animate(t1);
    Model_Draw(t1);
}

/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gTaskCurrent[];
s32 StageObj_Draw2DEx();
s32 Task_Destroy();

void RingEffect2D_Main(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    StageObj_Draw2DEx(*(u32 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10), *(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x4), 0, 0, (*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x8), (*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x50), 0);
    if ((*(u32 *)((u8 *)t0 + 0x34) & 2) == 0) {
        return;
    }
    Task_Destroy(*(u32 *)gTaskCurrent);
}

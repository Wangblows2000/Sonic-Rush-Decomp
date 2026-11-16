/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gTaskCurrent[];
s32 StageObj_Draw2D();

void func_ov000_023206f0(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    StageObj_Draw2D(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10), (*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x94), (*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0xdc));
    s32 t1 = *(u16 *)((u8 *)t0 + 0x5c);
    s32 t2 = (*(u16 *)((u8 *)t0 + 0x5c) ^ 1);
    *(u16 *)((u8 *)t0 + 0x5c) = t2;
    StageObj_Draw2D(t0, (t0 + 0x94), (t0 + 0xdc), t2);
    *(u16 *)((u8 *)t0 + 0x5c) = t1;
}

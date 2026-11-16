/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gTaskCurrent[];
s32 StageObj_Draw2D();

void func_ov000_023165ec(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t1 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t2 = (*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x20c);
    *(u16 *)((u8 *)t0 + 0x5c) = (*(u16 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x5c) | 0x100);
    StageObj_Draw2D(t0, t2, (t0 + 0x254));
    s32 t3 = (*(u16 *)((u8 *)t0 + 0x5c) & ~0x100);
    *(u16 *)((u8 *)t0 + 0x5c) = t3;
    StageObj_Draw2D(t0, (t0 + 0x94), (t0 + 0xdc), t3);
}

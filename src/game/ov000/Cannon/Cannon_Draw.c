/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gTaskCurrent[];
s32 StageObj_Draw2D();

void Cannon_Draw(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    StageObj_Draw2D(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10), (*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x94), (*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0xdc));
    StageObj_Draw2D(t0, (t0 + 0x20c), (t0 + 0x254));
    *(u16 *)((u8 *)t0 + 0x5c) = (*(u16 *)((u8 *)t0 + 0x5c) | 0x100);
    StageObj_Draw2D(t0, (t0 + 0x29c), (t0 + 0x2e4));
    *(u16 *)((u8 *)t0 + 0x5c) = (*(u16 *)((u8 *)t0 + 0x5c) & ~0x100);
}

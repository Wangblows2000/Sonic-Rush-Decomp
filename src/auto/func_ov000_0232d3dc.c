/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gTaskCurrent[];
s32 StageObj_Draw2DEx();
extern u8 gPlayers[];

void func_ov000_0232d3dc(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    if ((*(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x54) & 0x1000) != 0) {
        StageObj_Draw2DEx(0x8000, 0x3000, 0, t0, (t0 + 0x134), 0, 0);
    }
    s32 t1 = *(u32 *)((u8 *)t0 + 0x54);
    if ((*(u32 *)((u8 *)t0 + 0x54) & 0x2000) != 0) {
        if ((t1 & 0x4000) != 0) {
            *(u16 *)((u8 *)t0 + 0x5c) = (*(u16 *)((u8 *)t0 + 0x5c) ^ 1);
            StageObj_Draw2DEx(0x4800, 0x3000, 0, t0, (t0 + 0x1c4), 0, 0);
            *(u16 *)((u8 *)t0 + 0x5c) = (*(u16 *)((u8 *)t0 + 0x5c) ^ 1);
        } else {
            StageObj_Draw2DEx(0xb800, 0x3000, 0, t0, (t0 + 0x1c4), 0, 0);
        }
    }
    s32 t2 = (*(u32 *)((u8 *)*(u32 *)gPlayers + 0x32c) & 0x400);
    if (t2 != 0) {
        *(u32 *)((u8 *)t0 + 0x54) = (*(u32 *)((u8 *)t0 + 0x54) | 4);
    }
}

/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 PaletteTransfer_QueueCopy();

void PlayerSelectMenu_PaletteAnim_Main(void) {
    s32 t0 = *(u32 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t1 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t2 = (*(u32 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) & 3);
    if (t2 != 0) {
        *(u32 *)t1 = (t0 + 0x1);
        return;
    }
    PaletteTransfer_QueueCopy(((t1 + 0xc) + (((u32)((u32)t0 % (u32)0x30) >> 2) << 5)), 0x10, *(u32 *)((u8 *)*(u32 *)((u8 *)t1 + 0x4) + 0x14), *(u32 *)((u8 *)*(u32 *)((u8 *)t1 + 0x4) + 0x18));
    PaletteTransfer_QueueCopy(((t1 + 0x20c) + (((u32)((u32)t0 % (u32)0x30) >> 2) << 5)), 0x10, *(u32 *)((u8 *)*(u32 *)((u8 *)t1 + 0x8) + 0x14), *(u32 *)((u8 *)*(u32 *)((u8 *)t1 + 0x8) + 0x18));
    *(u32 *)t1 = (*(u32 *)t1 + 0x1);
}

#pragma thumb reset

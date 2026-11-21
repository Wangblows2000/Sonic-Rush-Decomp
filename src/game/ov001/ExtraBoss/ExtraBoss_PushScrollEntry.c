/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ExtraBoss_StackPop();
s32 ExtraBoss_RingList_AddPlaced();

void ExtraBoss_PushScrollEntry(s32 p0, s32 p1) {
    s32 t0 = ExtraBoss_StackPop();
    *(u32 *)t0 = p0;
    *(u32 *)((u8 *)t0 + 0x4) = p1;
    *(u32 *)((u8 *)t0 + 0xc) = 0;
    *(u32 *)((u8 *)t0 + 0x8) = *(u32 *)((u8 *)t0 + 0xc);
    *(u32 *)((u8 *)t0 + 0x14) = 0;
    s32 t1 = *(u32 *)((u8 *)t0 + 0x14);
    *(u32 *)((u8 *)t0 + 0x10) = t1;
    *(u32 *)((u8 *)t0 + 0x18) = 2;
    *(u32 *)((u8 *)t0 + 0x1c) = 0;
    ExtraBoss_RingList_AddPlaced(t0, 2, t1, 0);
}

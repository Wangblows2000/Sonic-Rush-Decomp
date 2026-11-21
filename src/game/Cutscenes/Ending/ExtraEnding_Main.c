/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gTaskCurrent[];

void ExtraEnding_Main(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    *(u32 *)t0 = (*(u32 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x1);
    *(u32 *)((u8 *)t0 + 0x8) = (*(u32 *)((u8 *)t0 + 0x8) + 0x1);
    ((s32 (*)())*(u32 *)((u8 *)t0 + 0x4))(t0, *(u32 *)((u8 *)t0 + 0x4));
}

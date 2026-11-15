/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
extern u8 data_0208922c[];
extern u8 data_02087d04[];

void func_ov001_022c8958(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    *(u16 *)((u8 *)data_0208922c + 0xc) = ((s32)*(u32 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) >> 0xc);
    *(u16 *)((u8 *)data_0208922c + 0xe) = ((s32)*(u32 *)((u8 *)t0 + 0x4) >> 0xc);
    *(u16 *)((u8 *)data_02087d04 + 0xc) = ((s32)*(u32 *)t0 >> 0xc);
    *(u16 *)((u8 *)data_02087d04 + 0xe) = ((s32)*(u32 *)((u8 *)t0 + 0x4) >> 0xc);
    *(u32 *)t0 = (*(u32 *)t0 + 0x2000);
}

#pragma thumb reset

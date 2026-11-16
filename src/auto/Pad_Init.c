/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gPad[];
s32 Pad_Reset();
s32 Pad_Update();

void Pad_Init(void) {
    Pad_Reset(gPad, 0, 0);
    u16 t0 = *(volatile u16 *)0x4000130;
    u16 t1 = *(u16 *)0x27fffa8;
    Pad_Update(gPad, ((u32)((((t0 | t1) ^ 0x2fff) & 0x2fff) << 0x10) >> 0x10), ((t0 | t1) ^ 0x2fff), t0);
}

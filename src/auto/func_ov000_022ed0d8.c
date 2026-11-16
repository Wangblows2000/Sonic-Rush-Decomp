/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void func_ov000_022ec6b8();
extern u8 data_ov000_02369ac0[];
void func_ov000_022ecfcc();

void func_ov000_022ed0d8(u8 *p0) {
    *(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x21c) + 0x320) = 0x1000;
    *(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x218) + 0x320) = 0x1000;
    s32 t0 = *(u32 *)((u8 *)p0 + 0x384);
    if (t0 <= 0) {
        *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022ec6b8;
        return;
    }
    *(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x218) + 0x54) = (*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x218) + 0x54) | 8);
    *(u32 *)((u8 *)p0 + 0x218) = 0;
    *(u16 *)((u8 *)data_ov000_02369ac0 + 0x8) = 0;
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022ecfcc;
}

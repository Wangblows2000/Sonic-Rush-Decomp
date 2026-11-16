/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_02069818();
void func_ov000_02316adc();

void func_ov000_02316b5c(u8 *p0) {
    func_02069818(p0, ((u32)(*(u32 *)((u8 *)p0 + 0x8) << 0x10) >> 0x10));
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_02316adc;
    s32 t0 = (*(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x4) & 5);
    if (t0 != 0) {
        *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 0x20);
    }
}

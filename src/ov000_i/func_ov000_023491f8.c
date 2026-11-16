#include <nitro/types.h>

void func_ov000_02349294(void);
s32 func_ov000_0234cb70();
void func_ov000_0234c4a4(u8 *w, s32 a);

void func_ov000_023491f8(u8 *w, s16 vx, s16 vy) {
    func_ov000_0234cb70(w);
    func_ov000_0234c4a4(w, 0xf);
    *(s16 *)(w + 0x2e) = vx;
    *(s16 *)(w + 0x30) = vy;
    *(s16 *)(w + 0x34) = 0;
    if (vx < 0)
        *(u16 *)(w + 0x5c) &= ~1;
    else
        *(u16 *)(w + 0x5c) |= 1;
    *(u32 *)(w + 0x54) |= 2;
    *(u32 *)(w + 0x58) |= 0x10;
    *(u32 *)(w + 0x58) |= 0x8000;
    *(u32 *)(w + 0x58) &= ~0x80000;
    *(u32 *)(w + 0x58) &= ~1;
    *(void **)(w + 0x68) = func_ov000_02349294;
}

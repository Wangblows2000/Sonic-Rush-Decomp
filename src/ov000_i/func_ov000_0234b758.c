#include <nitro/types.h>

void Player_SetAnimation(u8 *w, s32 a);
void func_ov000_0234b63c(u8 *w);
void func_ov000_0234aec8(void);
void func_ov000_02347870(u8 *w);

void func_ov000_0234b758(u8 *w) {
    if (((*(u16 *)(w + 0x5c) & 1) && (*(u16 *)(w + 0x420) & 0x10)) || (!(*(u16 *)(w + 0x5c) & 1) && (*(u16 *)(w + 0x420) & 0x20)))
        Player_SetAnimation(w, 0);
    else
        func_ov000_0234b63c(w);
    *(u32 *)(w + 0x58) &= ~0x10;
    *(void **)(w + 0x68) = func_ov000_0234aec8;
    if (*(s16 *)(w + 0x328) == 0) {
        if (*(u16 *)(w + 0x420) & 0x20)
            *(u32 *)(w + 0x32c) |= 1;
        else if (*(u16 *)(w + 0x420) & 0x10)
            *(u32 *)(w + 0x32c) &= ~1;
    }
    func_ov000_02347870(w);
}

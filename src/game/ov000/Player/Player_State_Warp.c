#include <nitro/types.h>

s16 ObjShift_Set(s32 v, s32 target, s32 a, s32 b, s32 c);
void Player_Respawn(u8 *w);

void Player_State_Warp(u8 *w) {
    (*(s32 *)(w + 0xc))++;
    if (*(s32 *)(w + 0xc) >= 8) {
        *(s16 *)(w + 0x44) = ObjShift_Set(*(s16 *)(w + 0x44), 1, 2, 0x20, 8);
        *(s16 *)(w + 0x46) = ObjShift_Set(*(s16 *)(w + 0x46), 0x400, 2, 0x80, 8);
    }
    if (*(s16 *)(w + 0x44) == 1 && *(s16 *)(w + 0x46) == 0x400)
        *(u16 *)(w + 0x5c) |= 0x20;
    if (*(s32 *)(w + 0xc) < 0x3c)
        return;
    *(u32 *)(w + 0x32c) &= ~0x400;
    *(u32 *)(w + 0x330) |= 0x400000;
    Player_Respawn(w);
    (*(void (**)(u8 *))(w + 0x334))(w);
}

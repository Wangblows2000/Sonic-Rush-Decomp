/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_0206f470[];
s32 MTX_RotX33_();
void Boss1Hammer_State_Fall();

void Boss1Hammer_State_StartFall(u8 *p0) {
    *(u8 *)((u8 *)p0 + 0x316) = 3;
    *(u8 *)((u8 *)p0 + 0x317) = 0;
    MTX_RotX33_(((u8 *)p0 + 0x330), *(s16 *)data_0206f470, *(s16 *)((u8 *)data_0206f470 + 0x2));
    *(u16 *)((u8 *)p0 + 0x29a) = (*(u16 *)((u8 *)p0 + 0x29a) | 4);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)Boss1Hammer_State_Fall;
}

/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Boss1_SetPartAnimation();
void Boss1_State_Unk_ov000_022c75dc();

void Boss1_State_Unk_ov000_022c7630(u8 *p0) {
    Boss1_SetPartAnimation(((u8 *)p0 + 0x228), *(u32 *)((u8 *)p0 + 0x224), 3, 0x1000, 0);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) & ~0x20);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)Boss1_State_Unk_ov000_022c75dc;
}

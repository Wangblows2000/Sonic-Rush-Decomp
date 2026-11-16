/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Boss1_SetPartAnimation();
void Boss1Neck_State_Unk_ov000_022c8ad4();

void Boss1Neck_State_Unk_ov000_022c8af8(u8 *p0) {
    Boss1_SetPartAnimation(((u8 *)p0 + 0x228), *(u32 *)((u8 *)p0 + 0x220), 0x1a, 0x1000, 0);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)Boss1Neck_State_Unk_ov000_022c8ad4;
}

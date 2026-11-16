/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Boss1Neck_Unk_ov000_022c7274();
void Boss1_State_Unk_ov000_022c8d64();

void Boss1Neck_State_Unk_ov000_022c8da0(u8 *p0) {
    if (*(u16 *)((u8 *)p0 + 0x20c) == 0) {
        s32 t0 = Boss1Neck_Unk_ov000_022c7274(9);
        if (t0 == 0) {
            *(u16 *)((u8 *)p0 + 0x20c) = 1;
        }
    }
    s32 t1 = (*(u8 *)((u8 *)p0 + 0x334) & 2);
    if (t1 != 0) {
        *(u32 *)((u8 *)p0 + 0x68) = (u32)Boss1_State_Unk_ov000_022c8d64;
    }
}

/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022cc00c();
void Boss1Neck_State_Unk_ov000_022c8a6c();

void Boss1Neck_State_Unk_ov000_022c8a84(u8 *p0) {
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) & ~0x20);
    func_ov000_022cc00c(((u8 *)p0 + 0x228), *(u32 *)((u8 *)p0 + 0x220), 0x1b, 0x1000, 0);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)Boss1Neck_State_Unk_ov000_022c8a6c;
}

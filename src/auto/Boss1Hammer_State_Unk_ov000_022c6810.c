/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gBoss1[];
s32 func_ov000_022c71d8();
void Boss1Hammer_State_Unk_ov000_022c67ec();

void Boss1Hammer_State_Unk_ov000_022c6810(u8 *p0) {
    *(u16 *)((u8 *)p0 + 0x2c0) = 0;
    func_ov000_022c71d8(*(s16 *)((u8 *)(*(u32 *)gBoss1 + 0x200) + 0x28), gBoss1, 0);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)Boss1Hammer_State_Unk_ov000_022c67ec;
}

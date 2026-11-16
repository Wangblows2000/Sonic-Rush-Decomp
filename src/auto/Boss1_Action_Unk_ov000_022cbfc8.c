/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gBoss1[];
s32 func_ov000_022cb12c();
s32 Boss1Neck_Action_Unk_ov000_022c9da8();
s32 Boss1Hammer_Action_Unk_ov000_022c696c();

void Boss1_Action_Unk_ov000_022cbfc8(void) {
    func_ov000_022cb12c(*(u32 *)gBoss1);
    Boss1Neck_Action_Unk_ov000_022c9da8(*(u32 *)((u8 *)*(u32 *)gBoss1 + 0x210));
    Boss1Hammer_Action_Unk_ov000_022c696c(*(u32 *)((u8 *)*(u32 *)gBoss1 + 0x214));
}

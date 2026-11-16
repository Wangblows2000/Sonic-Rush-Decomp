/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gBoss1[];
s32 Boss1_Action_Unk_ov000_022cb03c();
s32 Boss1Neck_Action_Unk_ov000_022c8d54();
s32 Boss1Hammer_Action_Unk_ov000_022c6924();

void Boss1_Action_Unk_ov000_022cbed8(s32 p0) {
    s32 t0 = (*(u32 *)gBoss1 + 0x200);
    *(u16 *)((u8 *)t0 + 0x28) = p0;
    Boss1_Action_Unk_ov000_022cb03c(*(u32 *)gBoss1, t0, gBoss1);
    Boss1Neck_Action_Unk_ov000_022c8d54(*(u32 *)((u8 *)*(u32 *)gBoss1 + 0x210));
    Boss1Hammer_Action_Unk_ov000_022c6924(*(u32 *)((u8 *)*(u32 *)gBoss1 + 0x214));
}

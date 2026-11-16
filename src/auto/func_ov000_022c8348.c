/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gBoss1[];
void Boss1Neck_State_Unk_ov000_022c82d4();

void func_ov000_022c8348(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gBoss1 + 0x214);
    *(u8 *)((u8 *)p0 + 0x334) = (*(u8 *)((u8 *)p0 + 0x334) & ~2);
    *(u32 *)((u8 *)p0 + 0x33c) = -0x1000;
    *(u8 *)((u8 *)t0 + 0x418) = (*(u8 *)((u8 *)t0 + 0x418) & ~2);
    *(u32 *)((u8 *)t0 + 0x420) = -0x1000;
    *(u32 *)((u8 *)p0 + 0x68) = (u32)Boss1Neck_State_Unk_ov000_022c82d4;
}

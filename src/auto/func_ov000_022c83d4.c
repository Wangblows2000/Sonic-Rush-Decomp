/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gBoss1[];
void Boss1Neck_State_Unk_ov000_022c8390();

void func_ov000_022c83d4(u8 *p0) {
    u32 t0 = *(u32 *)gBoss1;
    u32 t1 = *(u32 *)((u8 *)t0 + 0x214);
    u16 t2 = *(u16 *)((u8 *)(t1 + 0x200) + 0x42);
    *(u16 *)((u8 *)(t1 + 0x200) + 0x42) = (t2 & ~4);
    *(u16 *)((u8 *)p0 + 0x20c) = 0x40;
    *(u16 *)((u8 *)p0 + 0x20e) = 0;
    *(u32 *)((u8 *)p0 + 0x68) = (u32)Boss1Neck_State_Unk_ov000_022c8390;
    u32 t3 = *(u32 *)((u8 *)p0 + 0x68);
    ((s32 (*)())t3)(p0, t3, ((u8 *)p0 + 0x200), (u32)Boss1Neck_State_Unk_ov000_022c8390);
}

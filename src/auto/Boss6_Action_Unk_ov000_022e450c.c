/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gPlayers[];
void Boss6_State_Unk_ov000_022e4474();
s32 Player_SetAnimation();

void Boss6_Action_Unk_ov000_022e450c(s32 p0) {
    u32 t0 = *(u32 *)gPlayers;
    *(u16 *)((u8 *)(t0 + 0x400) + 0x34) = 0;
    *(u16 *)((u8 *)(t0 + 0x400) + 0x36) = 0;
    *(u16 *)((u8 *)(t0 + 0x400) + 0x30) = 0;
    *(u16 *)((u8 *)(t0 + 0x400) + 0x32) = 0;
    *(u16 *)((u8 *)(t0 + 0x400) + 0x2c) = 0;
    *(u16 *)((u8 *)(t0 + 0x400) + 0x2e) = 0;
    *(u16 *)((u8 *)t0 + 0x34) = 0;
    u32 t1 = *(u32 *)((u8 *)t0 + 0x68);
    *(u32 *)((u8 *)p0 + 0x111c) = t1;
    *(u32 *)((u8 *)t0 + 0x68) = (u32)Boss6_State_Unk_ov000_022e4474;
    Player_SetAnimation(t0, 0xf, (u32)Boss6_State_Unk_ov000_022e4474, t0);
}

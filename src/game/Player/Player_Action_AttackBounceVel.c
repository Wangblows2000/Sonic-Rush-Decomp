/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Player_InitState();
void Player_State_InAir();
s32 ObjRect_SetAtkDefFlags();

void Player_Action_AttackBounceVel(u8 *p0, s32 p1) {
    s32 t0 = (*(u32 *)((u8 *)p0 + 0x58) & 0x10);
    s32 t1 = *(u32 *)((u8 *)p0 + 0x58);
    if (t0 != 0) {
        s32 t2 = *(s16 *)((u8 *)p0 + 0x2e);
        s32 t3 = *(s16 *)((u8 *)p0 + 0x34);
        Player_InitState(p0, t0, t1);
        *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
        *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x10);
        *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x8000);
        *(u32 *)((u8 *)p0 + 0x68) = (u32)Player_State_InAir;
        ObjRect_SetAtkDefFlags(((u8 *)p0 + 0x2cc), 0x13, 0, (u32)Player_State_InAir);
        *(u16 *)((u8 *)p0 + 0x30) = p1;
        *(u16 *)((u8 *)p0 + 0x2e) = t2;
        *(u16 *)((u8 *)p0 + 0x34) = t3;
        *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) & ~0xf);
        *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) | 1);
    }
    *(u32 *)((u8 *)p0 + 0xc) = 0;
    *(u32 *)((u8 *)p0 + 0x8) = 0;
    *(u16 *)((u8 *)p0 + 0x32a) = 0;
}

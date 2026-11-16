/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 StageObj_SetAnimation();
void IronBall_State_Unk_ov000_0230cea8();
s32 ObjRect_SetAtkDefFlags();

void IronBall_Action_Unk_ov000_0230d1ac(u8 *p0) {
    StageObj_SetAnimation(p0, 0);
    *(u16 *)((u8 *)p0 + 0x13e) = (*(u16 *)((u8 *)p0 + 0x13e) & ~4);
    *(u16 *)((u8 *)p0 + 0x16a) = (*(u16 *)((u8 *)p0 + 0x16a) & ~4);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)IronBall_State_Unk_ov000_0230cea8;
    *(u32 *)((u8 *)p0 + 0xc) = 0;
    *(u32 *)((u8 *)p0 + 0x8) = 0;
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x2100);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x10);
    s32 t0 = (*(u32 *)((u8 *)p0 + 0x54) | 0x10);
    *(u32 *)((u8 *)p0 + 0x54) = t0;
    ObjRect_SetAtkDefFlags(((u8 *)p0 + 0x160), 0x37, -1, t0);
    *(u32 *)((u8 *)p0 + 0x17c) = 0;
    *(u16 *)((u8 *)p0 + 0x16a) = (*(u16 *)((u8 *)p0 + 0x16a) & ~0x1000);
    *(u16 *)((u8 *)p0 + 0x13e) = (*(u16 *)((u8 *)p0 + 0x13e) | 0x1000);
}

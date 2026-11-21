/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void Rock_State_Wait();
s32 EffShadow_Create();

void Rock_Action_Wait(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) & ~1);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x2000);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x80);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x1500);
    s32 t0 = *(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x9);
    *(u32 *)((u8 *)p0 + 0xc) = t0;
    *(u32 *)((u8 *)p0 + 0x68) = (u32)Rock_State_Wait;
    EffShadow_Create(p0, 0, (u32)Rock_State_Wait, t0);
}

/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Valve_SetSpriteAnimActive();
void Valve_State_Active();

void Valve_Action_Active(u8 *p0) {
    Valve_SetSpriteAnimActive();
    *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) & ~2);
    *(u32 *)((u8 *)p0 + 0xc) = ((u32)((*(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x8) * 0x64 + *(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x9)) << 0x10) >> 0x10);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)Valve_State_Active;
}

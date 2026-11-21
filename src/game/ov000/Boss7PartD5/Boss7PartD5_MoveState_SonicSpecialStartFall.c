/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gBoss7[];
extern u8 data_ov000_02368ba2[];
s32 Boss7_SetCollidersEnabled();
void Boss7PartD5_MoveState_SonicSpecialFall();

void Boss7PartD5_MoveState_SonicSpecialStartFall(u8 *p0) {
    *(u32 *)((u8 *)*(u32 *)gBoss7 + 0x20c) = (*(u32 *)((u8 *)*(u32 *)gBoss7 + 0x20c) & ~0x20);
    *(u16 *)((u8 *)p0 + 0x2e) = 0;
    *(u16 *)((u8 *)p0 + 0x30) = ((s32)(-*(s16 *)((u8 *)data_ov000_02368ba2 + (*(u32 *)((u8 *)p0 + 0x3d0) * 0x30)) + ((u32)-*(s16 *)((u8 *)data_ov000_02368ba2 + (*(u32 *)((u8 *)p0 + 0x3d0) * 0x30)) >> 0x1f)) >> 1);
    s32 t0 = (*(u32 *)((u8 *)p0 + 0x58) | 0x80);
    *(u32 *)((u8 *)p0 + 0x58) = t0;
    Boss7_SetCollidersEnabled(p0, 0, 0, t0);
    *(u32 *)((u8 *)p0 + 0x440) = (u32)Boss7PartD5_MoveState_SonicSpecialFall;
}

/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Boss7_SetRivalAnimation();
s32 Boss7_SetCollidersEnabled();
void Boss7PartD5_MoveState_BlazeSpecialStartHover();

void Boss7PartD5_MoveState_BlazeSpecialRise(u8 *p0) {
    if (*(s16 *)((u8 *)p0 + 0x30) < 0) {
        return;
    }
    *(u16 *)((u8 *)p0 + 0x2e) = 0;
    *(u16 *)((u8 *)p0 + 0x30) = 0;
    s32 t0 = (*(u32 *)((u8 *)p0 + 0x58) & ~0x80);
    *(u32 *)((u8 *)p0 + 0x58) = t0;
    Boss7_SetRivalAnimation(p0, 0x4a, t0);
    s32 t1 = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u16 *)((u8 *)p0 + 0x5c) = t1;
    Boss7_SetCollidersEnabled(p0, 0, 0, t1);
    *(u32 *)((u8 *)p0 + 0x440) = (u32)Boss7PartD5_MoveState_BlazeSpecialStartHover;
}

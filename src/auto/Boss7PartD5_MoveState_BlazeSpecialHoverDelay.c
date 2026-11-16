/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Boss7PartD5_HoverBob();
void Boss7PartD5_MoveState_BlazeSpecialSpawnPillar();

void Boss7PartD5_MoveState_BlazeSpecialHoverDelay(u8 *p0) {
    Boss7PartD5_HoverBob();
    s32 t0 = *(u16 *)((u8 *)p0 + 0x46a);
    *(u16 *)((u8 *)p0 + 0x46a) = (*(u16 *)((u8 *)p0 + 0x46a) + 0x1);
    if ((u32)t0 <= (u32)0x1e) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x440) = (u32)Boss7PartD5_MoveState_BlazeSpecialSpawnPillar;
    ((s32 (*)())*(u32 *)((u8 *)p0 + 0x440))(p0, *(u32 *)((u8 *)p0 + 0x440), t0);
}

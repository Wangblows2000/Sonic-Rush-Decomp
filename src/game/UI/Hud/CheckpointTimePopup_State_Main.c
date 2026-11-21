/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_0236a978[];
extern u8 gEnemyRoomHudTask[];

void CheckpointTimePopup_State_Main(u8 *p0) {
    s32 t0 = *(u32 *)data_ov000_0236a978;
    if (t0 != (s32)p0) {
        *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 4);
    }
    s32 t1 = *(u32 *)gEnemyRoomHudTask;
    if (t1 != 0) {
        *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 4);
    }
    *(u32 *)((u8 *)p0 + 0xc) = (*(u32 *)((u8 *)p0 + 0xc) + 0x1);
    s32 t2 = *(u32 *)((u8 *)p0 + 0xc);
    if (t2 > 0x12c) {
        *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 4);
    }
}

/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void EnemyRoomHud_State_Finish(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 0x3000);
    *(u16 *)((u8 *)p0 + 0x276) = (*(s16 *)((u8 *)p0 + 0x276) + 0x1);
    s32 t0 = *(s16 *)((u8 *)p0 + 0x276);
    if (t0 > 0x80) {
        *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 4);
    }
}

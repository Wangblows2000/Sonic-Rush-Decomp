/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ObjSpd_DownSet();

void GoalRingPart_State_Unk_ov000_0230a7f0(u8 *p0) {
    if ((*(u32 *)((u8 *)p0 + 0x58) & 0x80) == 0) {
        s32 t0 = ObjSpd_DownSet(*(s16 *)((u8 *)p0 + 0x2e), 8);
        *(u16 *)((u8 *)p0 + 0x2e) = t0;
        s32 t1 = ObjSpd_DownSet(*(s16 *)((u8 *)p0 + 0x30), 8);
        *(u16 *)((u8 *)p0 + 0x30) = t1;
    }
    s32 t2 = (*(u16 *)((u8 *)p0 + 0x5c) & 8);
    if (t2 != 0) {
        *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 4);
    }
}

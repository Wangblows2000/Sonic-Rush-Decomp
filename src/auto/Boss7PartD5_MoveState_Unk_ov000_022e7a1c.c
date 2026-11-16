/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Boss7_Unk_ov000_022ee198();
s32 Boss7_Unk_ov000_022e9b70();
void Boss7PartD5_MoveState_Unk_ov000_022e7a00();

void Boss7PartD5_MoveState_Unk_ov000_022e7a1c(u8 *p0) {
    if (*(s16 *)((u8 *)p0 + 0x30) < 0) {
        return;
    }
    *(u16 *)((u8 *)p0 + 0x2e) = 0;
    *(u16 *)((u8 *)p0 + 0x30) = 0;
    s32 t0 = (*(u32 *)((u8 *)p0 + 0x58) & ~0x80);
    *(u32 *)((u8 *)p0 + 0x58) = t0;
    Boss7_Unk_ov000_022ee198(p0, 0x4a, t0);
    s32 t1 = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u16 *)((u8 *)p0 + 0x5c) = t1;
    Boss7_Unk_ov000_022e9b70(p0, 0, 0, t1);
    *(u32 *)((u8 *)p0 + 0x440) = (u32)Boss7PartD5_MoveState_Unk_ov000_022e7a00;
}

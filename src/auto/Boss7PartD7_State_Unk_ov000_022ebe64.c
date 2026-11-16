/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void Boss7PartD7_State_Unk_ov000_022ebe00();

void Boss7PartD7_State_Unk_ov000_022ebe64(u8 *p0) {
    if ((*(u16 *)((u8 *)p0 + 0x5c) & 8) == 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x68) = (u32)Boss7PartD7_State_Unk_ov000_022ebe00;
    ((s32 (*)())*(u32 *)((u8 *)p0 + 0x68))(p0, *(u32 *)((u8 *)p0 + 0x68));
}

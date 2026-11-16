/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void Boss7_State_Unk_ov000_022ebea4();

void Boss7_State_Unk_ov000_022ebf08(u8 *p0) {
    if ((*(u16 *)((u8 *)p0 + 0x5c) & 8) == 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x68) = (u32)Boss7_State_Unk_ov000_022ebea4;
    ((s32 (*)())*(u32 *)((u8 *)p0 + 0x68))(p0, *(u32 *)((u8 *)p0 + 0x68));
}

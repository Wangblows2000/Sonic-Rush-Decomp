/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022f4ce4();
void BossFPartD8_State_Unk_ov000_022f3504();

void BossFPartD8_State_Unk_ov000_022f3544(u8 *p0) {
    *(u8 *)((u8 *)p0 + 0x57c) = (*(u8 *)((u8 *)p0 + 0x57c) | 0x40);
    func_ov000_022f4ce4(p0, 0x10, 0x1000, 0);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)BossFPartD8_State_Unk_ov000_022f3504;
    ((s32 (*)())*(u32 *)((u8 *)p0 + 0x68))(p0, *(u32 *)((u8 *)p0 + 0x68));
}

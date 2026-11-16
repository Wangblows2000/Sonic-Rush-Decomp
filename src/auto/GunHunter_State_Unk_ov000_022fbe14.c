/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 MapCollision_ProbeDistance();

void GunHunter_State_Unk_ov000_022fbe14(u8 *p0) {
    s32 t0 = MapCollision_ProbeDistance(((s32)*(u32 *)((u8 *)p0 + 0x10) >> 8), ((s32)*(u32 *)((u8 *)p0 + 0x14) >> 8), 1, 2, 0, 0);
    if (t0 >= 0) {
        if ((*(u16 *)((u8 *)p0 + 0x16a) & 0x200) == 0) {
            return;
        }
    }
    *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 4);
}

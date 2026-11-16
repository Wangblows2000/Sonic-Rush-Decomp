/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 StageObj_SetOAMOrder();
s32 EggBombHawk_Explode();

void EggBombHawk_State_Unk_ov000_022ff770(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x8);
    if (t0 != 0) {
        *(u32 *)((u8 *)p0 + 0x8) = (t0 - 0x1);
        if (*(u32 *)((u8 *)p0 + 0x8) == 0) {
            StageObj_SetOAMOrder(p0, 0xa);
        }
    }
    if ((*(u32 *)((u8 *)p0 + 0x58) & 0x100) != 0) {
        return;
    }
    s32 t1 = *(u32 *)((u8 *)p0 + 0x58);
    if ((*(u32 *)((u8 *)p0 + 0x58) & 0xf) == 0) {
        if ((*(u16 *)((u8 *)p0 + 0x16a) & 0x200) == 0) {
            return;
        }
    }
    EggBombHawk_Explode(p0, t1);
}

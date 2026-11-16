/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 StageObj_SetOAMOrder();

void EggBishop_State_Unk_ov000_022fc038(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0xc);
    if (t0 == 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0xc) = (t0 - 0x1);
    if (*(u32 *)((u8 *)p0 + 0xc) != 0) {
        return;
    }
    StageObj_SetOAMOrder(p0, 0xa);
}

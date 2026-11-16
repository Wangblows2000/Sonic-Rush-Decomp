/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 StageObj_SetOAMOrder();
s32 EffZakoBombPuff_Create();

void Enemy_Unk_ov000_022fc398(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0xc);
    if (t0 != 0) {
        *(u32 *)((u8 *)p0 + 0xc) = (t0 - 0x1);
        if (*(u32 *)((u8 *)p0 + 0xc) == 0) {
            StageObj_SetOAMOrder(p0, 0xa);
        }
    }
    if ((*(u32 *)((u8 *)p0 + 0x58) & 0x100) != 0) {
        return;
    }
    if ((*(u32 *)((u8 *)p0 + 0x58) & 0xf) == 0) {
        if ((*(u16 *)((u8 *)p0 + 0x16a) & 0x200) == 0) {
            return;
        }
    }
    EffZakoBombPuff_Create(p0, 0, 0);
    *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 4);
}

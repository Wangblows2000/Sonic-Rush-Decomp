/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Sound_PlaySeqArcEx();
extern u8 gPlayers[];
s32 Sound_SetPositional();
s32 func_ov000_022fc5b8();
s32 func_ov000_022fe7e8();

void EnemyPawnGroup_State_Unk_ov000_022fd7d8(u8 *p0) {
    if (((s32)*(s16 *)((u8 *)p0 + 0xa2) >> 4) == 6) {
        if ((*(u32 *)((u8 *)p0 + 0x1f4) & 4) == 0) {
            Sound_PlaySeqArcEx(*(u32 *)((u8 *)p0 + 0x1dc), 0, 0x78);
            Sound_SetPositional(*(u32 *)((u8 *)p0 + 0x1dc), p0, *(u32 *)gPlayers);
            func_ov000_022fc5b8(p0, 0xdf, 0x10, -32);
        }
        *(u32 *)((u8 *)p0 + 0x1f4) = (*(u32 *)((u8 *)p0 + 0x1f4) | 4);
    }
    if ((*(u16 *)((u8 *)p0 + 0x5c) & 0x10) != 0) {
        s32 t0 = *(u32 *)((u8 *)p0 + 0xc);
        if (t0 != 0) {
            *(u32 *)((u8 *)p0 + 0xc) = (t0 - 0x1);
            if (*(u32 *)((u8 *)p0 + 0xc) == 0) {
                *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) & ~0x10);
                *(u32 *)((u8 *)p0 + 0x1f4) = (*(u32 *)((u8 *)p0 + 0x1f4) | 1);
                *(u32 *)((u8 *)p0 + 0xc) = 0x20;
            }
        }
    }
    if ((*(u16 *)((u8 *)p0 + 0x5c) & 8) == 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0xc) = (*(u32 *)((u8 *)p0 + 0xc) - 0x1);
    if (*(u32 *)((u8 *)p0 + 0xc) != 0) {
        return;
    }
    s32 t1 = (*(u32 *)((u8 *)p0 + 0x1f4) & ~5);
    *(u32 *)((u8 *)p0 + 0x1f4) = t1;
    func_ov000_022fe7e8(p0, t1);
}

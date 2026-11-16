/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022c4560[];
s32 func_ov000_022f8c88();
s32 func_ov000_022f898c();

void BossFPartD8_State_Unk_ov000_022ef908(u8 *p0) {
    *(u16 *)((u8 *)p0 + 0x434) = (*(u16 *)((u8 *)p0 + 0x434) + 0x1);
    s32 t0 = *(u16 *)((u8 *)p0 + 0x434);
    if ((*(u16 *)((u8 *)p0 + 0x434) & 1) == 0) {
        if ((t0 & 0x8000) != 0) {
            func_ov000_022f8c88(*(u32 *)data_022c4560, 0);
        } else {
            func_ov000_022f8c88(*(u32 *)data_022c4560, 1);
        }
        *(u16 *)((u8 *)p0 + 0x434) = (*(u16 *)((u8 *)p0 + 0x434) & ~1);
        *(u16 *)((u8 *)p0 + 0x434) = (*(u16 *)((u8 *)p0 + 0x434) ^ 0x8000);
    }
    *(u32 *)((u8 *)p0 + 0x18) = (*(u32 *)((u8 *)p0 + 0x18) - 0x200);
    s32 t1 = *(u32 *)((u8 *)p0 + 0x18);
    if (t1 < -0x1f400) {
        *(u32 *)((u8 *)p0 + 0x18) = -0x1f400;
    }
    if ((*(u8 *)((u8 *)p0 + 0x57c) & 2) == 0) {
        return;
    }
    if ((*(u16 *)((u8 *)p0 + 0x434) & 0x8000) != 0) {
        return;
    }
    func_ov000_022f898c((*(u16 *)((u8 *)p0 + 0x434) & 0x8000), t1);
}

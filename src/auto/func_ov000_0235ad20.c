/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022b4574[];
s32 func_0206b768();
s32 func_020371c8();

void func_ov000_0235ad20(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    func_0206b768(*(u32 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10), *(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x4), 0, 0, (*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x8), (*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x50), 0);
    if ((*(u32 *)((u8 *)t0 + 0x34) & 2) == 0) {
        return;
    }
    func_020371c8(*(u32 *)data_022b4574);
}

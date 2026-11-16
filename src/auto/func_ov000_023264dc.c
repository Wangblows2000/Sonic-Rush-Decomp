/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022b4574[];
s32 func_02069090();
s32 func_02067d58();

void func_ov000_023264dc(void) {
    if ((*(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x54) & 0xc) != 0) {
        return;
    }
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    if (*(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x14c) != 0) {
        func_02069090(t0, (t0 + 0x134));
    }
    if (*(u32 *)((u8 *)t0 + 0x160) == 0) {
        return;
    }
    func_02067d58((t0 + 0x160));
}

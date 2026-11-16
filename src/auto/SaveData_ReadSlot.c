/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gSaveState[];
s32 func_0200f24c();
s32 func_0200fa70();
s32 func_0200f23c();

s32 SaveData_ReadSlot(s32 p0, s32 p1) {
    s32 t0 = *(u16 *)gSaveState;
    func_0200f24c(*(u16 *)gSaveState, p1, gSaveState);
    s32 t1 = func_0200fa70((((s32)p0 * 0x320) + 0xc), p1, 0x314, 0, 0, 0);
    func_0200f23c(t0);
    return (t1 != 0);
}

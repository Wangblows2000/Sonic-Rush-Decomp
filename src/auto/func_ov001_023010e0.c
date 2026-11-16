/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gExtraBossPlayerManager[];
s32 func_ov001_02301118();

void func_ov001_023010e0(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gExtraBossPlayerManager + 0x4);
    if (t0 == 0) {
        return;
    }
    func_ov001_02301118(t0);
}

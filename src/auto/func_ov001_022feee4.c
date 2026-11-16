/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gExtraBossWork[];
s32 func_ov001_022fd744();
s32 func_ov001_02300ca8();
s32 func_ov001_02304028();

void func_ov001_022feee4(void) {
    s32 t0 = (*(u32 *)*(u32 *)gExtraBossWork & 4);
    if (t0 != 0) {
        func_ov001_022fd744(t0);
    }
    func_ov001_02300ca8();
    func_ov001_02304028();
}

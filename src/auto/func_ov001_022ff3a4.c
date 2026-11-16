/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov001_0231f52c[];

void func_ov001_022ff3a4(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_ov001_0231f52c + 0xd4);
    if (t0 == 0) {
        return;
    }
    ((s32 (*)())t0)(t0);
}

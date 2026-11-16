/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022b4578[];
extern u8 data_022b458c[];
s32 func_02006abc();

s32 func_02036e9c(void) {
    s32 t0 = *(u32 *)data_022b4578;
    if (t0 < 0x100) {
        *(u32 *)data_022b4578 = (t0 + 0x1);
        return *(u32 *)((u8 *)data_022b458c + (t0 << 2));
    }
    s32 t1 = func_02006abc(0, data_022b4578);
    return t1;
}

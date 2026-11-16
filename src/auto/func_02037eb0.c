/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022b65dc[];
extern u8 data_022b65e0[];
s32 func_02006abc();

s32 func_02037eb0(void) {
    s32 t0 = *(u16 *)data_022b65dc;
    if ((u32)t0 < (u32)0x100) {
        *(u16 *)data_022b65dc = (t0 + 0x1);
        return *(u32 *)((u8 *)data_022b65e0 + (t0 << 2));
    }
    s32 t1 = func_02006abc(0, data_022b65dc);
    return t1;
}

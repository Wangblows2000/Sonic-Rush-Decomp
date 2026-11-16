/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022c1990[];
extern u8 data_022c1a00[];
extern u8 data_022c1964[];
extern u8 data_022c1970[];
extern u8 data_022c195c[];
extern u8 data_022c1984[];
extern u8 data_022c1978[];
extern u8 data_022c1944[];
s32 func_0203e3f4();

s32 func_0203e480(void) {
    *(u32 *)data_022c1990 = 0;
    *(u32 *)data_022c1964 = 0;
    *(u32 *)data_022c1970 = 0;
    *(u16 *)data_022c195c = 0;
    *(u16 *)data_022c1944 = 1;
    *(u32 *)data_022c1984 = 0;
    *(u32 *)data_022c1a00 = 0;
    *(u16 *)((u8 *)data_022c1a00 + 0x4) = 0;
    *(u32 *)data_022c1978 = 0;
    s32 t0 = func_0203e3f4(data_022c1978, data_022c1a00, data_022c1984, data_022c1944);
    return (t0 != 0);
}

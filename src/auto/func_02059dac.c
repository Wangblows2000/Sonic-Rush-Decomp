/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_02050018();

void func_02059dac(u8 *p0) {
    s32 t0 = func_02050018();
    if (t0 != 0) {
        *(u32 *)((u8 *)p0 + 0x8) = (*(u32 *)((u8 *)p0 + 0x8) | 0x80000000);
    }
}

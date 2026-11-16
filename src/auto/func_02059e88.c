/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_02050018();
void func_02059e2c();

void func_02059e88(u8 *p0) {
    s32 t0 = func_02050018();
    if (t0 != 0) {
        *(u32 *)p0 = (u32)func_02059e2c;
    }
}

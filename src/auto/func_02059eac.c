/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_02059340();
s32 func_0205004c();
void func_02059e88();
extern u8 data_022b4574[];
s32 func_020371c8();
s32 func_0205a304();
s32 func_0205a27c();

void func_02059eac(u8 *p0) {
    s32 t0 = func_02059340();
    if (t0 == 0) {
        func_0205004c();
        *(u32 *)p0 = (u32)func_02059e88;
        return;
    }
    func_020371c8(*(u32 *)data_022b4574);
    func_0205a304();
    func_0205a27c(2, 0);
}

/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_0205943c();
s32 func_0205004c();
void func_02059f80();

void func_02059fa4(u8 *p0) {
    s32 t0 = func_0205943c();
    s32 v1 = t0;
    if (t0 != 0) {
        s32 t2 = (*(u32 *)((u8 *)p0 + 0x8) | 1);
        *(u32 *)((u8 *)p0 + 0x8) = t2;
        v1 = t2;
    }
    func_0205004c(v1);
    *(u32 *)p0 = (u32)func_02059f80;
}

/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_0201bf1c();
s32 Sound_FreeHandle();
extern u8 data_ov000_0236ac34[];

void func_ov000_02342a34(u8 *p0) {
    s32 t0 = *(u32 *)*(u32 *)((u8 *)p0 + 0x10);
    s32 t1 = *(u32 *)((u8 *)p0 + 0x10);
    if (t0 != 0) {
        func_0201bf1c(t0);
    }
    s32 t2 = *(u32 *)t1;
    if (t2 != 0) {
        Sound_FreeHandle(t2);
    }
    *(u32 *)data_ov000_0236ac34 = 0;
}

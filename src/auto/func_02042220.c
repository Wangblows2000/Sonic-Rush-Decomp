/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_0203e934();
s32 func_0203e5f8();
void func_02041264();
s32 func_02042774();
s32 func_0203e10c();
void func_020421d4();

void func_02042220(u8 *p0) {
    s32 t0 = func_0203e934();
    if (t0 != 7) {
        return;
    }
    *(u16 *)((u8 *)p0 + 0x10) = 0;
    s32 t1 = *(u16 *)((u8 *)p0 + 0x10);
    if (t1 == 0) {
        s32 t2 = func_0203e5f8(t1);
        *(u16 *)((u8 *)p0 + 0x10) = t2;
    }
    s32 t3 = *(u32 *)((u8 *)p0 + 0x4);
    if (t3 == 3) {
        *(u32 *)p0 = (u32)func_02041264;
    } else {
        s32 t4 = func_02042774(*(u8 *)((u8 *)p0 + 0x16));
        *(u16 *)((u8 *)p0 + 0x12) = t4;
        func_0203e10c(4, *(u16 *)((u8 *)p0 + 0x12), *(u16 *)((u8 *)p0 + 0x10));
        *(u32 *)p0 = (u32)func_020421d4;
    }
    *(u16 *)((u8 *)p0 + 0x1a) = 0;
}

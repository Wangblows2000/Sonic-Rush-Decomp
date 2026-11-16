/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_02031a68();
s32 func_02033550();

void func_02069818(u8 *p0, s32 p1) {
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) & ~8);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) & ~4);
    s32 t0 = *(u32 *)((u8 *)p0 + 0x124);
    if (t0 != 0) {
        func_02031a68(t0, 0, *(u32 *)((u8 *)t0 + 0x13c), p1, 0);
    }
    s32 t1 = *(u32 *)((u8 *)p0 + 0x12c);
    if (t1 != 0) {
        func_02033550((t1 + 0x90), p1);
    }
    if (*(u32 *)((u8 *)p0 + 0xa4) != 0) {
        func_02033550(((u8 *)p0 + 0x94), p1);
    }
    if (*(u32 *)((u8 *)p0 + 0xec) == 0) {
        return;
    }
    func_02033550(((u8 *)p0 + 0xdc), p1);
}

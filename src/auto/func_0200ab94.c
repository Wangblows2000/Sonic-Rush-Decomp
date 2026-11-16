/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void func_0200ab94(u8 *p0) {
    u32 t0 = *(u32 *)((u8 *)p0 + 0x28);
    u32 t1 = *(u32 *)((u8 *)p0 + 0x34);
    u32 t2 = *(u32 *)((u8 *)p0 + 0x8);
    u32 t3 = *(u32 *)((u8 *)p0 + 0x2c);
    *(u32 *)((u8 *)p0 + 0x28) = (t0 + t1);
    u32 t4 = *(u32 *)((u8 *)t2 + 0x3c);
    ((s32 (*)())t4)(t2, t3, t0, t1);
}

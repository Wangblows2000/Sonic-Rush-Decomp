/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_02369ac0[];
void func_ov000_022ea798();
void func_ov000_022ea190();

s32 func_ov000_022ea1b8(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)data_ov000_02369ac0 + 0x4);
    if (t0 != 0x27) {
        *(u32 *)((u8 *)p0 + 0x43c) = (u32)func_ov000_022ea798;
        return 0;
    }
    *(u32 *)((u8 *)p0 + 0x43c) = (u32)func_ov000_022ea190;
    s32 t1 = ((s32 (*)())*(u32 *)((u8 *)p0 + 0x43c))(p0, *(u32 *)((u8 *)p0 + 0x43c));
    return t1;
}

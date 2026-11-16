/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BrightnessFade_IsDone();
void func_ov001_022de04c();

void func_ov001_022de0b4(u8 *p0, u8 *p1) {
    s32 t0 = BrightnessFade_IsDone();
    if (t0 != 0) {
        *(u32 *)((u8 *)p0 + 0x4) = (*(u32 *)((u8 *)p0 + 0x4) & ~0x20);
        *(u32 *)p1 = (u32)func_ov001_022de04c;
    }
}

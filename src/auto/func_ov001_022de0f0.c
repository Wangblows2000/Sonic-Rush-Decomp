/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BrightnessFade_Create();
void func_ov001_022de0b4();

s32 func_ov001_022de0f0(u8 *p0, u8 *p1) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x4);
    if ((*(u32 *)((u8 *)p0 + 0x4) & 0x10) == 0) {
        return t0;
    }
    *(u32 *)((u8 *)p0 + 0x4) = (t0 & ~0x10);
    BrightnessFade_Create(2, 0x88);
    *(u32 *)((u8 *)p0 + 0x4) = (*(u32 *)((u8 *)p0 + 0x4) | 0x20);
    *(u32 *)p1 = (u32)func_ov001_022de0b4;
    return (s32)(u32)func_ov001_022de0b4;
}

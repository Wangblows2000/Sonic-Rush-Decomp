/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_0236ac60[];
s32 func_02036754();
void func_ov000_022ed5ac();

void func_ov000_022ed658(u8 *p0) {
    *(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x218) + 0x320) = 0x3000;
    *(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x21c) + 0x320) = 0x3000;
    if (*(u8 *)((u8 *)*(u32 *)data_ov000_0236ac60 + 0x324) == 0) {
        func_02036754(0, 0, 9);
    } else {
        func_02036754(0, 0, 0x16);
    }
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022ed5ac;
}

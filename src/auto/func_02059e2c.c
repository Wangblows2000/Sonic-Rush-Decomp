/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 MsgBox_Create();
void func_02059e08();

void func_02059e2c(u8 *p0) {
    MsgBox_Create(0x18, 3, 5, 0, 1, 1, 0, 0x100010);
    *(u16 *)((u8 *)p0 + 0x10) = 0xb4;
    *(u32 *)p0 = (u32)func_02059e08;
}

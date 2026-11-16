/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void func_ov000_02340034();
void func_ov000_023400ac();
s32 Task_Create();

void func_ov000_02341264(void) {
    Task_Create((u32)func_ov000_02340034, (u32)func_ov000_023400ac, 0, 0x3f10, 3, 0);
}

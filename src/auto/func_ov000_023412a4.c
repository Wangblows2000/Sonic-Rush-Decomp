/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void func_ov000_02340980();
void func_ov000_023409f8();
s32 Task_Create();

void func_ov000_023412a4(void) {
    Task_Create((u32)func_ov000_02340980, (u32)func_ov000_023409f8, 0, 0x3f10, 3, 0);
}

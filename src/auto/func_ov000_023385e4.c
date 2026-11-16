/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_0236aaac[];
s32 Task_Destroy();

void func_ov000_023385e4(void) {
    s32 t0 = *(u32 *)data_ov000_0236aaac;
    if (t0 == 0) {
        return;
    }
    Task_Destroy(t0);
    *(u32 *)data_ov000_0236aaac = 0;
}

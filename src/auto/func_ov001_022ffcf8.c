/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov001_0230007c();
s32 func_ov001_02301058();
extern u8 data_ov001_0231f52c[];

void func_ov001_022ffcf8(void) {
    s32 t0 = func_ov001_0230007c();
    func_ov001_02301058();
    s32 t1 = (*(u32 *)*(u32 *)data_ov001_0231f52c | 4);
    *(u32 *)*(u32 *)data_ov001_0231f52c = t1;
}

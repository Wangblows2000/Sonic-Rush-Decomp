/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov001_0231f5bc[];
s32 func_02032d2c();

void func_ov001_02308600(void) {
    s32 t0 = *(u32 *)data_ov001_0231f5bc;
    func_02032d2c((*(u32 *)data_ov001_0231f5bc + 0x270));
    func_02032d2c((t0 + 0x2b8));
    func_02032d2c((t0 + 0x300));
}

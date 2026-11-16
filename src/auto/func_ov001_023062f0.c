/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov001_0231f5a8[];
s32 func_ov001_022ffce8();
s32 func_02032d2c();

void func_ov001_023062f0(void) {
    s32 t0 = (*(u32 *)*(u32 *)data_ov001_0231f5a8 & 1);
    if (t0 != 0) {
        s32 t1 = func_ov001_022ffce8(t0);
        s32 t2 = *(u32 *)((u8 *)t1 + 0x3c);
        func_02032d2c((*(u32 *)((u8 *)t1 + 0x3c) * 0x48 + (*(u32 *)data_ov001_0231f5a8 + 0x98)), (*(u32 *)data_ov001_0231f5a8 + 0x98));
        func_02032d2c(((t2 + 0x2) * 0x48 + (*(u32 *)data_ov001_0231f5a8 + 0x98)), (t2 + 0x2), (*(u32 *)data_ov001_0231f5a8 + 0x98));
    }
    if ((*(u32 *)*(u32 *)data_ov001_0231f5a8 & 2) == 0) {
        return;
    }
    func_02032d2c((*(u32 *)data_ov001_0231f5a8 + 0x1b8), *(u32 *)data_ov001_0231f5a8);
    func_02032d2c((*(u32 *)data_ov001_0231f5a8 + 0x200));
    func_02032d2c((*(u32 *)data_ov001_0231f5a8 + 0x248));
    func_02032d2c((*(u32 *)data_ov001_0231f5a8 + 0x290));
}

/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov001_0231c394[];
s32 func_02036854();
extern u8 data_02250458[];
s32 func_0201e7cc();
s32 func_0201bf34();

void func_ov001_022ddfc4(s32 p0, s32 p1) {
    func_02036854(data_ov001_0231c394);
    func_0201e7cc(0xf, *(u32 *)data_02250458);
    s32 t0 = func_0201bf34(((u8 *)p1 + 0x4));
}

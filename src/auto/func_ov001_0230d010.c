/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov001_0231f654[];
s32 func_ov001_0230d650();
s32 func_ov001_0230d528();

void func_ov001_0230d010(void) {
    s32 t0 = *(u32 *)data_ov001_0231f654;
    s32 t1 = func_ov001_0230d650(0xbf000, *(u32 *)*(u32 *)data_ov001_0231f654);
    func_ov001_0230d528((t0 + 0x8), -0xbf000, t1);
}

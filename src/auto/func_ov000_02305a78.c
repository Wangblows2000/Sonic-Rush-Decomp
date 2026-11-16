/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_02369e9c[];
s32 func_020686dc();
extern u8 data_ov000_02369ea4[];
s32 func_020683e4();
extern u8 data_ov000_02369eac[];
s32 func_0206d3fc();
s32 func_ov000_022fb9c8();

void func_ov000_02305a78(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    func_020686dc(data_ov000_02369e9c);
    func_020683e4(data_ov000_02369ea4, 0);
    func_020683e4(data_ov000_02369eac, 1);
    func_0206d3fc((*(u16 *)((u8 *)(t0 + 0x900) + 0xe0) & 0xff));
    func_ov000_022fb9c8(p0);
}

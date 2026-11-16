/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_0236a2b4[];
s32 func_020686dc();
extern u8 data_ov000_0236a2a4[];
s32 func_ov000_022fb9c8();

void func_ov000_02319390(u8 *p0) {
    if (*(u16 *)((u8 *)*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x10) + 0x1e0) + 0x2) == 0xbe) {
        func_020686dc(data_ov000_0236a2b4);
    } else {
        func_020686dc(data_ov000_0236a2a4);
    }
    func_ov000_022fb9c8(p0);
}

/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_0236a978[];
s32 func_0206bef4();

void func_ov000_0232b3a4(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    *(u32 *)((u8 *)t0 + 0xc8) = 0;
    *(u32 *)((u8 *)t0 + 0x110) = 0;
    if (*(u32 *)data_ov000_0236a978 == t0) {
        *(u32 *)data_ov000_0236a978 = 0;
    }
    func_0206bef4(p0);
}

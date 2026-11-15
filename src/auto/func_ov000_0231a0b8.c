/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_0201c070();
extern u8 data_ov000_0236a2fc[];
s32 func_ov000_022fb9c8();

void func_ov000_0231a0b8(u8 *p0) {
    func_0201c070(*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x10) + 0x1dc), 0);
    *(u32 *)data_ov000_0236a2fc = 0;
    s32 t0 = func_ov000_022fb9c8(p0, data_ov000_0236a2fc, 0);
}

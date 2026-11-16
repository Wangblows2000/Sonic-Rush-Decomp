/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gCamera[];
s32 func_ov000_02339110();
s32 func_ov000_02339478();

void func_ov000_02339bdc(s32 p0, s32 p1) {
    s32 t0 = (*(u32 *)((u8 *)gCamera + ((s32)p1 * 0x64)) & 0x4010);
    s32 t1 = *(u32 *)((u8 *)gCamera + ((s32)p1 * 0x64));
    if (t0 == 0) {
        func_ov000_02339110(p0, p1, t0, t1);
    }
    func_ov000_02339478(p0, p1);
}

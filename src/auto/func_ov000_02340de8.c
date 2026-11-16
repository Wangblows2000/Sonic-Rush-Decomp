/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gCamera[];
s32 func_ov000_02340154();
s32 func_ov000_023400b0();

void func_ov000_02340de8(u8 *p0) {
    s32 t0 = (*(u32 *)p0 + 0x1);
    *(u32 *)p0 = t0;
    if ((*(u32 *)((u8 *)gCamera + 0xc8) & 1) != 0) {
        func_ov000_02340154(p0, 0, t0);
        func_ov000_023400b0(p0);
        return;
    }
    func_ov000_02340154(p0, 0);
    func_ov000_02340154(p0, 1);
}

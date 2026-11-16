/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gCamera[];
s32 func_ov000_02334eb4();

void func_ov000_0233545c(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)gCamera + 0x4);
    if (t0 == *(u32 *)((u8 *)p0 + 0xc)) {
        if (*(u32 *)((u8 *)gCamera + 0x8) == *(u32 *)((u8 *)p0 + 0x10)) {
            return;
        }
    }
    func_ov000_02334eb4(p0, 0, 5, t0);
}

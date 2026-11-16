/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gCamera[];
s32 func_ov000_0233f104();
s32 func_ov000_0233f02c();

void func_ov000_02340b94(s32 p0) {
    if ((*(u32 *)((u8 *)gCamera + 0xc8) & 1) != 0) {
        func_ov000_0233f104(p0, 0);
        func_ov000_0233f02c(p0);
        return;
    }
    func_ov000_0233f104(p0, 0);
    func_ov000_0233f104(p0, 1);
}

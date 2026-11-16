/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_0233219c();
s32 func_0204b7ec();

void func_ov000_02346d30(u8 *p0) {
    if ((*(u32 *)((u8 *)p0 + 0x32c) & 0x1000000) != 0) {
        return;
    }
    if ((*(u32 *)((u8 *)p0 + 0x32c) & 0x100) != 0) {
        return;
    }
    *(u8 *)((u8 *)p0 + 0x3cd) = 0;
    if ((*(u32 *)((u8 *)p0 + 0x32c) & 0x80) == 0) {
        func_ov000_0233219c(p0, 0, 0);
    }
    *(u8 *)((u8 *)p0 + 0x3d1) = 8;
    *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) | 0x100);
    *(u16 *)((u8 *)p0 + 0x3ac) = 0x60;
    if ((*(u32 *)((u8 *)p0 + 0x32c) & 0x80) != 0) {
        return;
    }
    if (*(u8 *)((u8 *)p0 + 0x324) != 1) {
        func_0204b7ec(((u8 *)p0 + 0x384), 0, 9);
        return;
    }
    func_0204b7ec(((u8 *)p0 + 0x384), 0, 0x16);
}

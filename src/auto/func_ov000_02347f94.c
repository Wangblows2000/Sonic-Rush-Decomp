/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_0204b7ec();
s32 func_ov000_0234c4a4();
void func_ov000_02347ec0();

void func_ov000_02347f94(u8 *p0) {
    if (*(u32 *)((u8 *)p0 + 0x3e8) != 0) {
        *(u16 *)((u8 *)p0 + 0x3ca) = 0;
        func_0204b7ec(((u8 *)p0 + 0x384), 0, 0x53, ((u8 *)p0 + 0x300));
        return;
    }
    func_0204b7ec(((u8 *)p0 + 0x384), 0, 0x53);
    func_ov000_0234c4a4(p0, 0x23);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_02347ec0;
    *(u16 *)((u8 *)p0 + 0x30) = 0;
    *(u16 *)((u8 *)p0 + 0x3ca) = 0;
    *(u32 *)((u8 *)p0 + 0xc) = 4;
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x2000);
}

/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022faf48();
void func_ov000_022fcd50();
extern u8 data_022c4680[];
extern u8 data_022c4560[];

void func_ov000_022fcef4(u8 *p0) {
    func_ov000_022faf48(p0, 3);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x10);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022fcd50;
    *(u16 *)((u8 *)p0 + 0x34) = 0;
    *(u32 *)((u8 *)p0 + 0xc) = 0x30;
    *(u32 *)((u8 *)p0 + 0x8) = 0x78;
    if ((*(u32 *)((u8 *)data_022c4680 + 0x8) & 1) == 0) {
        return;
    }
    if (*(u32 *)((u8 *)data_022c4560 + 0x10) == 2) {
        return;
    }
    s32 t0 = *(u32 *)((u8 *)data_022c4560 + 0x10);
    if (t0 != 3) {
        *(u32 *)((u8 *)p0 + 0xc) = (*(u32 *)((u8 *)p0 + 0xc) + ((s32)*(u32 *)((u8 *)p0 + 0xc) >> 1));
    }
}

/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_020657dc();
s32 func_020648c4();

void func_02065ee0(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x400000);
    s32 t0 = (*(u32 *)((u8 *)p0 + 0x58) & 1);
    if (t0 != 0) {
        *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x400000);
    }
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0xf);
    *(u16 *)((u8 *)p0 + 0x5e) = 0;
    s32 t1 = *(s16 *)((u8 *)p0 + 0x28);
    if ((*(u32 *)((u8 *)p0 + 0x58) & 0x400) == 0) {
        func_020657dc(p0, 0, t1);
    }
    if ((*(u32 *)((u8 *)p0 + 0x58) & 0x800) == 0) {
        func_020648c4(p0);
    }
    s32 t2 = (*(u32 *)((u8 *)p0 + 0x58) & 1);
    if (t2 != 0) {
        *(u32 *)((u8 *)p0 + 0x14) = (*(u32 *)((u8 *)p0 + 0x14) & ~0xff);
    }
    if ((*(u32 *)((u8 *)p0 + 0x58) & 0x400) != 0) {
        return;
    }
    func_020657dc(p0, 1, t1);
}

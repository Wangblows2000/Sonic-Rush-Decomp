/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov001_022fe550();
s32 func_ov001_022fde54();

void func_ov001_022fe6b8(u8 *p0, u8 *p1) {
    s32 t0 = *(u16 *)((u8 *)p0 + 0x24);
    if (((u32)((*(u16 *)((u8 *)p0 + 0x24) + *(u16 *)((u8 *)p0 + 0x34)) << 0x10) >> 0x10) == 0) {
        if (*(u16 *)((u8 *)p1 + 0x24) == 0) {
            s32 t1 = *(u16 *)((u8 *)p1 + 0x34);
            if (t1 == 0) {
                func_ov001_022fe550(p0, p1, t1, t0);
                return;
            }
        }
    }
    func_ov001_022fde54();
}

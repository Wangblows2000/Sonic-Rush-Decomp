/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_0235b804();
s32 func_0204b7ec();
extern u8 data_ov000_0236ac60[];
s32 func_02068204();

s32 func_ov000_02313500(u8 *p0) {
    s32 t0 = func_ov000_0235b804();
    if (t0 != 0) {
        if ((*(u16 *)((u8 *)p0 + 0x5c) & 0x10) != 0) {
            func_0204b7ec(0, 0, 0x65);
        }
        func_02068204(*(u32 *)((u8 *)p0 + 0x1dc), p0, *(u32 *)data_ov000_0236ac60);
        s32 t1 = (*(u16 *)((u8 *)p0 + 0x5c) & ~0x10);
        *(u16 *)((u8 *)p0 + 0x5c) = t1;
        return t1;
    }
    s32 t2 = *(u16 *)((u8 *)p0 + 0x5c);
    s32 v3 = t2;
    if ((*(u16 *)((u8 *)p0 + 0x5c) & 8) != 0) {
        *(u16 *)((u8 *)p0 + 0x5c) = (v3 | 0x10);
        v3 = (v3 | 0x10);
    }
    return v3;
}

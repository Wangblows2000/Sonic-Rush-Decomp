/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_0208922c[];
void func_ov000_022ca8cc();

void func_ov000_022ca9cc(u8 *p0) {
    s32 t0 = *(s16 *)((u8 *)data_0208922c + 0x48);
    if (t0 > 0) {
        *(u16 *)((u8 *)data_0208922c + 0x48) = (t0 - 0x2);
    }
    s32 t1 = *(u16 *)((u8 *)p0 + 0x220);
    *(u16 *)((u8 *)p0 + 0x220) = (*(u16 *)((u8 *)p0 + 0x220) + 0x1);
    if ((u32)t1 > (u32)0x1e) {
        *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022ca8cc;
    }
}

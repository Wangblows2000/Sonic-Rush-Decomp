/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_02369b2c[];
extern u8 data_0208922c[];
void func_ov000_022dd518();

void func_ov000_022dd590(u8 *p0) {
    s32 t0 = *(u32 *)data_ov000_02369b2c;
    *(u32 *)((u8 *)t0 + 0x224) = (*(u32 *)((u8 *)*(u32 *)data_ov000_02369b2c + 0x224) + 0x1);
    s32 t1 = *(s16 *)((u8 *)data_0208922c + 0x48);
    if (t1 > 0) {
        *(u16 *)((u8 *)data_0208922c + 0x48) = (t1 - 0x2);
    }
    s32 t2 = *(u32 *)((u8 *)t0 + 0x224);
    if (t2 > 0x1e) {
        *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022dd518;
    }
}

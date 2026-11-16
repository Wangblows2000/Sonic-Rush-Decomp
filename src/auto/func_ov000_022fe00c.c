/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_0206e3c0();
s32 func_ov000_022fe7e8();
s32 func_02069090();

void func_ov000_022fe00c(u8 *p0) {
    if ((*(u16 *)((u8 *)p0 + 0x5c) & 8) != 0) {
        *(u16 *)((u8 *)p0 + 0x238) = (*(s16 *)((u8 *)p0 + 0x238) - 0x1);
        if (*(s16 *)((u8 *)p0 + 0x238) == 0) {
            func_0206e3c0(((u8 *)p0 + 0x20c), -120, -48, -16, -16);
            func_ov000_022fe7e8(p0);
            return;
        }
    }
    func_02069090(p0, ((u8 *)p0 + 0x20c));
}

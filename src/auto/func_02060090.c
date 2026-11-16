/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_0208922c[];
void func_0206005c();
s32 func_0205fcac();
s32 func_02060e54();

void func_02060090(u8 *p0) {
    *(u16 *)((u8 *)p0 + 0x8) = (*(s16 *)((u8 *)p0 + 8) + 0x1);
    *(u8 *)((u8 *)p0 + 0x16) = (((-46 * *(s16 *)((u8 *)p0 + 8)) / 0xf) + 0x86);
    *(u8 *)((u8 *)data_0208922c + 0x15) = *(u8 *)((u8 *)p0 + 0x16);
    s32 t0 = *(u8 *)((u8 *)p0 + 0x16);
    *(u8 *)((u8 *)data_0208922c + 0x17) = t0;
    if (*(s16 *)((u8 *)p0 + 8) >= 0xf) {
        *(u32 *)p0 = (u32)func_0206005c;
        func_0205fcac(p0, t0);
        func_02060e54(1, 4);
    }
}

#pragma thumb reset

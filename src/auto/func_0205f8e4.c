/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_0208922c[];
void func_0205f8b0();
s32 func_02060e54();

void func_0205f8e4(u8 *p0) {
    *(u16 *)((u8 *)p0 + 0x4) = (*(s16 *)((u8 *)p0 + 4) + 0x1);
    *(u16 *)((u8 *)data_0208922c + 0x24) = ((*(s16 *)((u8 *)p0 + 4) << 3) / 0x1e);
    if (*(s16 *)((u8 *)p0 + 4) >= 0x1e) {
        *(u32 *)p0 = (u32)func_0205f8b0;
        func_02060e54(2, 4);
    }
}

#pragma thumb reset

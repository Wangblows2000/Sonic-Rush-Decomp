/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
extern u8 data_0208922c[];
s32 func_02052104();
void func_ov001_022e901c();

void func_ov001_022e926c(s32 p0) {
    s32 v0 = (s32)p0;
    if ((s32)p0 == 0) {
        v0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    }
    *(u32 *)v0 = 1;
    *(u32 *)((u8 *)v0 + 0x4) = 0;
    *(u32 *)((u8 *)v0 + 0x10) = 0;
    *(u16 *)((u8 *)data_0208922c + 0x8) = 0;
    *(u16 *)((u8 *)data_0208922c + 0xa) = 0x48;
    *(u32 *)((u8 *)v0 + 0xc) = 0x48;
    func_02052104(0xa, 0x48);
    *(u32 *)((u8 *)*(u32 *)((u8 *)v0 + 0x190) + 0x8) = (u32)func_ov001_022e901c;
}

#pragma thumb reset

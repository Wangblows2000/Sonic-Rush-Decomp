/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gPad[];
s32 func_0205004c();
void func_02059dac();

void func_02059dd0(u8 *p0) {
    if ((*(u16 *)((u8 *)gPad + 0x4) & 8) == 0) {
        return;
    }
    func_0205004c((*(u16 *)((u8 *)gPad + 0x4) & 8), gPad);
    *(u32 *)p0 = (u32)func_02059dac;
}

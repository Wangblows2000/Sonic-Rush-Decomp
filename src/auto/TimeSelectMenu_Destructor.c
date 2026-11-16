/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 Touch_Disable();
s32 func_ov000_02360324();
s32 func_ov000_0235fce0();
s32 func_ov000_023622f0();
s32 func_ov000_023622ec();
s32 func_ov000_023622b4();
s32 func_ov000_02362240();
s32 func_ov000_0235f35c();

void TimeSelectMenu_Destructor(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    Touch_Disable();
    func_ov000_02360324(t0);
    func_ov000_0235fce0(t0);
    func_ov000_023622f0(t0);
    func_ov000_023622ec(t0);
    func_ov000_023622b4(t0);
    func_ov000_02362240(t0);
    func_ov000_0235f35c();
}

#pragma thumb reset

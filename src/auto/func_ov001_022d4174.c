/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_ov001_022d40a4();
s32 func_ov001_022d3110();
s32 func_ov001_022d4048();
s32 func_ov001_022d3fc8();
s32 func_ov001_022d3f10();
s32 Touch_Enable();
s32 func_ov001_022d35d0();
extern u8 data_0208926c[];
s32 func_ov001_022d3538();
s32 func_ov001_022d37ac();
s32 SysSound_Init();
s32 func_02052168();

void func_ov001_022d4174(void) {
    s32 t0 = func_ov001_022d40a4();
    func_ov001_022d3110();
    func_ov001_022d4048(t0);
    func_ov001_022d3fc8(t0);
    func_ov001_022d3f10(t0);
    Touch_Enable();
    func_ov001_022d35d0();
    func_ov001_022d3538(*(s16 *)((u8 *)data_0208926c + 8), 0, 0x10, 2);
    func_ov001_022d37ac(t0);
    SysSound_Init();
    func_02052168(0);
}

#pragma thumb reset

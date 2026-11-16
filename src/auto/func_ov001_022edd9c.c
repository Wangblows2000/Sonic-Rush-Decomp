/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_ov001_022edd28();
s32 func_ov001_022ec49c();
s32 func_ov001_022edcb0();
s32 func_ov001_022edb3c();
s32 func_ov001_022eda54();
s32 func_ov001_022ecae4();
s32 Touch_Enable();
extern u8 data_0208926c[];
s32 func_ov001_022ec8e8();
void func_ov001_022ec964();
s32 Task_Create();
s32 SysSound_Init();
s32 func_02052168();

void func_ov001_022edd9c(void) {
    s32 t0 = func_ov001_022edd28();
    func_ov001_022ec49c();
    func_ov001_022edcb0(t0);
    func_ov001_022edb3c(t0);
    func_ov001_022eda54(t0);
    func_ov001_022ecae4(t0);
    Touch_Enable();
    func_ov001_022ec8e8(*(s16 *)((u8 *)data_0208926c + 8), 0, 0x10, 0);
    Task_Create((u32)func_ov001_022ec964, 0, 0, 0x2000, 0x1000, 0);
    SysSound_Init();
    func_02052168(0);
}

#pragma thumb reset

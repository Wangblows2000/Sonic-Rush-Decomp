/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
s32 func_020371c8();
void func_020480ec();
s32 func_0204e934();
extern u8 data_022c4560[];
s32 func_02059bdc();
s32 func_02059bcc();
s32 func_0204ed24();

void func_020486b8(void) {
    u32 t0 = *(u32 *)data_022b4574;
    u32 t1 = *(u32 *)((u8 *)t0 + 0x10);
    u32 t2 = *(u32 *)t1;
    if (t2 != 0) {
        ((s32 (*)())t2)(t1, t2);
    }
    u32 t3 = *(u32 *)((u8 *)t1 + 0xc);
    if ((t3 & 0x80000000) != 0) {
        u32 t4 = *(u32 *)((u8 *)t1 + 0x4);
        u32 t5 = *(u32 *)((u8 *)t1 + 0x1c);
        u32 t6 = *(u32 *)data_022b4574;
        func_020371c8(t6, (t3 & 0x80000000));
        if (t4 == (s32)(u32)func_020480ec) {
            func_0204e934((u32)func_020480ec);
        }
        if ((t3 & 1) != 0) {
            u32 t7 = *(u32 *)data_022c4560;
            if (t7 != 1) {
                func_02059bdc(t5);
            } else {
                func_02059bcc(t5);
            }
            func_0204ed24();
            ((s32 (*)())t4)();
            return;
        }
        ((s32 (*)())t4)();
    }
}

#pragma thumb reset

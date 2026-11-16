/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022503e0[];
s32 func_02060e54();
s32 func_02060e70();
extern u8 data_022b65b4[];
void func_02060bd8();

void func_02060c1c(u8 *p0) {
    if ((*(u16 *)((u8 *)data_022503e0 + 0x4) & 1) != 0) {
        func_02060e54(3, 0x10);
    }
    s32 t0 = func_02060e70(0);
    if ((t0 & 1) != 0) {
        if ((*(u32 *)((u8 *)data_022b65b4 + 0x10) & 0x40) != 0) {
            func_02060e54(3, 0x10);
        }
    }
    if ((*(u16 *)data_022503e0 & 0x100) != 0) {
        func_02060e54(3, 0x40);
    }
    s32 t1 = func_02060e70(0);
    if ((t1 & 0x200) != 0) {
        s32 t2 = func_02060e70(2, 0x200);
        if ((t2 & 4) != 0) {
            func_02060e54(0, 0x400);
            *(u32 *)p0 = (u32)func_02060bd8;
        }
    }
    s32 t3 = func_02060e70(3);
    if ((t3 & 0x100) != 0) {
        *(u32 *)p0 = (u32)func_02060bd8;
    }
}

#pragma thumb reset

#include <nitro/types.h>

extern u16 data_022c4da0;

#pragma thumb on
void func_ov001_022e1a30(s32 v) {
    switch (v) {
    case 1:
        data_022c4da0 = 0x28;
        break;
    case 4:
        data_022c4da0 = 0x29;
        break;
    case 7:
        data_022c4da0 = 0x2a;
        break;
    case 15:
        data_022c4da0 = 0x2b;
        break;
    case 0x13:
        data_022c4da0 = 0x2c;
        break;
    case 0x17:
        data_022c4da0 = 0x2d;
        break;
    case 0x1b:
        data_022c4da0 = 0x2e;
        break;
    case 0x1e:
        data_022c4da0 = 0x2f;
        break;
    }
}
#pragma thumb reset

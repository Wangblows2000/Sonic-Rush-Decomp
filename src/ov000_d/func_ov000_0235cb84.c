#include <nitro/types.h>

s32 func_02051300(void *buf, s32 n);
BOOL func_ov000_0235cbd8(void);
void func_0205961c(s32 a, void *buf, u8 len, s32 b);

#pragma thumb on

void func_ov000_0235cb84(s32 mode, s32 arg) {
    u8 buf[0x18];
    u8 len = func_02051300(buf, 10);
    s32 v;

    switch (mode) {
    case 0:
        if (func_ov000_0235cbd8()) {
            v = 0;
        } else {
            v = 1;
        }
        break;
    case 1:
        if (func_ov000_0235cbd8()) {
            v = 1;
        } else {
            v = 0;
        }
        break;
    default:
        v = 2;
        break;
    }
    func_0205961c(v, buf, len, arg);
}

#pragma thumb reset

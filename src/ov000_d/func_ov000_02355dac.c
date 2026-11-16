#include <nitro/types.h>

void func_020550ec(void);
BOOL func_020513a8(void);
void func_0204b624(void);
void func_020521b4(void);
void func_02051684(void (*f)(void));
void func_020550d0(void);
void func_020550b4(void);

#pragma thumb on

void func_ov000_02355dac(s32 mode) {
    switch (mode) {
    case 0:
        func_020550ec();
        break;
    case 1:
        if (func_020513a8()) {
            func_0204b624();
        }
        func_020521b4();
        func_02051684(func_020550ec);
        break;
    case 2:
        func_020521b4();
        func_020550ec();
        break;
    case 3:
        func_020550d0();
        break;
    case 4:
        func_020550b4();
        break;
    }
}

#pragma thumb reset

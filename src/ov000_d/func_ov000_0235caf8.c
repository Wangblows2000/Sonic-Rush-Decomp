#include <nitro/types.h>

void func_020521b4(void);
void func_02051628(s32 a);
BOOL func_020513a8(void);
void func_ov000_0235e31c(void);
void func_02055140(void);
void func_0204b528(void);
BOOL func_ov000_0235cbd8(void);
void func_0205507c(void);
void func_02055060(void);
void func_0204b624(void);
void func_02054e68(void);
void func_02051684(void (*f)(void));

#pragma thumb on

void func_ov000_0235caf8(s32 mode) {
    switch (mode) {
    case 0:
        func_020521b4();
        func_02051628(1);
        if (func_020513a8()) {
            func_ov000_0235e31c();
        } else {
            func_02055140();
        }
        break;
    case 1:
        if (func_020513a8()) {
            func_0204b528();
        }
        func_020521b4();
        if (func_ov000_0235cbd8()) {
            func_0205507c();
        } else {
            func_02055060();
        }
        break;
    case 2:
        func_020521b4();
        if (func_020513a8()) {
            func_0204b528();
            func_0204b624();
        }
        func_02051684(func_02054e68);
        break;
    }
}

#pragma thumb reset

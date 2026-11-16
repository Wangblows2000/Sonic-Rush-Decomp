#include <nitro/types.h>

u32 func_020111e4(void);
void func_020405bc(s32 v);
void func_020405e0(s32 a);
u32 func_0203e674(void (*cb)(void), u16 ch);
void func_0203e6a0(void);

u16 func_0203e790(u16 ch) {
    u32 m = func_020111e4();

    if (m == 0x8000) {
        func_020405bc(3);
        func_020405e0(9);
        return 3;
    }
    if (m == 0) {
        func_020405bc(0x16);
        func_020405e0(9);
        return 0x18;
    }
    while (!((1 << (ch - 1)) & m)) {
        ch++;
        if (ch > 16)
            return 0x18;
    }
    return func_0203e674(func_0203e6a0, ch);
}

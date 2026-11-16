#include <nitro/types.h>

extern u32 data_02077978;

u32 func_020061ec(void);
BOOL OS_IsRunOnEmulator(void);
BOOL func_020061b8(void);

u32 OS_GetConsoleType(void)
{
    if (data_02077978 == 0xffffffff) {
        u32 t = func_020061ec();
        u32 v;
        if (OS_IsRunOnEmulator()) {
            v = t | 0x10000000;
        } else if (func_020061b8()) {
            v = t | 0x40000000;
        } else if (t & 0x1000000) {
            v = t | 0x20000000;
        } else {
            v = t | 0x80000000;
        }
        data_02077978 = v | *(u16 *)0x027ffffa;
    }
    return data_02077978;
}

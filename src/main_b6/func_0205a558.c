#include <nitro/types.h>

s32 SaveData_Startup(void);
void func_0205a27c(u32 a, u32 b);
void func_0205a304(void);

s32 func_0205a558(u32 arg) {
    switch (SaveData_Startup()) {
    case 0:
    case 4:
        return 1;
    case 3:
        func_0205a27c(3, arg);
        return 0;
    case 6:
    case 7:
        return 1;
    case 1:
    case 2:
    case 5:
    default:
        func_0205a304();
        func_0205a27c(1, 0);
        return 0;
    }
}

#include <nitro/types.h>

extern void *data_022b4574;
extern s32 data_ov001_0231f044;
u32 func_0203e944(void);
void func_02042d34(s32 a);
void func_020371c8(void *p);

#pragma thumb on
void func_ov001_022c8640(void) {
    if ((func_0203e944() & ~1) == 0) {
        func_02042d34(0);
        func_020371c8(data_022b4574);
        data_ov001_0231f044 = 0;
    }
}
#pragma thumb reset

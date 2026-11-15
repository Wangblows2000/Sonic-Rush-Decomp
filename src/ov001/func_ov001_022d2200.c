#include <nitro/types.h>

extern s16 data_0208926c[];
extern s16 data_02087d44[];
void func_ov001_022d2234(void *p, void (*f)(void));
void func_ov001_022d21d4(void);

#pragma thumb on
void func_ov001_022d2200(s32 *p) {
    if (*p == 2) {
        data_0208926c[4] = -0x10;
        data_02087d44[4] = -0x10;
    } else {
        data_0208926c[4] = 0x10;
        data_02087d44[4] = 0x10;
    }
    func_ov001_022d2234(p, func_ov001_022d21d4);
}
#pragma thumb reset

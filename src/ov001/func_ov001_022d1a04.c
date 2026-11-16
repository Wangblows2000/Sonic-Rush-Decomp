#include <nitro/types.h>

extern u16 data_0208926c[];
extern u16 data_02087d44[];
void StaffRoll_SetState(void *p, void (*f)(void));
void func_ov001_022d15e8(void);

#pragma thumb on
void func_ov001_022d1a04(void *p) {
    data_0208926c[4] = 0;
    data_02087d44[4] = 0;
    StaffRoll_SetState(p, func_ov001_022d15e8);
}
#pragma thumb reset

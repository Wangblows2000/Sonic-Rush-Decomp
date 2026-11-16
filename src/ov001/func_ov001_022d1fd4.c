#include <nitro/types.h>

void StaffRoll_SetState(void *p, void (*f)(void));
void func_ov001_022d1ec8(void);

#pragma thumb on
void func_ov001_022d1fd4(u8 *p) {
    if (*(s32 *)(p + 0x14) > 0x1d0) {
        StaffRoll_SetState(p, func_ov001_022d1ec8);
    }
}
#pragma thumb reset

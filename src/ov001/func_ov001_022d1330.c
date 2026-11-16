#include <nitro/types.h>

void StaffRoll_SetPictureState(void *p, void (*f)(void));
void func_ov001_022d12a8(void);

#pragma thumb on
void func_ov001_022d1330(u8 *p) {
    if (*(s32 *)(p + 0xbe0) == 1) {
        StaffRoll_SetPictureState(p, func_ov001_022d12a8);
    }
}
#pragma thumb reset

#include <nitro/types.h>

void StaffRoll_SetState(void *p, void (*f)(void));
void func_ov001_022d1c9c(void);

#pragma thumb on
void func_ov001_022d1cc4(u8 *p) {
    if (*(s32 *)(p + 0x14) >= 0x5a) {
        StaffRoll_SetState(p, func_ov001_022d1c9c);
    }
}
#pragma thumb reset

#include <nitro/types.h>

void StaffRoll_SetState(void *p, void (*f)(void));
void func_ov001_022d20e4(void);
void func_ov001_022d215c(void);

#pragma thumb on
void func_ov001_022d21d4(s32 *p) {
    if (*p == 2) {
        StaffRoll_SetState(p, func_ov001_022d20e4);
    } else {
        StaffRoll_SetState(p, func_ov001_022d215c);
    }
}
#pragma thumb reset

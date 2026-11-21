#include <nitro/types.h>

void StaffRoll_State_None(void);

#pragma thumb on
void StaffRoll_SetState(u8 *p, void (*f)(void)) {
    if (f == NULL) {
        *(void (**)(void))(p + 0x10) = StaffRoll_State_None;
    } else {
        *(void (**)(void))(p + 0x10) = f;
    }
    *(s32 *)(p + 0x14) = 0;
}
#pragma thumb reset

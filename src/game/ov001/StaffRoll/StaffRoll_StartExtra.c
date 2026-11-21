#include <nitro/types.h>

void StaffRoll_Create(s32 a);

#pragma thumb on
void StaffRoll_StartExtra(void) {
    StaffRoll_Create(2);
}
#pragma thumb reset

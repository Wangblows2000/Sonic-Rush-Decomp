#include <nitro/types.h>

extern u8 *gStaffRollWork;
extern u16 data_02087d44[];

#pragma thumb on
void StaffRoll_SetSubBrightness(u16 v) {
    if (*(s32 *)(gStaffRollWork + 0xbb4) == 0) {
        data_02087d44[4] = v;
    }
}
#pragma thumb reset

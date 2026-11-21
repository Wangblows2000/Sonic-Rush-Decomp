#include <nitro/types.h>

extern u8 *gStaffRollWork;
extern u16 data_0208926c[];

#pragma thumb on
void StaffRoll_SetMainBrightness(u16 v) {
    if (*(s32 *)(gStaffRollWork + 0xbb4) == 0) {
        data_0208926c[4] = v;
    }
}
#pragma thumb reset

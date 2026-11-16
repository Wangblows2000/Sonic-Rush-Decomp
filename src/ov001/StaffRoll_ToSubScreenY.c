#include <nitro/types.h>

#pragma thumb on
s32 StaffRoll_ToSubScreenY(s32 x) {
    return x - 0x110;
}
#pragma thumb reset

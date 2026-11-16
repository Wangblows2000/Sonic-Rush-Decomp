#include <nitro/types.h>

s32 func_ov000_0235f9b0(void);
void TimeSelectMenu_Create(s32 a, s32 b);

#pragma thumb on
void TimeSelectMenu_StartMode1(void) {
    TimeSelectMenu_Create(1, func_ov000_0235f9b0());
}
#pragma thumb reset

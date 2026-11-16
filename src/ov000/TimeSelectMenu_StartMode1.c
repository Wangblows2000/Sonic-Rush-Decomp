#include <nitro/types.h>

s32 TimeSelectMenu_IsMode1Flag(void);
void TimeSelectMenu_Create(s32 a, s32 b);

#pragma thumb on
void TimeSelectMenu_StartMode1(void) {
    TimeSelectMenu_Create(1, TimeSelectMenu_IsMode1Flag());
}
#pragma thumb reset

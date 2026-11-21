#include <nitro/types.h>

s32 TimeSelectMenu_IsMode0Flag(void);
void TimeSelectMenu_Create(s32 a, s32 b);

#pragma thumb on
void TimeSelectMenu_StartMode0(void) {
    TimeSelectMenu_Create(0, TimeSelectMenu_IsMode0Flag());
}
#pragma thumb reset

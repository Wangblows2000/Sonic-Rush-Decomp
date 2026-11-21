#include <nitro/types.h>

void OptionsMenu_OpenPage1(void *p);

#pragma thumb on
void OptionsMenu_OpenPage4(u8 *p) {
    OptionsMenu_OpenPage1(p);
    *(s32 *)(p + 0x28) = -1;
}
#pragma thumb reset

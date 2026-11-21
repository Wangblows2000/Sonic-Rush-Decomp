#include <nitro/types.h>

void OptionsMenu_OpenPage1(void);

#pragma thumb on
void OptionsMenu_OpenPage2(void) {
    OptionsMenu_OpenPage1();
}
#pragma thumb reset

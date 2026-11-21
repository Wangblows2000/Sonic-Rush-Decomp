#include <nitro/types.h>

void OptionsMenu_ClosePage1(void);

#pragma thumb on
void OptionsMenu_ClosePage2(void) {
    OptionsMenu_ClosePage1();
}
#pragma thumb reset

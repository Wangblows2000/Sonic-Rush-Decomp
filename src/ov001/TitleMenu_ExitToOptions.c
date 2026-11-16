#include <nitro/types.h>

void Options_StartFromTitleMenu(void);

#pragma thumb on
void TitleMenu_ExitToOptions(void) {
    Options_StartFromTitleMenu();
}
#pragma thumb reset

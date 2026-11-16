#include <nitro/types.h>

void TitleMenu_Start(void);

#pragma thumb on
void ZoneActSelectMode_ExitToTitleMenu(void) {
    TitleMenu_Start();
}
#pragma thumb reset

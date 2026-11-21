#include <nitro/types.h>

void SysSound_Release(void);
void TitleScreen_Start(void);

#pragma thumb on
void TitleMenu_ExitToTitleScreen(void) {
    SysSound_Release();
    TitleScreen_Start();
}
#pragma thumb reset

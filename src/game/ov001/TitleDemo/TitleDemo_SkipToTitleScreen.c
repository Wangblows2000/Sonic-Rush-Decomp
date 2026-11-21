#include <nitro/types.h>

void TitleScreen_StartFromDemo(void);

#pragma thumb on
void TitleDemo_SkipToTitleScreen(void) {
    TitleScreen_StartFromDemo();
}
#pragma thumb reset

#include <nitro/types.h>

extern u8 gPad[];
void Pad_Reset(void *p, s32 a, s32 b);
void TitleMenu_Start(void);

#pragma thumb on
void TitleScreen_GoToTitleMenu(void) {
    Pad_Reset(gPad, 0, 0);
    TitleMenu_Start();
}
#pragma thumb reset

#include <nitro/types.h>

extern s32 gSaveDataBusy;
extern u32 gSystemFlags;
void TitleScreen_Create(void);

#pragma thumb on
void TitleScreen_StartFromDemo(void) {
    TitleScreen_Create();
    gSaveDataBusy = 0;
    gSystemFlags &= ~0x10;
}
#pragma thumb reset

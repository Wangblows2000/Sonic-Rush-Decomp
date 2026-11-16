#include <nitro/types.h>

extern s32 gSaveDataBusy;
extern u32 gSystemFlags;
void SegaLogo_InitDisplay(void);
void SegaLogo_Create(void);

#pragma thumb on
void SegaLogo_Start(void) {
    SegaLogo_InitDisplay();
    SegaLogo_Create();
    gSaveDataBusy = 0;
    gSystemFlags &= ~0x10;
}
#pragma thumb reset

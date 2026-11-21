#include <nitro/types.h>

void TitleScreen_ReleaseSound(void *p);
void Touch_Disable(void);
void TitleScreen_ReleaseSprites(void *p);
void TitleScreen_Dummy(void *p);
void TitleScreen_ReleaseAssets(void *p);
void TitleScreen_ResetDisplay(void);

#pragma thumb on
void TitleScreen_Destructor(u8 *p) {
    void *q = *(void **)(p + 0x10);
    TitleScreen_ReleaseSound(q);
    Touch_Disable();
    TitleScreen_ReleaseSprites(q);
    TitleScreen_Dummy(q);
    TitleScreen_ReleaseAssets(q);
    TitleScreen_ResetDisplay();
}
#pragma thumb reset

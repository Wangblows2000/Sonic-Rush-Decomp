#include <nitro/types.h>

void Touch_Disable(void *p);
void TitleMenu_ReleaseAssets(void *p);

#pragma thumb on
void TitleMenu_Destructor(u8 *p) {
    void *q = *(void **)(p + 0x10);
    Touch_Disable(p);
    TitleMenu_ReleaseAssets(q);
}
#pragma thumb reset

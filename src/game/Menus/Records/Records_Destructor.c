#include <nitro/types.h>

void Touch_Disable(void *p);
void Records_FreeNameGlyphVram(void *p);
void Records_DestructorDummy(void *p);
void Records_ReleaseSprites(void *p);
void Records_ReleaseAssets(void *p);
void Records_ResetDisplay(void);

#pragma thumb on
void Records_Destructor(u8 *p) {
    void *q = *(void **)(p + 0x10);
    Touch_Disable(p);
    Records_FreeNameGlyphVram(q);
    Records_DestructorDummy(q);
    Records_ReleaseSprites(q);
    Records_ReleaseAssets(q);
    Records_ResetDisplay();
}
#pragma thumb reset

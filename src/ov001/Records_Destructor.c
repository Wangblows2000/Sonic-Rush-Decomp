#include <nitro/types.h>

void Touch_Disable(void *p);
void Records_Unk_ov001_022eca98(void *p);
void Records_Unk_ov001_022ed960(void *p);
void Records_ReleaseSprites(void *p);
void Records_ReleaseAssets(void *p);
void Records_ResetDisplay(void);

#pragma thumb on
void Records_Destructor(u8 *p) {
    void *q = *(void **)(p + 0x10);
    Touch_Disable(p);
    Records_Unk_ov001_022eca98(q);
    Records_Unk_ov001_022ed960(q);
    Records_ReleaseSprites(q);
    Records_ReleaseAssets(q);
    Records_ResetDisplay();
}
#pragma thumb reset

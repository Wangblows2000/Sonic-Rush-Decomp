#include <nitro/types.h>

void SegaLogo_Unk_ov001_022cf894(void *p);
void SegaLogo_ReleaseAssets(void *p);
void SegaLogo_ResetDisplay(void);

#pragma thumb on
void SegaLogo_Destructor(u8 *p) {
    void *q = *(void **)(p + 0x10);
    SegaLogo_Unk_ov001_022cf894(q);
    SegaLogo_ReleaseAssets(q);
    SegaLogo_ResetDisplay();
}
#pragma thumb reset

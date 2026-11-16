#include <nitro/types.h>

void SegaLogo_Dummy(void *p);
void SegaLogo_ReleaseAssets(void *p);
void SegaLogo_ResetDisplay(void);

#pragma thumb on
void SegaLogo_Destructor(u8 *p) {
    void *q = *(void **)(p + 0x10);
    SegaLogo_Dummy(q);
    SegaLogo_ReleaseAssets(q);
    SegaLogo_ResetDisplay();
}
#pragma thumb reset

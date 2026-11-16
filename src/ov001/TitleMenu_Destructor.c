#include <nitro/types.h>

void Touch_Disable(void *p);
void TitleMenu_Unk_ov001_022d8c78(void *p);

#pragma thumb on
void TitleMenu_Destructor(u8 *p) {
    void *q = *(void **)(p + 0x10);
    Touch_Disable(p);
    TitleMenu_Unk_ov001_022d8c78(q);
}
#pragma thumb reset

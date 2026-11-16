#include <nitro/types.h>

void TitleMenu_Unk_ov001_022d6dd4(void);

#pragma thumb on
void VsMenu_Unk_ov001_022da41c(u8 *p) {
    s32 i;
    u8 *q;
    TitleMenu_Unk_ov001_022d6dd4();
    for (i = 0, q = p; i < 0x1f; i++) {
        *(u32 *)(q + 0x23c) &= ~0x60000;
        q += 0x48;
    }
    *(s32 *)(p + 0x18) = 0;
}
#pragma thumb reset

#include <nitro/types.h>

void WirelessSession_StartDownloadPlayPreview(s32 a);

#pragma thumb on
void OptionsMenu_Unk_ov001_022c8d70(u8 *p) {
    WirelessSession_StartDownloadPlayPreview(*(s32 *)(p + 0xc));
}
#pragma thumb reset

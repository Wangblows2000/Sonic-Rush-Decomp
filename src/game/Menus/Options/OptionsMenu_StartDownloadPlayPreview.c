#include <nitro/types.h>

void WirelessSession_StartDownloadPlayPreview(s32 a);

#pragma thumb on
void OptionsMenu_StartDownloadPlayPreview(u8 *p) {
    WirelessSession_StartDownloadPlayPreview(*(s32 *)(p + 0xc));
}
#pragma thumb reset

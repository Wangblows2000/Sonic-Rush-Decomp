#include <nitro/types.h>
#include <main_sdk2/wh.h>

u32 WM_GetAllowedChannel(void);

BOOL changeScanChannel(UnkScanParam_wh *param)
{
    u32 mask;
    u16 ch;
    u16 i;
    u16 orig;

    mask = WM_GetAllowedChannel();
    if (mask == 0) {
        return FALSE;
    }
    orig = param->channel;
    ch = orig;
    i = 0;
    do {
        if ((mask & (1 << (ch - 1))) && orig != ch) {
            param->channel = ch;
            break;
        }
        i++;
        ch = (ch == 16) ? 1 : ch + 1;
    } while (i < 16);
    return TRUE;
}

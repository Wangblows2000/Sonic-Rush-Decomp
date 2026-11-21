#include <nitro/types.h>
#include <NitroSDK/wm.h>

s32 WM_EndDataSharing(void *ds);

typedef struct {
    u16 apiid;
    u16 errcode;
    u32 unk4;
} UnkWmCb_02012d20;

s32 WM_EndKeySharing(void (*cb)(void *))
{
    u8 *sys = WMi_GetSystemWork();
    UnkWmCb_02012d20 arg;
    s32 r = WMi_CheckStateEx(2, 9, 10);
    if (r != 0) {
        return r;
    }
    if (*(u32 *)(sys + 0x15c) == 0) {
        return 3;
    }
    r = WM_EndDataSharing(sys + 0x160);
    *(u32 *)(sys + 0x15c) = 0;
    arg.apiid = 0x16;
    arg.errcode = r;
    cb(&arg);
    return 2;
}

#include <nitro/types.h>
#include <main_sdk2/wm.h>

void DC_InvalidateRange(void *p, u32 size);
s32 WM_StartDataSharing(void *ds, u16 port, u16 bitmap, u16 size, BOOL doubleMode);

typedef struct {
    u16 apiid;
    u16 errcode;
} UnkWmCb_02012da0;

s32 WM_StartKeySharing(void (*cb)(void *))
{
    UnkWmWork *sys = WMi_GetSystemWork();
    UnkWmCb_02012da0 arg;
    s32 r = WMi_CheckStateEx(2, 9, 10);
    if (r != 0) {
        return r;
    }
    DC_InvalidateRange(sys->unk4 + 0xc, 4);
    if (*(u32 *)(sys->unk4 + 0xc) == 0) {
        return 3;
    }
    *(u32 *)((u8 *)sys + 0x15c) = 1;
    r = WM_StartDataSharing((u8 *)sys + 0x160, 8, 0xffff, 2, 1);
    arg.apiid = 0x15;
    arg.errcode = r;
    cb(&arg);
    return 2;
}

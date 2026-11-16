#include <nitro/types.h>
#include <main_sdk2/wm.h>

void DC_InvalidateRange(void *p, u32 size);
void DC_StoreRange(void *p, u32 size);

s32 func_02011c44(void *cb, void *arg, u16 *data, u16 size, u16 dest, u16 port, u16 prio)
{
    u16 maxSize;
    u16 bitmap = 1;
    u8 *p = ((UnkWmWork *)func_02010b70())->unk4;
    s32 r = WMi_CheckStateEx(2, 9, 10);
    BOOL isParent;
    if (r != 0) {
        return r;
    }
    DC_InvalidateRange(p + 0x18e, 2);
    maxSize = *(u16 *)(p + 0x18e);
    DC_InvalidateRange(p + 0x184, 2);
    isParent = (*(u16 *)(p + 0x184) == 0) ? TRUE : FALSE;
    if (isParent == TRUE) {
        DC_InvalidateRange(p + 0x17e, 2);
        bitmap = *(u16 *)(p + 0x17e);
        DC_InvalidateRange(p + 0x86, 2);
    }
    if (data == NULL) {
        return 6;
    }
    if (bitmap == 0) {
        return 7;
    }
    DC_InvalidateRange(p + 0x50, 2);
    if ((u32)data == *(u32 *)(p + 0x50)) {
        return 6;
    }
    if (size + (isParent ? 4 : 2) > maxSize) {
        return 6;
    }
    if (size == 0) {
        return 6;
    }
    DC_StoreRange(data, size);
    r = func_02010b80(0xf, 7, data, size, dest, port, prio, cb, arg);
    if (r == 0) {
        r = 2;
    }
    return r;
}

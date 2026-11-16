#include <nitro/types.h>

u8 *ExtraBoss_GetWork(void);
void func_ov001_023017c8(void *p);
void func_ov001_023024d0(void *p, s32 a);

BOOL func_ov001_02302408(u8 *p) {
    u32 *a = *(u32 **)(p + 0x1dc);
    u16 held;
    BOOL r = FALSE;
    held = *(u16 *)(ExtraBoss_GetWork() + 0x18);
    u16 trg = *(u16 *)(ExtraBoss_GetWork() + 0x1c);
    u16 *keys;
    if (a[0] & 4) {
        return r;
    }
    keys = (u16 *)a[2];
    if (trg & keys[0]) {
        func_ov001_023017c8(p);
        r = TRUE;
    } else if (trg & keys[1]) {
        func_ov001_023024d0(p, 8);
        r = TRUE;
        *(u32 *)(p + 0x14) &= ~0x700;
    } else if (!(a[1] & 2) && (held & 0xf0)) {
        func_ov001_023024d0(p, 2);
        r = TRUE;
        *(u32 *)(p + 0x14) &= ~0x700;
    }
    return r;
}

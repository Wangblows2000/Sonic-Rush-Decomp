#include <nitro/types.h>

typedef struct { u8 pad0[0xc]; s32 unkC; } UnkWork_02050d60;
typedef struct { u8 pad0[0x10]; UnkWork_02050d60 *work; } UnkTask_02050d60;
extern UnkTask_02050d60 *data_022c4638;

u32 WirelessMgr_GetConnectedBitmap(void);
u8 *WirelessMgr_GetChildInfo(u16 i);
void MIi_CpuCopy16(const void *src, void *dst, u32 size);

#pragma thumb on
s32 func_02050d60(s32 n, u16 *dst, s32 len) {
    UnkTask_02050d60 *t = data_022c4638;
    UnkWork_02050d60 *w = t->work;
    s32 st;
    u8 *p;
    s32 i;
    u32 mask;

    if (t == NULL)
        st = 8;
    else
        st = w->unkC;
    if (st < 3)
        return 0;
    if (len > 10)
        len = 10;
    mask = WirelessMgr_GetConnectedBitmap();
    p = NULL;
    for (i = 1; i < 16; i++) {
        if (mask & (1 << i)) {
            if (n == 0) {
                p = WirelessMgr_GetChildInfo(i);
                break;
            }
            n--;
        }
    }
    if (p == NULL)
        return 0;
    MIi_CpuCopy16(p + 2, dst, len * 2);
    if (len >= p[1])
        len = p[1];
    return len;
}
#pragma thumb reset

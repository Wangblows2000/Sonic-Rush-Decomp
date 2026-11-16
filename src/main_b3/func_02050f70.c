#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    s32 unkC;
    u8 pad10[0x24];
    u16 unk34;
    u16 unk36;
    u16 names[8][0xb]; /* 0x38 */
    u32 unkE8[8];      /* 0xe8 */
    u16 unk108[8];     /* 0x108 */
} UnkWork_02050f70;
typedef struct { u8 pad0[0x10]; UnkWork_02050f70 *work; } UnkTask_02050f70;
typedef struct { u8 pad0[0x50]; u16 name[0xb]; u16 unk66; u8 pad68[0x5c]; u16 unkC4; } UnkInfo_02050f70;
extern UnkTask_02050f70 *data_022c4638;

s32 WirelessMgr_GetScanResultCount(void);
UnkInfo_02050f70 *WirelessMgr_GetScanResult(u16 i);
void MIi_CpuCopy16(const void *src, void *dst, u32 size);

#pragma thumb on
BOOL func_02050f70(void) {
    UnkTask_02050f70 *t = data_022c4638;
    UnkWork_02050f70 *w = t->work;
    s32 st;
    BOOL changed;
    s32 n;
    s32 i;
    s32 j;
    UnkInfo_02050f70 *p;
    u16 *name;

    if (t == NULL)
        st = 7;
    else
        st = w->unkC;
    if (st > 3)
        return FALSE;
    changed = FALSE;
    n = WirelessMgr_GetScanResultCount();
    if (n != w->unk34)
        changed = TRUE;
    for (i = 0; i < n && i < 8; i++) {
        p = WirelessMgr_GetScanResult(i);
        name = p->name;
        for (j = 0; j < 10 && !changed; j++) {
            if (name[j] != w->names[i][j])
                changed = TRUE;
        }
        if (p->unkC4 != w->unkE8[i])
            changed = TRUE;
        if (name[0xb] != w->unk108[i])
            changed = TRUE;
        MIi_CpuCopy16(name, w->names[i], 0x16);
        w->unkE8[i] = p->unkC4;
        w->unk108[i] = name[0xb];
    }
    w->unk34 = n;
    return changed;
}
#pragma thumb reset

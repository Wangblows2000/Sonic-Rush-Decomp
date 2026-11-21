#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u8 mac[6];
    u8 pada[0x3e];
    u16 unk48;
    u8 pad4a;
    u8 unk4b;
    u8 pad4c[0x74];
} UnkBss_02042404;

typedef struct {
    UnkBss_02042404 bss;
    u32 unkc0;
    u16 unkc4;
    u8 padc6[2];
} UnkEntry_02042404;

typedef struct {
    u8 pad0[0x16];
    u8 unk16;
    u8 pad17[0x11];
    UnkEntry_02042404 list[32];
    u16 count;
} UnkWork_02042404;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02042404 *work;
} UnkTask_02042404;

typedef struct {
    u8 pad0[0x12];
    u16 unk12;
} UnkInfo_02042404;

extern UnkTask_02042404 *gWirelessMgrTask;

s32 memcmp(const void *a, const void *b, u32 n);
void MIi_CpuCopy16(const void *src, void *dst, u32 size);
void MIi_CpuCopy32(const void *src, void *dst, u32 size);

void WirelessMgr_OnScanFoundParent(UnkBss_02042404 *d, UnkInfo_02042404 *info) {
    UnkWork_02042404 *w = gWirelessMgrTask->work;
    UnkEntry_02042404 *list = w->list;
    u16 n;
    u16 i;

    if (d->unk4b & 2)
        return;
    n = w->count;
    if (n >= 32)
        return;
    if ((d->unk48 & 0x1f) != w->unk16)
        return;
    if (d->unk48 & 0x8000) {
        for (i = 0; i < n; i++) {
            if (memcmp(list[i].bss.mac, d->mac, 6) == 0) {
                for (i++; i < n; i++) {
                    MIi_CpuCopy16(&list[i], &list[i - 1], sizeof(UnkEntry_02042404));
                    n = w->count;
                }
                w->count--;
                return;
            }
        }
        return;
    }
    for (i = 0; i < n; i++) {
        if (memcmp(list[i].bss.mac, d->mac, 6) == 0) {
            MIi_CpuCopy32(d, &list[i], sizeof(UnkBss_02042404));
            list[i].unkc0 = 0;
            list[i].unkc4 = info->unk12;
            return;
        }
    }
    MIi_CpuCopy32(d, &list[i], sizeof(UnkBss_02042404));
    list[i].unkc0 = 0;
    list[i].unkc4 = info->unk12;
    w->count++;
}

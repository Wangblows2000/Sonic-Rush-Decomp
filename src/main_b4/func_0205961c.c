#include <nitro/types.h>

typedef struct {
    u16 name[10];
    u8 mac[6];
    u8 pad1a[2];
    u8 len;
    u8 cnt[3];
} UnkEntry_0205961c;

typedef struct {
    u8 pad0[0x1d0];
    u8 cnt1d0[3];
    u8 pad1d3[0x1e];
    u8 cnt1f1[3];
} UnkSave_0205961c;

extern UnkEntry_0205961c data_022c4854[10];
extern UnkSave_0205961c gSaveData;

void MIi_CpuCopy16(const void *src, void *dst, u32 size);
void MIi_CpuClear16(u16 v, void *dst, u32 size);
void MI_CpuCopy8(const void *src, void *dst, u32 size);

void func_0205961c(s32 kind, u16 *name, u8 len, u8 *mac) {
    s32 i;
    s32 j;
    UnkEntry_0205961c *p;
    UnkEntry_0205961c e;

    i = 0;
    if (len > 10)
        len = 10;
    p = data_022c4854;
    for (i = 0; i < 10; i++, p++) {
        if (len == p->len) {
            for (j = 0; j < len; j++) {
                if (name[j] != p->name[j])
                    break;
            }
            if (j >= len) {
                for (j = 0; j < 6; j++) {
                    if (mac[j] != p->mac[j])
                        break;
                }
                if (j >= 6)
                    break;
            }
        }
    }
    if (i < 10) {
        MIi_CpuCopy16(&data_022c4854[i], &e, sizeof(e));
    } else {
        vu16 zero = 0;
        MIi_CpuClear16(zero, &e, sizeof(e));
        MIi_CpuCopy16(name, &e, len * 2);
        e.len = len;
        MI_CpuCopy8(mac, e.mac, 6);
    }
    for (j = i - 1; j >= 0; j--) {
        if (j < 9)
            MIi_CpuCopy16(&data_022c4854[j], &data_022c4854[j + 1], sizeof(e));
    }
    MIi_CpuCopy16(&e, &data_022c4854[0], sizeof(e));
    switch (kind) {
    case 0:
        if (gSaveData.cnt1d0[0] < 99)
            gSaveData.cnt1d0[0]++;
        if (gSaveData.cnt1f1[0] < 99)
            gSaveData.cnt1f1[0]++;
        break;
    case 1:
        if (gSaveData.cnt1d0[1] < 99)
            gSaveData.cnt1d0[1]++;
        if (gSaveData.cnt1f1[1] < 99)
            gSaveData.cnt1f1[1]++;
        break;
    case 2:
        if (gSaveData.cnt1d0[2] < 99)
            gSaveData.cnt1d0[2]++;
        if (gSaveData.cnt1f1[2] < 99)
            gSaveData.cnt1f1[2]++;
        break;
    }
}

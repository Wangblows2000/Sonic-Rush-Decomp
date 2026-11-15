#include <nitro/types.h>
#include <main_sdk2/wmds.h>
#include <main_sdk2/wm.h>

void DC_InvalidateRange(void *p, u32 size);
void MIi_CpuClearFast(u32 data, void *dest, u32 size);
u32 MATH_CountPopulation(u32 x);
s32 func_020114f0(u16 port, void *cb, void *arg);
s32 func_02011c44(void *cb, void *arg, void *data, u16 size, u16 dest, u16 port, u16 prio);
void func_020123bc(void *cb);
void func_020124d0(void *cb);
void func_0201228c(void *cb);

s32 func_02012a58(UnkDS *ds, u16 port, u16 aidBitmap, u16 dataLength, BOOL doubleMode)
{
    u16 childBitmap = 1;
    u16 aid;
    u8 *p = ((UnkWmWork *)func_02010b70())->unk4;
    s32 r = func_02010a50(2, 9, 10);
    if (r != 0) {
        return r;
    }
    DC_InvalidateRange(p + 0xc, 4);
    if (*(u32 *)(p + 0xc) == 0) {
        return 3;
    }
    if (ds == NULL) {
        return 6;
    }
    if (port >= 16) {
        return 6;
    }
    if (aidBitmap == 0) {
        return 6;
    }
    DC_InvalidateRange(p + 0x184, 2);
    aid = *(u16 *)(p + 0x184);
    if (aid == 0) {
        DC_InvalidateRange(p + 0x86, 2);
        childBitmap = *(u16 *)(p + 0x86);
    }
    {
        vu32 z = 0;
        MIi_CpuClearFast(z, ds, sizeof(UnkDS));
    }
    ds->writeIdx = 0;
    ds->readIdx = 0;
    ds->unk80c = 0;
    ds->dataLength = dataLength;
    ds->port = port;
    ds->aidBitmap = 0;
    ds->doubleMode = doubleMode ? 1 : 0;
    {
        u16 bm = aidBitmap | (1 << aid);
        u32 n;
        ds->aidBitmap = bm;
        n = MATH_CountPopulation(bm);
        ds->count = n;
        ds->setLength = dataLength * n;
    }
    if (ds->setLength > 0x1fc) {
        ds->aidBitmap = 0;
        return 6;
    }
    ds->setLength += 4;
    ds->state = 1;
    if (aid == 0) {
        s32 i;
        for (i = 0; i < 4; i++) {
            ds->buf[i].bitmap0 = ds->aidBitmap & (childBitmap | 1);
        }
        func_020114f0(port, func_020123bc, ds);
        for (i = 0; i < (ds->doubleMode == 1 ? 2 : 1); i++) {
            ds->writeIdx = (ds->writeIdx + 1) & 3;
            r = func_02011c44(func_020124d0, ds, &ds->buf[i], ds->setLength, ds->aidBitmap & childBitmap, ds->port,
                              1);
            if (r == 7) {
                ds->seq[i] = 0xffff;
                ds->readIdx = (ds->readIdx + 1) & 3;
            } else if (r != 0 && r != 2) {
                ds->state = 5;
                return 1;
            }
        }
    } else {
        ds->readIdx = 3;
        func_020114f0(port, func_0201228c, ds);
    }
    return 0;
}

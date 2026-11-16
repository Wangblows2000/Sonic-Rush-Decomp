#pragma opt_common_subs off
#include <nitro/types.h>
#include <main_sdk2/wmds.h>
#include <main_sdk2/wm.h>

void DC_InvalidateRange(void *p, u32 size);
void MIi_CpuCopy16(const void *src, void *dst, u32 size);
s32 func_02011c44(void *cb, void *arg, void *data, u16 size, u16 dest, u16 port, u16 prio);
void func_02011ff4(UnkDS *ds, BOOL force);
void WmDataSharingReceiveData(UnkDS *ds, u16 aid, const void *src);
void func_020124d0(void *cb);

s32 WM_StepDataSharing(UnkDS *ds, const u16 *send, u16 *recvSet)
{
    u16 childBitmap;
    u8 *p = ((UnkWmWork *)func_02010b70())->unk4;
    s32 r = WMi_CheckStateEx(2, 9, 10);
    BOOL ok;
    u16 state;
    BOOL force;
    u16 idx;
    s32 result;
    u16 aid;

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
    if (send == NULL) {
        return 6;
    }
    if (recvSet == NULL) {
        return 6;
    }
    DC_InvalidateRange(p + 0x184, 2);
    aid = *(u16 *)(p + 0x184);
    if (aid == 0) {
        DC_InvalidateRange(p + 0x86, 2);
        childBitmap = *(u16 *)(p + 0x86);
    }
    state = ds->state;
    if (state == 5) {
        return 1;
    }
    if (state != 1 && state != 4) {
        return 3;
    }
    result = 5;
    if (aid == 0) {
        ok = FALSE;
        force = FALSE;
        if (state == 4) {
            ds->state = 1;
            idx = (ds->writeIdx + 3) & 3;
            r = func_02011c44(func_020124d0, ds, &ds->buf[idx], ds->setLength, ds->aidBitmap & childBitmap, ds->port,
                              1);
            if (r == 7) {
                ds->seq[idx] = 0xffff;
                ds->readIdx = (ds->readIdx + 1) & 3;
            } else if (r != 0 && r != 2) {
                ds->state = 5;
                return 1;
            }
        }
        if (ds->unk80c != ds->readIdx) {
            ds->buf[ds->unk80c].bitmap0 |= 1;
            MIi_CpuCopy16(&ds->buf[ds->unk80c], recvSet, 0x200);
            ok = TRUE;
            result = 0;
            ds->unk81a = ds->seq[ds->unk80c];
            ds->unk80c = (ds->unk80c + 1) & 3;
            if (ds->doubleMode == 0 && childBitmap != 0 && ds->buf[ds->writeIdx].bitmap0 == 1) {
                force = TRUE;
            } else {
                force = FALSE;
            }
        }
        func_02011ff4(ds, FALSE);
        if (ok) {
            WmDataSharingReceiveData(ds, 0, send);
            if (ds->doubleMode == 0) {
                func_02011ff4(ds, force);
            }
        }
    } else {
        ok = FALSE;
        if (state == 4) {
            ds->state = 1;
            ok = TRUE;
        } else if (ds->unk80c != ds->writeIdx) {
            if (!(ds->buf[ds->unk80c].bitmap0 & 1)) {
                ds->buf[ds->unk80c].bitmap0 |= 1;
            } else {
                MIi_CpuCopy16(&ds->buf[ds->unk80c], recvSet, 0x200);
                ok = TRUE;
                result = 0;
                ds->unk81a = ds->seq[ds->unk80c];
                ds->unk80c = (ds->unk80c + 1) & 3;
            }
        }
        if (ok) {
            u8 *dst = (u8 *)&ds->buf[ds->readIdx] + 0x20;
            MIi_CpuCopy16(send, dst, ds->dataLength);
            r = func_02011c44(func_020124d0, ds, dst, ds->dataLength, ds->aidBitmap, ds->port, 1);
            ds->readIdx = (ds->readIdx + 1) & 3;
            if (r != 2 && r != 0) {
                ds->state = 5;
                result = 1;
            }
        }
    }
    return result;
}

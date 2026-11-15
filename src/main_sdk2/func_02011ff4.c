#include <nitro/types.h>
#include <main_sdk2/wmds.h>
#include <main_sdk2/wm.h>

void DC_InvalidateRange(void *p, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
s32 func_02011c44(void *cb, void *arg, void *data, u16 size, u16 dest, u16 port, u16 prio);
void func_020124d0(void *cb);

void func_02011ff4(UnkDS *ds, BOOL force)
{
    u8 *p = ((UnkWmWork *)func_02010b70())->unk4;
    u32 e = OS_DisableInterrupts();
    u16 idx;
    u16 childBitmap;
    u16 next;
    u16 clearIdx;
    s32 r;

    if (ds->buf[ds->writeIdx].bitmap0 == 0) {
        DC_InvalidateRange(p + 0x86, 2);
        idx = ds->writeIdx;
        childBitmap = *(u16 *)(p + 0x86);
        next = (idx + 1) & 3;
        if (ds->doubleMode == 1) {
            clearIdx = (next + 1) & 3;
        } else {
            clearIdx = next;
        }
        {
            vu16 z = 0;
            MIi_CpuClear16(z, &ds->buf[clearIdx], 0x200);
        }
        ds->buf[clearIdx].bitmap0 = ds->aidBitmap & (childBitmap | 1);
        ds->writeIdx = next;
        ds->buf[idx].bitmap0 = ds->aidBitmap;
        if (force == TRUE) {
            ds->buf[idx].bitmap0 &= ~1;
        }
        OS_RestoreInterrupts(e);
        r = func_02011c44(func_020124d0, ds, &ds->buf[idx], ds->setLength, ds->aidBitmap & childBitmap, ds->port, 1);
        if (r == 7) {
            ds->seq[idx] = 0xffff;
            ds->readIdx = (ds->readIdx + 1) & 3;
            return;
        }
        if (r == 0) {
            return;
        }
        if (r != 2) {
            ds->state = 5;
        }
    } else {
        OS_RestoreInterrupts(e);
    }
}

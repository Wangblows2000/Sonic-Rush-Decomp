#include <nitro/types.h>
#include <main_sdk2/mbseg.h>

void OS_Terminate(void);

void MBi_SetSegmentInfo(u8 *rom, u32 *type, UnkMbSeg *seg, u32 *cur)
{
    switch (*type) {
    case 0: {
        u32 *a = (u32 *)(rom + 0x28);
        if (a[0] >= 0x02000000 && a[0] < 0x022c0000 && a[0] + a[1] <= 0x022c0000) {
            seg->size = a[1];
            seg->addr = a[0];
            seg->src = seg->addr;
            seg->flag = FALSE;
            return;
        }
        OS_Terminate();
        return;
    }
    case 1: {
        BOOL err = FALSE;
        BOOL useCur = FALSE;
        u32 *a = (u32 *)(rom + 0x38);
        u32 end;
        u32 size;
        u32 addr;
        addr = a[0];
        size = a[1];
        end = addr + size;
        if (addr >= 0x02000000 && addr < 0x023fe800) {
            if (end > 0x02300000) {
                if (end < 0x023fe800 && size <= 0x40000) {
                    useCur = TRUE;
                } else {
                    err = TRUE;
                }
            }
        } else if (addr >= 0x037f8000 && addr < 0x0380f000) {
            if (end <= 0x0380f000) {
                useCur = TRUE;
            } else {
                err = TRUE;
            }
        } else {
            err = TRUE;
        }
        if (err == TRUE) {
            OS_Terminate();
        }
        seg->size = a[1];
        seg->addr = a[0];
        if (useCur == FALSE) {
            seg->src = seg->addr;
        } else {
            seg->src = *cur;
            *cur = *cur + seg->size;
        }
        seg->flag = TRUE;
        return;
    }
    case 2:
        seg->size = 0x160;
        seg->addr = 0x027ffe00;
        seg->src = seg->addr;
        seg->flag = FALSE;
        return;
    }
}

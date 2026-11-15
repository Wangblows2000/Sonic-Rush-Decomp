#include <nitro/types.h>
#include <main_sdk2/wh.h>
#include <main_sdk2/mbseg.h>

BOOL func_02016b98(s32 i, u32 addr, u32 size);

BOOL func_02016e30(UnkMbSegInfo *info, UnkMbHeader *h)
{
    u16 *blk = info->blk;
    u32 sum = 0;
    u8 i;
    if (h == NULL) {
        return FALSE;
    }
    for (i = 0; i < 3; i++) {
        info->offset[i] = sum;
        sum += h->seg[i].size;
    }
    blk[0] = 0;
    for (i = 0; i < 3; i++) {
        UnkMbSeg *seg = &h->seg[i];
        u32 d = data_02080f1c->unk1318;
        u16 total = blk[i] + (u16)((seg->size + d - 1) / d);
        if (!func_02016b98(i, seg->addr, seg->size)) {
            return FALSE;
        }
        if (i < 2) {
            blk[i + 1] = total;
        } else {
            info->total = total;
        }
    }
    return TRUE;
}

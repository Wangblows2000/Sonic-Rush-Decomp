#include <nitro/types.h>
#include <NitroSDK/wh.h>
#include <NitroSDK/mbseg.h>

BOOL MBi_get_blockinfo(UnkMbSegPos *out, UnkMbSegInfo *info, u32 idx, UnkMbHeader *h)
{
    s8 i;
    u32 rel;
    UnkMbSeg *seg;
    if (idx >= info->total) {
        return FALSE;
    }
    for (i = 2; i >= 0; i--) {
        if (idx >= info->blk[i]) {
            break;
        }
    }
    if (i < 0) {
        return FALSE;
    }
    rel = idx - info->blk[i];
    rel *= data_02080f1c->unk1318;
    seg = &h->seg[i];
    out->size = seg->size - rel;
    if (out->size > data_02080f1c->unk1318) {
        out->size = data_02080f1c->unk1318;
    }
    out->offset = info->offset[i] + rel;
    out->src = rel + seg->src;
    out->seg = i;
    return TRUE;
}

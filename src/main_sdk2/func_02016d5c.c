#include <nitro/types.h>
#include <main_sdk2/wh.h>
#include <main_sdk2/mbseg.h>

BOOL func_02016d5c(UnkMbSegPos *out, UnkMbSegInfo *info, u32 idx, UnkMbHeader *h)
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

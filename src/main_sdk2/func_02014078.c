#include <nitro/types.h>
#include <main_sdk2/blkinfo.h>

s32 func_0201567c(void);
void MI_CpuCopy8(const void *src, void *dest, u32 size);

BOOL func_02014078(UnkBlockInfo *info, s32 idx, const void *src)
{
    s32 blk = func_0201567c();
    u8 *base = info->base;

    if (idx < info->count) {
        if (idx == info->count - 1) {
            s32 rem = info->size % blk;
            if (rem != 0) {
                MI_CpuCopy8(src, base + idx * blk, rem);
            } else {
                MI_CpuCopy8(src, base + idx * blk, blk);
            }
        } else {
            MI_CpuCopy8(src, base + idx * blk, blk);
        }
    } else {
        return FALSE;
    }
    return TRUE;
}

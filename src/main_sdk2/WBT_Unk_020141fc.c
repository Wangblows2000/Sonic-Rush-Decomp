#include <nitro/types.h>
#include <main_sdk2/blkinfo.h>

s32 WBT_Unk_0201567c(void);
u32 WBT_CalcPacketbitmapSize(s32 size);
void MI_CpuFill8(void *dest, u8 data, u32 size);

void WBT_Unk_020141fc(UnkBlockInfo *info, s32 size, u32 *bits, u8 *base)
{
    s32 blk = WBT_Unk_0201567c();
    info->size = size;
    info->last = 0;
    info->cursor = 0;
    info->unk10 = 0;
    info->count = size / blk + (size % blk != 0 ? 1 : 0);
    info->bits = bits;
    info->base = base;
    MI_CpuFill8(bits, 0, WBT_CalcPacketbitmapSize(size));
}

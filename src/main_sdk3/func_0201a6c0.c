#include <nitro/types.h>
#include <main_sdk2/blk2.h>

void MI_CpuCopy8(const void *src, void *dest, u32 size);
BOOL func_0201a660(u16 aid);

u8 *func_0201a6c0(u8 *packet, u16 aid)
{
    u8 seq;
    s32 i;

    if (data_02080f44 == NULL) {
        return NULL;
    }
    seq = packet[2];
    if (seq > data_02080f48.count) {
        return NULL;
    }
    i = aid - 1;
    MI_CpuCopy8(packet + 3, data_02080f44->data[i] + seq * data_02080f48.size, data_02080f48.size);
    data_02080f44->recvMask[i] |= 1 << seq;
    if (func_0201a660(aid)) {
        return data_02080f44->data[i];
    }
    return NULL;
}

#include <nitro/types.h>
#include <main_sdk2/wm.h>

void DC_InvalidateRange(void *p, u32 size);

typedef struct {
    u8 pad0[4];
    u16 aid;
} UnkMpRecvData;

typedef struct {
    u8 pad0[4];
    vu16 count;
    u16 length;
    u8 pad8[2];
    u8 data[1];
} UnkMpRecvHeader;

UnkMpRecvData *func_02011210(UnkMpRecvHeader *hdr, u16 aid)
{
    UnkWmWork *w = func_02010b70();
    UnkMpRecvData *ptrs[16];
    int i;

    if (func_02010b54() != 0) {
        return NULL;
    }
    if (aid < 1 || aid > 15) {
        return NULL;
    }
    DC_InvalidateRange(w->unk4 + 0x17e, 2);
    if (!(*(u16 *)(w->unk4 + 0x17e) & (1 << aid))) {
        return NULL;
    }
    if (hdr->count == 0) {
        return NULL;
    }
    ptrs[0] = (UnkMpRecvData *)hdr->data;
    i = 0;
    do {
        if (aid == ptrs[i]->aid) {
            return ptrs[i];
        }
        i++;
        ptrs[i] = (UnkMpRecvData *)((u32)ptrs[i - 1] + hdr->length);
    } while (i < hdr->count);
    return NULL;
}

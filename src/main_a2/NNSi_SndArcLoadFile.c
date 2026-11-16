#include <nitro/types.h>

u32 NNS_SndArcGetFileSize(u32 fileId);
void *NNS_SndHeapAlloc(void *heap, u32 size, void *cb, void *arg, u32 c);
s32 NNS_SndArcReadFile(u32 idx, void *buf, u32 size, u32 pos);
void DC_StoreRange(void *p, u32 size);

void *NNSi_SndArcLoadFile(u32 fileId, void *cb, void *arg, u32 c, void *heap) {
    u32 size;
    void *mem;
    size = NNS_SndArcGetFileSize(fileId);
    if (size == 0) return NULL;
    if (heap == NULL) return NULL;
    mem = NNS_SndHeapAlloc(heap, size + 0x20, cb, arg, c);
    if (mem == NULL) return NULL;
    if (size != NNS_SndArcReadFile(fileId, mem, size, 0)) return NULL;
    DC_StoreRange(mem, size);
    return mem;
}

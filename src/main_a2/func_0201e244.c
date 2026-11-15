#include <nitro/types.h>

u32 func_0201d1c4(u32 fileId);
void *func_0201d9e4(void *heap, u32 size, void *cb, void *arg, u32 c);
s32 func_0201d140(u32 idx, void *buf, u32 size, u32 pos);
void DC_StoreRange(void *p, u32 size);

void *func_0201e244(u32 fileId, void *cb, void *arg, u32 c, void *heap) {
    u32 size;
    void *mem;
    size = func_0201d1c4(fileId);
    if (size == 0) return NULL;
    if (heap == NULL) return NULL;
    mem = func_0201d9e4(heap, size + 0x20, cb, arg, c);
    if (mem == NULL) return NULL;
    if (size != func_0201d140(fileId, mem, size, 0)) return NULL;
    DC_StoreRange(mem, size);
    return mem;
}

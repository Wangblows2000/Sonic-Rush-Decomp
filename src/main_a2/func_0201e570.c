#include <nitro/types.h>

typedef struct { u32 fileId; u16 bankNo; } UnkSeqInfo_0201e570;

UnkSeqInfo_0201e570 *func_0201d4d0(s32 idx);
s32 func_0201e3a0(s32 idx, u32 flags, void *heap, BOOL cache, void **out);
void *func_0201e1c8(u32 fileId, void *heap, BOOL cache);
void *func_0201d0f8(u32 fileId);

s32 func_0201e570(s32 idx, u32 flags, void *heap, BOOL cache, void **out) {
    UnkSeqInfo_0201e570 *info;
    void *p;
    s32 r;
    info = func_0201d4d0(idx);
    if (info == NULL) return 2;
    r = func_0201e3a0(info->bankNo, flags, heap, cache, NULL);
    if (r != 0) return r;
    if (flags & 1) {
        p = func_0201e1c8(info->fileId, heap, cache);
        if (p == NULL) return 6;
    } else {
        p = func_0201d0f8(info->fileId);
    }
    if (out != NULL) *out = p;
    return 0;
}

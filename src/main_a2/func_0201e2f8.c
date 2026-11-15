#include <nitro/types.h>

typedef struct { u32 fileId : 24; u32 flags : 8; } UnkInfo_0201e2f8;

UnkInfo_0201e2f8 *func_0201d3a4(s32 idx);
void *func_0201df20(u32 fileId, void *heap, BOOL cache);
void *func_0201e054(u32 fileId, void *heap, BOOL cache);
void *func_0201d0f8(u32 fileId);

s32 func_0201e2f8(s32 idx, u32 flags, void *heap, BOOL cache, void **out) {
    UnkInfo_0201e2f8 *info = func_0201d3a4(idx);
    void *p;
    if (info == NULL) return 5;
    if (flags & 4) {
        if (info->flags & 1) {
            p = func_0201df20(info->fileId, heap, cache);
        } else {
            p = func_0201e054(info->fileId, heap, cache);
        }
        if (p == NULL) return 9;
    } else {
        p = func_0201d0f8(info->fileId);
    }
    if (out != NULL) *out = p;
    return 0;
}

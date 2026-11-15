#include <nitro/types.h>

typedef struct { u8 type; u8 loadFlag; u16 pad; u32 index; } UnkGroupItem_0201e61c;
typedef struct { u32 count; UnkGroupItem_0201e61c items[1]; } UnkGroup_0201e61c;

UnkGroup_0201e61c *func_0201d214(s32 idx);
s32 func_0201e570(s32 idx, u32 flags, void *heap, BOOL cache, void **out);
s32 func_0201e500(s32 idx, u32 flags, void *heap, BOOL cache, void **out);
s32 func_0201e3a0(s32 idx, u32 flags, void *heap, BOOL cache, void **out);
s32 func_0201e2f8(s32 idx, u32 flags, void *heap, BOOL cache, void **out);

s32 func_0201e61c(s32 idx, void *heap) {
    UnkGroup_0201e61c *g;
    u32 i;
    s32 r;
    g = func_0201d214(idx);
    if (g == NULL) return 1;
    for (i = 0; i < g->count; i++) {
        UnkGroupItem_0201e61c *it = &g->items[i];
        switch (it->type) {
        case 0:
            r = func_0201e570(it->index, it->loadFlag, heap, TRUE, NULL);
            if (r != 0) return r;
            break;
        case 3:
            r = func_0201e500(it->index, it->loadFlag, heap, TRUE, NULL);
            if (r != 0) return r;
            break;
        case 1:
            r = func_0201e3a0(it->index, it->loadFlag, heap, TRUE, NULL);
            if (r != 0) return r;
            break;
        case 2:
            r = func_0201e2f8(it->index, it->loadFlag, heap, TRUE, NULL);
            if (r != 0) return r;
            break;
        }
    }
    return 0;
}

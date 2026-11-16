#include <nitro/types.h>

typedef struct UnkObj_020214d0 UnkObj_020214d0;
typedef struct { void (*fn0)(UnkObj_020214d0 *, u32); void (*fn4)(UnkObj_020214d0 *, u32); } UnkVtbl_020214d0;
struct UnkObj_020214d0 { UnkVtbl_020214d0 *vtbl; };

void NNS_FndAllocFromAllocator(UnkObj_020214d0 *obj, u32 a) {
    obj->vtbl->fn0(obj, a);
}

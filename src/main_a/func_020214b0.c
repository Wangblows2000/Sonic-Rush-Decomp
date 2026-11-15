#include <nitro/types.h>

typedef struct UnkObj_020214b0 UnkObj_020214b0;
typedef struct { void (*fn0)(UnkObj_020214b0 *, u32); void (*fn4)(UnkObj_020214b0 *, u32); } UnkVtbl_020214b0;
struct UnkObj_020214b0 { UnkVtbl_020214b0 *vtbl; };

void func_020214b0(UnkObj_020214b0 *obj, u32 a) {
    obj->vtbl->fn4(obj, a);
}

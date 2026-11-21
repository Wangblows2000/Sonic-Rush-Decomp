#include <nitro/types.h>

u32 NNS_G3dAnmObjCalcSizeRequired(u32 a, u32 b);
void NNS_FndAllocFromAllocator(void *obj, u32 a);

void NNS_G3dAllocAnmObj(void *obj, u32 a, u32 b) {
    NNS_FndAllocFromAllocator(obj, NNS_G3dAnmObjCalcSizeRequired(a, b));
}

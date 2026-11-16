#include <nitro/types.h>

u32 NNS_G3dAnmObjCalcSizeRequired(u32 a, u32 b);
void func_020214d0(void *obj, u32 a);

void NNS_G3dAllocAnmObj(void *obj, u32 a, u32 b) {
    func_020214d0(obj, NNS_G3dAnmObjCalcSizeRequired(a, b));
}

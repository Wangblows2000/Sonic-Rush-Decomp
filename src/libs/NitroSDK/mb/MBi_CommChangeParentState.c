#include <nitro/types.h>

BOOL IsChildAidValid(u32 v);
void MBi_CommChangeParentStateCallbackOnly(u32 a, u32 b, void *c);
extern u8 *data_02080f1c;
void MBi_CommChangeParentState(u32 a, u32 b, void *c) {
    if (IsChildAidValid(a)) *(u32 *)(data_02080f1c + (a - 1) * 4 + 0x14e8) = b;
    MBi_CommChangeParentStateCallbackOnly(a, b, c);
}

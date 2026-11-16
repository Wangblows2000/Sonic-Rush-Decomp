#include <nitro/types.h>

u32 NNS_FndGetAllocatableSizeForFrmHeapEx(void *heap, s32 align);

u32 func_0201d9b4(void **p) {
    u32 r = NNS_FndGetAllocatableSizeForFrmHeapEx(*p, 0x20);
    if (r < 0x20) return 0;
    return (r - 0x20) & ~0x1F;
}

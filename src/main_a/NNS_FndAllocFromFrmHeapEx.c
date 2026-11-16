#include <nitro/types.h>

void *func_02021194(void *hd, u32 size, s32 align);
void *func_02021118(void *hd, u32 size, s32 align);

void *NNS_FndAllocFromFrmHeapEx(u8 *heap, u32 size, s32 align) {
    void *hd = heap + 0x24;
    if (size == 0) size = 1;
    size = (size + 3) & ~3;
    if (align >= 0) {
        return func_02021194(hd, size, align);
    } else {
        return func_02021118(hd, size, -align);
    }
}

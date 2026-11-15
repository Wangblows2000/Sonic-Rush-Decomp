#include <nitro/types.h>

void *func_02020c1c(void *heap, u32 size, s32 align);
void *func_02020b58(void *heap, u32 size, s32 align);

void *func_020209d0(void *heap, u32 size, s32 align) {
    if (size == 0) size = 1;
    size = (size + 3) & ~3;
    if (align >= 0) {
        return func_02020c1c(heap, size, align);
    } else {
        return func_02020b58(heap, size, -align);
    }
}

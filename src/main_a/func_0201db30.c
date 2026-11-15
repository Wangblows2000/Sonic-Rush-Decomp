#include <nitro/types.h>

void *func_02021084(void *start, u32 size, u16 opt);
BOOL func_0201d964(void *s, void *heap);
void func_02021078(void *heap);

void *func_0201db30(u8 *mem, u32 size) {
    u8 *end = mem + size;
    u8 *start = (u8 *)(((u32)mem + 3) & ~3);
    u32 n;
    void *heap;
    if (start > end) return NULL;
    n = end - start;
    if (n < 0x10) return NULL;
    heap = func_02021084(start + 0x10, n - 0x10, 0);
    if (heap == NULL) return NULL;
    if (func_0201d964(start, heap)) return start;
    func_02021078(heap);
    return NULL;
}

#include <nitro/types.h>

BOOL func_020209d0(void *a, void *b, void *c);
void func_02006abc(u32 a);

void Heap_Alloc(void *a, void *b, void *c) {
    if (func_020209d0(a, b, c)) return;
    func_02006abc(0);
}

#include <nitro/types.h>

typedef struct { void *alloc; void *free; } UnkFuncs_02034bc0;
typedef struct { UnkFuncs_02034bc0 *funcs; void *heap; u32 align; u32 unkC; } UnkAllocator_02034bc0;
extern u8 data_020aff00[];
extern void *gMainHeap;
extern UnkAllocator_02034bc0 gMainHeapAllocator;
extern UnkFuncs_02034bc0 data_0208fe8c;
void *func_02020a14(void *start, u32 size, u16 opt);
u16 func_02020960(void *heap, u32 v);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void Heap_AllocatorAlloc(void);
void Heap_AllocatorFree(void);

void Heap_InitMain(void) {
    vu32 z0, z1;
    gMainHeap = func_02020a14(data_020aff00, 0x1A0000, 0);
    func_02020960(gMainHeap, 1);
    z0 = 0;
    MIi_CpuClear32(z0, &gMainHeapAllocator, sizeof(UnkAllocator_02034bc0));
    z1 = 0;
    MIi_CpuClear32(z1, &data_0208fe8c, sizeof(UnkFuncs_02034bc0));
    data_0208fe8c.alloc = Heap_AllocatorAlloc;
    data_0208fe8c.free = Heap_AllocatorFree;
    gMainHeapAllocator.funcs = &data_0208fe8c;
    gMainHeapAllocator.heap = &gMainHeap;
    gMainHeapAllocator.align = 0x20;
}

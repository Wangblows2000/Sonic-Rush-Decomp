#include <nitro/types.h>

typedef struct { void *alloc; void *free; } UnkFuncs_02034a70;
typedef struct { UnkFuncs_02034a70 *funcs; void *heap; u32 align; u32 unkC; } UnkAllocator_02034a70;

extern void *gITCMHeap;
extern UnkAllocator_02034a70 gITCMHeapAllocator;
extern UnkFuncs_02034a70 data_0208fe94;
void *func_02020a14(void *start, u32 size, u16 opt);
u16 func_02020960(void *heap, u32 v);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void Heap_AllocatorAlloc(void);
void Heap_AllocatorFree(void);

void Heap_InitITCM(void) {
    vu32 z0, z1;
    gITCMHeap = func_02020a14((void *)0x01FF8760, 0x7000, 0);
    func_02020960(gITCMHeap, 1);
    z0 = 0;
    MIi_CpuClear32(z0, &gITCMHeapAllocator, sizeof(UnkAllocator_02034a70));
    z1 = 0;
    MIi_CpuClear32(z1, &data_0208fe94, sizeof(UnkFuncs_02034a70));
    data_0208fe94.alloc = Heap_AllocatorAlloc;
    data_0208fe94.free = Heap_AllocatorFree;
    gITCMHeapAllocator.funcs = &data_0208fe94;
    gITCMHeapAllocator.heap = &gITCMHeap;
    gITCMHeapAllocator.align = 4;
}

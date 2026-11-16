#include <nitro/types.h>

typedef struct { void *alloc; void *free; } UnkFuncs_02034b18;
typedef struct { UnkFuncs_02034b18 *funcs; void *heap; u32 align; u32 unkC; } UnkAllocator_02034b18;

extern u8 data_027e0060[];
extern void *gDTCMHeap;
extern UnkAllocator_02034b18 gDTCMHeapAllocator;
extern UnkFuncs_02034b18 data_0208fe9c;
void *func_02020a14(void *start, u32 size, u16 opt);
u16 func_02020960(void *heap, u32 v);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void Heap_AllocatorAlloc(void);
void Heap_AllocatorFree(void);

void Heap_InitDTCM(void) {
    vu32 z0, z1;
    gDTCMHeap = func_02020a14((void *)data_027e0060, 0x2000, 0);
    func_02020960(gDTCMHeap, 1);
    z0 = 0;
    MIi_CpuClear32(z0, &gDTCMHeapAllocator, sizeof(UnkAllocator_02034b18));
    z1 = 0;
    MIi_CpuClear32(z1, &data_0208fe9c, sizeof(UnkFuncs_02034b18));
    data_0208fe9c.alloc = Heap_AllocatorAlloc;
    data_0208fe9c.free = Heap_AllocatorFree;
    gDTCMHeapAllocator.funcs = &data_0208fe9c;
    gDTCMHeapAllocator.heap = &gDTCMHeap;
    gDTCMHeapAllocator.align = 4;
}

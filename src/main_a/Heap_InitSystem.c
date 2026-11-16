#include <nitro/types.h>

typedef struct { void *alloc; void *free; } UnkFuncs_02034c68;
typedef struct { UnkFuncs_02034c68 *funcs; void *heap; u32 align; u32 unkC; } UnkAllocator_02034c68;
extern u8 data_0208ff00[];
extern void *gSystemHeap;
extern UnkAllocator_02034c68 gSystemHeapAllocator;
extern UnkFuncs_02034c68 data_0208fe84;
void *NNS_FndCreateExpHeapEx(void *start, u32 size, u16 opt);
u16 NNS_FndSetAllocModeForExpHeap(void *heap, u32 v);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void Heap_AllocatorAlloc(void);
void Heap_AllocatorFree(void);

void Heap_InitSystem(void) {
    vu32 z0, z1;
    gSystemHeap = NNS_FndCreateExpHeapEx(data_0208ff00, 0x20000, 0);
    NNS_FndSetAllocModeForExpHeap(gSystemHeap, 1);
    z0 = 0;
    MIi_CpuClear32(z0, &gSystemHeapAllocator, sizeof(UnkAllocator_02034c68));
    z1 = 0;
    MIi_CpuClear32(z1, &data_0208fe84, sizeof(UnkFuncs_02034c68));
    data_0208fe84.alloc = Heap_AllocatorAlloc;
    data_0208fe84.free = Heap_AllocatorFree;
    gSystemHeapAllocator.funcs = &data_0208fe84;
    gSystemHeapAllocator.heap = &gSystemHeap;
    gSystemHeapAllocator.align = 0x20;
}

#include <nitro/types.h>

typedef struct { void *alloc; void *free; } UnkFuncs_02034b18;
typedef struct { UnkFuncs_02034b18 *funcs; void *heap; u32 align; u32 unkC; } UnkAllocator_02034b18;

extern void *data_0208fe74;
extern UnkAllocator_02034b18 data_0208feb4;
extern UnkFuncs_02034b18 data_0208fe9c;
void *func_02020a14(void *start, u32 size, u16 opt);
u16 func_02020960(void *heap, u32 v);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_020349a0(void);
void func_0203498c(void);

void func_02034b18(void) {
    vu32 z0, z1;
    data_0208fe74 = func_02020a14((void *)0x027E0060, 0x2000, 0);
    func_02020960(data_0208fe74, 1);
    z0 = 0;
    MIi_CpuClear32(z0, &data_0208feb4, sizeof(UnkAllocator_02034b18));
    z1 = 0;
    MIi_CpuClear32(z1, &data_0208fe9c, sizeof(UnkFuncs_02034b18));
    data_0208fe9c.alloc = func_020349a0;
    data_0208fe9c.free = func_0203498c;
    data_0208feb4.funcs = &data_0208fe9c;
    data_0208feb4.heap = &data_0208fe74;
    data_0208feb4.align = 4;
}

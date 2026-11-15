#include <nitro/types.h>

typedef struct { void *alloc; void *free; } UnkFuncs_02034a70;
typedef struct { UnkFuncs_02034a70 *funcs; void *heap; u32 align; u32 unkC; } UnkAllocator_02034a70;

extern void *data_0208fe7c;
extern UnkAllocator_02034a70 data_0208fea4;
extern UnkFuncs_02034a70 data_0208fe94;
void *func_02020a14(void *start, u32 size, u16 opt);
u16 func_02020960(void *heap, u32 v);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_020349a0(void);
void func_0203498c(void);

void func_02034a70(void) {
    vu32 z0, z1;
    data_0208fe7c = func_02020a14((void *)0x01FF8760, 0x7000, 0);
    func_02020960(data_0208fe7c, 1);
    z0 = 0;
    MIi_CpuClear32(z0, &data_0208fea4, sizeof(UnkAllocator_02034a70));
    z1 = 0;
    MIi_CpuClear32(z1, &data_0208fe94, sizeof(UnkFuncs_02034a70));
    data_0208fe94.alloc = func_020349a0;
    data_0208fe94.free = func_0203498c;
    data_0208fea4.funcs = &data_0208fe94;
    data_0208fea4.heap = &data_0208fe7c;
    data_0208fea4.align = 4;
}

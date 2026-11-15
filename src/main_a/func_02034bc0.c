#include <nitro/types.h>

typedef struct { void *alloc; void *free; } UnkFuncs_02034bc0;
typedef struct { UnkFuncs_02034bc0 *funcs; void *heap; u32 align; u32 unkC; } UnkAllocator_02034bc0;
extern u8 data_020aff00[];
extern void *data_0208fe80;
extern UnkAllocator_02034bc0 data_0208fec4;
extern UnkFuncs_02034bc0 data_0208fe8c;
void *func_02020a14(void *start, u32 size, u16 opt);
u16 func_02020960(void *heap, u32 v);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_020349a0(void);
void func_0203498c(void);

void func_02034bc0(void) {
    vu32 z0, z1;
    data_0208fe80 = func_02020a14(data_020aff00, 0x1A0000, 0);
    func_02020960(data_0208fe80, 1);
    z0 = 0;
    MIi_CpuClear32(z0, &data_0208fec4, sizeof(UnkAllocator_02034bc0));
    z1 = 0;
    MIi_CpuClear32(z1, &data_0208fe8c, sizeof(UnkFuncs_02034bc0));
    data_0208fe8c.alloc = func_020349a0;
    data_0208fe8c.free = func_0203498c;
    data_0208fec4.funcs = &data_0208fe8c;
    data_0208fec4.heap = &data_0208fe80;
    data_0208fec4.align = 0x20;
}

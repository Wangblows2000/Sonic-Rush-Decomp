#include <nitro/types.h>

typedef struct { void *alloc; void *free; } UnkFuncs_02034c68;
typedef struct { UnkFuncs_02034c68 *funcs; void *heap; u32 align; u32 unkC; } UnkAllocator_02034c68;
extern u8 data_0208ff00[];
extern void *data_0208fe78;
extern UnkAllocator_02034c68 data_0208fed4;
extern UnkFuncs_02034c68 data_0208fe84;
void *func_02020a14(void *start, u32 size, u16 opt);
u16 func_02020960(void *heap, u32 v);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_020349a0(void);
void func_0203498c(void);

void func_02034c68(void) {
    vu32 z0, z1;
    data_0208fe78 = func_02020a14(data_0208ff00, 0x20000, 0);
    func_02020960(data_0208fe78, 1);
    z0 = 0;
    MIi_CpuClear32(z0, &data_0208fed4, sizeof(UnkAllocator_02034c68));
    z1 = 0;
    MIi_CpuClear32(z1, &data_0208fe84, sizeof(UnkFuncs_02034c68));
    data_0208fe84.alloc = func_020349a0;
    data_0208fe84.free = func_0203498c;
    data_0208fed4.funcs = &data_0208fe84;
    data_0208fed4.heap = &data_0208fe78;
    data_0208fed4.align = 0x20;
}

#include <nitro/types.h>

void IC_InvalidateRange(void *p, u32 size);
void DC_InvalidateRange(void *p, u32 size);
void MI_CpuFill8(void *dst, u8 v, u32 size);
typedef struct { u32 id; u8 *addr; u32 ramSize; u32 bssSize; } UnkStruct_0200c850;
void func_0200c850(UnkStruct_0200c850 *p) {
    u8 *addr = p->addr;
    u32 ram = p->ramSize;
    u32 total = ram + p->bssSize;
    IC_InvalidateRange(addr, total);
    DC_InvalidateRange(addr, total);
    MI_CpuFill8(addr + ram, 0, total - ram);
}

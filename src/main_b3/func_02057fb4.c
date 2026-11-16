#include <nitro/types.h>

typedef struct { u16 v[8]; } UnkMosaic_02057fb4;
extern UnkMosaic_02057fb4 data_0208922c;

void MIi_CpuClear32(u32 data, void *dest, u32 size);

#pragma thumb on
static inline u32 GetPlane_02057fb4(void) {
    return (*(vu32 *)0x04000000 & 0x1f00) >> 8;
}

static inline void SetPlane_02057fb4(u32 v) {
    *(vu32 *)0x04000000 = (*(vu32 *)0x04000000 & ~0x1f00) | (v << 8);
}

static inline void Clear32_02057fb4(void *dst, u32 size) {
    vu32 z = 0;
    MIi_CpuClear32(z, dst, size);
}

void func_02057fb4(void) {
    SetPlane_02057fb4(GetPlane_02057fb4() & ~8);
    Clear32_02057fb4((void *)0x0600c000, 0x4000);
    Clear32_02057fb4((void *)0x06000000, 0x1000);
    Clear32_02057fb4((void *)0x06200000, 0x3000);
    data_0208922c.v[6] = data_0208922c.v[7] = 0;
}
#pragma thumb reset

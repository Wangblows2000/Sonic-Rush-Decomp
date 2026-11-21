#include <nitro/types.h>

void GX_SetGraphicsMode(s32 a, s32 b, s32 c);
void MIi_CpuClear32(u32 data, void *dest, u32 size);

#pragma thumb on
static inline u32 GetPlane_022e92b4(void) {
    return (*(vu32 *)0x4000000 & 0x1f00) >> 8;
}

static inline void SetPlane_022e92b4(u32 plane) {
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | (plane << 8);
}

void ZoneMapPause_ResetDisplay(void) {
    vu32 z;

    GX_SetGraphicsMode(1, 5, 1);
    SetPlane_022e92b4(GetPlane_022e92b4() & ~4);
    z = 0;
    MIi_CpuClear32(z, (void *)0x6002000, 0x1000);
}
#pragma thumb reset

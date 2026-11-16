#include <nitro/types.h>

#define REG_DIVCNT (*(vu16 *)0x04000280)
#define REG_DIV_NUMER (*(vu32 *)0x04000290)
#define REG_DIV_DENOM (*(volatile u64 *)0x04000298)
#define REG_DIV_RESULT_S16 (*(volatile s16 *)0x040002a0)

static inline void CP_SetDiv32_32(u32 numer, u64 denom) {
    REG_DIVCNT = 0;
    REG_DIV_NUMER = numer;
    REG_DIV_DENOM = denom;
}

static inline void CP_WaitDiv(void) {
    while (REG_DIVCNT & 0x8000)
        ;
}

static inline s32 CP_GetDivResult_020490b8(void) {
    return REG_DIV_RESULT_S16;
}

void Math_LerpRGB555(u16 *out, u16 c1, u16 c2, s32 total, s32 t) {
    u16 r;
    u16 g;
    u16 b;
    s32 w;
    u64 d;
    d = (u32)total;
    w = total - t;

    CP_SetDiv32_32((c1 & 0x1f) * w + (c2 & 0x1f) * t, d);
    CP_WaitDiv();
    r = (s16)CP_GetDivResult_020490b8();
    CP_SetDiv32_32(((c1 & 0x3e0) >> 5) * w + ((c2 & 0x3e0) >> 5) * t, d);
    CP_WaitDiv();
    g = (s16)CP_GetDivResult_020490b8();
    CP_SetDiv32_32(((c1 & 0x7c00) >> 10) * w + ((c2 & 0x7c00) >> 10) * t, d);
    CP_WaitDiv();
    b = (s16)CP_GetDivResult_020490b8();
    *out = (r & 0x1f) | ((g << 5) & 0x3e0) | ((b << 10) & 0x7c00);
}

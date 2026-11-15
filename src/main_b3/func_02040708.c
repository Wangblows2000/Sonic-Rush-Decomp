#include <nitro/types.h>

typedef struct { u32 flags; u32 frame; s32 x; s32 y; u32 z; s32 e0; s32 e1; u8 pad1C[4]; } UnkKey_02040708;
typedef struct { s32 x; s32 y; u32 z; } UnkOut_02040708;

s32 func_0204069c(s32 t, s32 e);

#pragma thumb on
static inline s32 FX_Mul_02040708(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_02040708(UnkKey_02040708 *k0, UnkKey_02040708 *k1, s32 ratio, UnkOut_02040708 *out, u32 flags) {
    s32 e0;
    s32 e1;

    if (ratio >= 0x1000) {
        out->x = k1->x;
        out->y = k1->y;
        out->z = k1->z;
        return;
    }
    if (ratio <= 0 || !(k0->flags & 1)) {
        out->x = k0->x;
        out->y = k0->y;
        out->z = k0->z;
        return;
    }
    e0 = k0->e0;
    if (flags & 4)
        e0 = 0x1000;
    else if (flags & 0x10)
        e0 = 0x2000 - e0;
    e1 = k0->e1;
    if (flags & 4)
        e1 = 0x1000;
    else if (flags & 0x10)
        e1 = 0x2000 - e1;
    if (e0 != 0x1000)
        func_0204069c(ratio, e0);
    if (e1 != 0x1000 && e1 != e0)
        func_0204069c(ratio, e1);
    out->x = k0->x + FX_Mul_02040708(k1->x - k0->x, ratio);
    out->y = k0->y + FX_Mul_02040708(k1->y - k0->y, ratio);
    out->z = (k0->z + FX_Mul_02040708((k1->z - k0->z) << 12, ratio)) >> 12;
}
#pragma thumb reset

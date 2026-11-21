#include <nitro/types.h>

typedef struct { u32 flags; u32 frame; s32 x; s32 y; s32 e0; } UnkKey_02040848;
typedef struct { s32 x; s32 y; } UnkOut_02040848;

s32 KeyframeAnim_EaseCurve(s32 t, s32 e);

#pragma thumb on
static inline s32 FX_Mul_02040848(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void KeyframeAnim_LerpPosition(UnkKey_02040848 *k0, UnkKey_02040848 *k1, s32 ratio, UnkOut_02040848 *out, u32 flags) {
    s32 e0;

    if (ratio >= 0x1000) {
        out->x = k1->x;
        out->y = k1->y;
        return;
    }
    if (ratio <= 0 || !(k0->flags & 1)) {
        out->x = k0->x;
        out->y = k0->y;
        return;
    }
    e0 = k0->e0;
    if (flags & 4)
        e0 = 0x1000;
    else if (flags & 0x10)
        e0 = 0x2000 - e0;
    if (e0 != 0x1000)
        ratio = KeyframeAnim_EaseCurve(ratio, e0);
    out->x = k0->x + FX_Mul_02040848(k1->x - k0->x, ratio);
    out->y = k0->y + FX_Mul_02040848(k1->y - k0->y, ratio);
}
#pragma thumb reset

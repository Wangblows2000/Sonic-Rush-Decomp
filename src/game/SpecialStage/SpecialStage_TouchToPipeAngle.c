#include <nitro/types.h>

typedef struct UnkStruct_0231293c_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_0231293c_v;

extern UnkStruct_0231293c_v data_ov001_0231eeb4;

s32 VEC_Mag(UnkStruct_0231293c_v *v);
s32 FX_Div(s32 n, s32 d);
s32 SpecialStage_PipeXToAngle(s32 v);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

s32 SpecialStage_TouchToPipeAngle(s32 x, s32 y) {
    UnkStruct_0231293c_v c = data_ov001_0231eeb4;
    s32 dy = (y << 12) - c.y;
    s32 dx = (x << 12) - c.x;
    s32 r;
    s32 len;
    s32 a;

    if (dy <= 0) {
        r = dx > 0 ? 0x1e000 : -0x1e000;
    } else {
        UnkStruct_0231293c_v v;
        v.x = dx;
        v.y = dy;
        v.z = 0;
        len = VEC_Mag(&v);
        if (len == 0)
            r = 0;
        else
            r = FX_Mul(FX_Div(0x1e000, len), dx);
        if (r > 0x1e000)
            r = 0x1e000;
        else if (r < -0x1e000)
            r = -0x1e000;
    }
    a = SpecialStage_PipeXToAngle(r);
    if (a > 0x8000)
        a -= 0x10000;
    return a;
}

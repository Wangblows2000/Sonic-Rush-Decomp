#include <nitro/types.h>

typedef struct UnkStruct_022cc06c_cam {
    u8 pad[0x3c];
    s32 unk3C;
    s32 unk40;
} UnkStruct_022cc06c_cam;

extern UnkStruct_022cc06c_cam data_ov000_0236a444;

s32 func_02044690(void);
s32 func_02044670(void);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_ov000_022cc06c(u16 *p, s32 a, s32 sh) {
    data_ov000_0236a444.unk3C = FX_Mul(a * (func_02044690() << 12), (*p << 12) >> sh);
    data_ov000_0236a444.unk40 = FX_Mul(a * (func_02044670() << 12), (*p << 12) >> sh);
    if (*p != 0)
        (*p)--;
}

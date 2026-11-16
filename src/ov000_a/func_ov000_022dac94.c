#include <nitro/types.h>

typedef struct UnkStruct_022dac94_c {
    u8 pad[0x3c];
    s32 unk3C;
    s32 unk40;
    s32 unk44;
} UnkStruct_022dac94_c;

extern UnkStruct_022dac94_c data_ov000_0236a444;

s32 func_02044690(void);
s32 func_02044670(void);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_ov000_022dac94(u16 *p, s32 a, s32 b) {
    s32 t;
    t = (*p << 12) >> b;
    data_ov000_0236a444.unk3C = FX_Mul((func_02044690() << 12) * a, t);
    t = (*p << 12) >> b;
    data_ov000_0236a444.unk40 = FX_Mul((func_02044670() << 12) * a, t);
    t = (*p << 12) >> b;
    data_ov000_0236a444.unk44 = FX_Mul((func_02044670() << 12) * a, t);
    if (*p != 0)
        (*p)--;
}

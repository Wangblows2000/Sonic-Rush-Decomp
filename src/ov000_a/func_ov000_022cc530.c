#include <nitro/types.h>

typedef struct UnkStruct_022cc530_2 {
    u8 pad[0x21c];
    u16 unk21C;
    u16 unk21E;
} UnkStruct_022cc530_2;

typedef struct UnkStruct_022cc530_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_022cc530_v;

extern UnkStruct_022cc530_2 *data_ov000_02369acc;
extern s16 data_0206e470[];

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_ov000_022cc530(UnkStruct_022cc530_v *out, s32 flag) {
    UnkStruct_022cc530_2 *g = data_ov000_02369acc;
    if (flag) {
        out->x = FX_Mul(data_0206e470[((g->unk21C & 0xffff) >> 4) * 2], 0x46f22) + 0xaa8da;
        out->z = FX_Mul(data_0206e470[((g->unk21C & 0xffff) >> 4) * 2 + 1], 0x46f22) - 0x2a1290;
        out->y = -0x1c14dc;
    } else {
        out->x = FX_Mul(data_0206e470[((g->unk21E & 0xffff) >> 4) * 2], 0x48988) + 0x117726;
        out->z = FX_Mul(data_0206e470[((g->unk21E & 0xffff) >> 4) * 2 + 1], 0x48988) - 0x2a1290;
        out->y = -0x1c14dc;
    }
}

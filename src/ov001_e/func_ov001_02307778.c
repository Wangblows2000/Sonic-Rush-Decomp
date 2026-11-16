#include <nitro/types.h>

typedef struct UnkStruct_02307778 {
    u8 pad0[0xbd8];
    s32 angles[7];
} UnkStruct_02307778;

extern UnkStruct_02307778 *data_ov001_0231f5bc;
extern s16 data_0206e470[];

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

void func_ov001_02307778(s32 i, float scale, s16 *px, s16 *py) {
    u16 a = (data_ov001_0231f5bc->angles[i] << 16) / 175 + 0xa000;
    s16 c = COS(a);
    s16 s = SIN(a);

    if (px != NULL)
        *px = 0.5f + (128.0f + scale * (c / 4096.0f));
    if (py != NULL)
        *py = 0.5f + (120.0f - scale * (s / 4096.0f));
}

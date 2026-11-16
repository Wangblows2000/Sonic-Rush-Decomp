#include <nitro/types.h>

typedef struct {
    fx32 x;
    fx32 y;
    fx32 z;
} UnkVec_020222e4;

typedef struct {
    u8 unk0[0xe0];
    UnkVec_020222e4 scale;
    UnkVec_020222e4 trans;
    u8 unkF8[4];
    u32 flag;
} UnkGlb_020222e4;

extern UnkGlb_020222e4 data_02084d38;

void func_020222e4(const UnkVec_020222e4 *scale)
{
    data_02084d38.scale = *scale;
    data_02084d38.flag &= ~0xa4;
}

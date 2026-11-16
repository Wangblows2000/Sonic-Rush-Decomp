#include <nitro/types.h>

typedef struct {
    fx32 x;
    fx32 y;
    fx32 z;
} UnkVec_020222bc;

typedef struct {
    u8 unk0[0xe0];
    UnkVec_020222bc scale;
    UnkVec_020222bc trans;
    u8 unkF8[4];
    u32 flag;
} UnkGlb_020222bc;

extern UnkGlb_020222bc data_02084d38;

void func_020222bc(const UnkVec_020222bc *trans)
{
    data_02084d38.trans = *trans;
    data_02084d38.flag &= ~0xa4;
}

#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10c];
    u8 unk10C;
    u8 pad10D[0x114 - 0x10d];
    s32 unk114;
} UnkStruct_022cc00c;

void Model_SetAnimation(UnkStruct_022cc00c *a, s32 b, s32 c, s32 d, s32 e);

void Boss1_SetPartAnimation(UnkStruct_022cc00c *a, s32 b, s32 c, s32 d, s32 e) {
    Model_SetAnimation(a, 0, b, c, 0);
    a->unk114 = d;
    a->unk10C &= ~4;
    if (e)
        a->unk10C |= 4;
}

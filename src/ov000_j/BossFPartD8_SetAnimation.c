#include <nitro/types.h>

typedef struct {
    u8 pad0[0x46c];
    s32 unk46C;
    u8 unk470[0x57c - 0x470];
    u8 unk57C;
    u8 pad57D[0x584 - 0x57d];
    s32 unk584;
} UnkStruct_022f4ce4;

void Model_SetAnimation(void *a, s32 b, s32 c, s32 d, s32 e);

void BossFPartD8_SetAnimation(UnkStruct_022f4ce4 *p, s32 a, s32 b, s32 c) {
    Model_SetAnimation(p->unk470, 0, p->unk46C, a, 0);
    p->unk584 = b;
    if (c)
        p->unk57C |= 4;
    else
        p->unk57C &= ~4;
}

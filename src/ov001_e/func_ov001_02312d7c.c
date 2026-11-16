#include <nitro/types.h>

typedef struct UnkStruct_02312d7c_b {
    u8 pad0[0xa];
    u8 unkA;
    u8 padB[0x90 - 0xb];
    u8 anm[4];
} UnkStruct_02312d7c_b;

typedef struct UnkStruct_02312d7c {
    u8 pad0[4];
    UnkStruct_02312d7c_b b;
} UnkStruct_02312d7c;

extern UnkStruct_02312d7c *gSpecialStagePlayerResources;

void NNS_G3dRenderObjSetCallBack(void *p, void (*cb)(void), s32 a, s32 b, s32 c);
void func_ov001_02311e64(void);

void func_ov001_02312d7c(void) {
    UnkStruct_02312d7c_b *b = &gSpecialStagePlayerResources->b;

    NNS_G3dRenderObjSetCallBack(b->anm, func_ov001_02311e64, 0, 6, 3);
    b->unkA = 4;
}

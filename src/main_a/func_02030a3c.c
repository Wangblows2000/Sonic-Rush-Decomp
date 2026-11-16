#include <nitro/types.h>

void NNS_G3dGeBufferOP_N(u32 a, void *b, u32 c);

typedef struct { u8 unk0[8]; u16 unk8; } UnkStruct_02030a3c;

void func_02030a3c(UnkStruct_02030a3c *s) {
    u32 v = s->unk8;
    NNS_G3dGeBufferOP_N(0x14, &v, 1);
}

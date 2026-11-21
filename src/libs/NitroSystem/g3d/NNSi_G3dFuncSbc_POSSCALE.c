#include <nitro/types.h>

void NNS_G3dGeBufferOP_N(u32 a, void *b, u32 c);

typedef struct { u32 unk0; u32 unk4; u32 flags; u8 unkC[0xD4]; u32 unkE0; u32 unkE4; } UnkStruct_02023124;

void NNSi_G3dFuncSbc_POSSCALE(UnkStruct_02023124 *s, BOOL b) {
    u32 v[4];
    if (!(s->flags & 0x100) && !(s->flags & 0x200)) {
        if (b == 0) {
            v[0] = v[1] = v[2] = s->unkE0;
        } else {
            v[0] = v[1] = v[2] = s->unkE4;
        }
        NNS_G3dGeBufferOP_N(0x1B, v, 3);
    }
    s->unk0++;
}

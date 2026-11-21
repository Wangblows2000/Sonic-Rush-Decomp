#include <nitro/types.h>

typedef struct { u8 unk0[0x20]; u32 unk20; u8 unk24; u8 unk25; } UnkStruct_02021c38;

void NNS_G3dRenderObjSetCallBack(UnkStruct_02021c38 *s, u32 a, u32 b, u8 c, u32 d) {
    s->unk20 = a;
    s->unk24 = c;
    s->unk25 = d;
}

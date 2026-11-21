#include <nitro/types.h>

typedef struct { u8 unk0[0x20]; u32 unk20; u8 unk24; u8 unk25; } UnkStruct_02021c24;

void NNS_G3dRenderObjResetCallBack(UnkStruct_02021c24 *s) {
    s->unk20 = 0;
    s->unk24 = 0;
    s->unk25 = 0;
}

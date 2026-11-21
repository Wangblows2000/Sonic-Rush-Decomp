#include <nitro/types.h>

typedef struct { u8 unk0[8]; u32 unk8; u8 unkC[4]; u16 unk10; u8 unk12[6]; u32 unk18; u8 unk1C[4]; u16 unk20; } UnkStruct_02021ae4;

void NNS_G3dTexReleaseTexKey(UnkStruct_02021ae4 *s, u32 *a, u32 *b) {
    s->unk10 &= ~1;
    s->unk20 &= ~1;
    *a = s->unk8;
    s->unk8 = 0;
    *b = s->unk18;
    s->unk18 = 0;
}

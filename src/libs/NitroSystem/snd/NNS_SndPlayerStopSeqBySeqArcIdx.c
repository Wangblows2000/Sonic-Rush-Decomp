#include <nitro/types.h>

typedef struct {
    u8 unk0[0x2c];
    u8 active;
    u8 unk2D[7];
    u16 type;
    u16 unk36;
    u16 a;
    u16 b;
    u8 unk3C[8];
} UnkSnd_0201c000;

extern UnkSnd_0201c000 data_02083464[];

void NNSi_SndPlayerStopSeq(UnkSnd_0201c000 *p, s32 frames);

void NNS_SndPlayerStopSeqBySeqArcIdx(u16 a, u16 b, s32 frames)
{
    s32 i;
    UnkSnd_0201c000 *p;
    p = data_02083464;
    for (i = 0; i < 16; i++, p++) {
        if (p->active && p->type == 2 && p->a == a && p->b == b)
            NNSi_SndPlayerStopSeq(p, frames);
    }
}

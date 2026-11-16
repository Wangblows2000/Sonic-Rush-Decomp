#include <nitro/types.h>

typedef struct UnkStruct_022dde00 {
    u8 pad0[4];
    u32 unk4;
    s32 unk8;
    u8 padC[0x10 - 0xc];
    s32 unk10;
    u8 pad14[0xbd4 - 0x14];
    s32 unkBD4;
} UnkStruct_022dde00;

typedef struct UnkStruct_022dde00_b {
    u8 pad0[4];
    u8 unk4[1];
} UnkStruct_022dde00_b;

void Sound_PlaySeqArc(void *p, u32 a, u32 b);
void NNS_SndPlayerStopSeq(void *p, s32 a);
void Sound_PlayVoice(s32 a, s32 b, s32 c);

void func_ov001_022dde00(UnkStruct_022dde00 *w, UnkStruct_022dde00_b *b) {
    if (w->unk8 == 0x1e) {
        if (w->unk10 != 2)
            Sound_PlaySeqArc(b->unk4, 0xb, 0);
        else
            Sound_PlaySeqArc(b->unk4, 0xb, 1);
    }
    if ((w->unk4 & 0x400) && !(w->unk4 & 0x1000)) {
        NNS_SndPlayerStopSeq(b->unk4, 0x32);
        w->unk4 |= 0x1000;
    }
    if (w->unk10 != 2) {
        if (w->unkBD4 == 0) {
            if (w->unk8 == 0x78)
                Sound_PlayVoice(0, 0xb, 2);
        } else {
            if (w->unk8 == 0xa0)
                Sound_PlayVoice(0, 0xb, 3);
        }
        if (w->unk8 == 0xdc && !(w->unk4 & 0x1000)) {
            NNS_SndPlayerStopSeq(b->unk4, 0x32);
            w->unk4 |= 0x1000;
        }
    } else {
        if (w->unk8 == 0xb4)
            Sound_PlayVoice(0, 0xb, 2);
        if (w->unk8 == 0x12c)
            Sound_PlayVoice(0, 0xb, 3);
        if (w->unk8 == 0x195 && !(w->unk4 & 0x1000)) {
            NNS_SndPlayerStopSeq(b->unk4, 0x32);
            w->unk4 |= 0x1000;
        }
    }
}

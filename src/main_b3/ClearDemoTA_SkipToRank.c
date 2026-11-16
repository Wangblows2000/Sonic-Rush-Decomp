#include <nitro/types.h>

typedef struct { s32 unk0; u8 pad4[0x18]; u8 unk1C; u8 unk1D; } UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;

typedef struct {
    void (*fn)(void);
    s32 unk4;
    u8 pad8[0x94];
    s16 unk9C;
    u8 pad9E[2];
    s32 unkA0;
    s32 unkA4;
    s32 unkA8;
    u8 padAC[0xe];
    u8 unkBA;
    u8 unkBB;
} UnkObj_020453f8;
typedef struct {
    u8 pad0[0x274];
    UnkObj_020453f8 obj;   /* 0x274 */
    u8 pad330[0x14];
    u32 flags;             /* 0x344 */
    u8 pad348[0x12e8];
    void *unk1630;         /* 0x1630 */
    u8 unk1634[4];         /* 0x1634 */
} UnkWork_020453f8;

void NNS_SndPlayerStopSeq(void *p, s32 a);
void ClearDemo_Unk_02045594(void);
void Sound_PlaySeq(void *a, s32 b);
void Sound_PlaySeqArc(void *a, void *b, s32 c);
void Sound_PlayVoice(void *a, void *b, s32 c);

#pragma thumb on
void ClearDemoTA_SkipToRank(UnkWork_020453f8 *w) {
    UnkObj_020453f8 *o = &w->obj;

    w->flags |= 0x10000;
    o->unkA4 = 0x50000;
    o->unkA8 = 0x7c000;
    o->unkA0 = 0x1000;
    NNS_SndPlayerStopSeq(w->unk1634, 0);
    o->unk4 = 0;
    w->obj.fn = ClearDemo_Unk_02045594;
    w->flags |= 0x8000;
    if (o->unkBA == 0) {
        if (o->unk9C == 0) {
            if (gGameState.unk1C >= 9 || gGameState.unk1D >= 2)
                Sound_PlaySeq(NULL, 0xe);
            else
                Sound_PlaySeq(NULL, 0xc);
        } else {
            Sound_PlaySeqArc(NULL, w->unk1630, 1);
        }
    }
    if (o->unkBB == 0) {
        if (gGameState.unk0 == 0) {
            if (gGameState.unk1C >= 9 || gGameState.unk1D >= 2)
                Sound_PlayVoice(NULL, w->unk1630, 4);
            else
                Sound_PlayVoice(NULL, w->unk1630, 3);
        } else {
            if (gGameState.unk1C >= 9 || gGameState.unk1D >= 2)
                Sound_PlayVoice(NULL, w->unk1630, 6);
            else
                Sound_PlayVoice(NULL, w->unk1630, 5);
        }
    }
}
#pragma thumb reset

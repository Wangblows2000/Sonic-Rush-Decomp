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
} UnkObj_02045768;
typedef struct {
    u8 pad0[0x274];
    UnkObj_02045768 obj;   /* 0x274 */
    u8 pad330[0x14];
    u32 flags;             /* 0x344 */
    u8 pad348[0x12e8];
    void *unk1630;         /* 0x1630 */
    u8 unk1634[4];         /* 0x1634 */
} UnkWork_02045768;

void NNS_SndPlayerStopSeq(void *p, s32 a);
void ClearDemoTA_State_StampRank(void);
void Sound_PlaySeq(void *a, s32 b);
void Sound_PlaySeqArc(void *a, void *b, s32 c);
void Sound_PlayVoice(void *a, void *b, s32 c);

#pragma thumb on
void ClearDemoTA_State_WaitRank(UnkWork_02045768 *w, UnkObj_02045768 *o) {
    o->unk4--;
    if (o->unk4 <= 0) {
        o->fn = ClearDemoTA_State_StampRank;
        o->unk4 = 0;
        w->flags |= 0x10000;
        NNS_SndPlayerStopSeq(w->unk1634, 0);
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
        o->unkBB = 1;
    }
}
#pragma thumb reset

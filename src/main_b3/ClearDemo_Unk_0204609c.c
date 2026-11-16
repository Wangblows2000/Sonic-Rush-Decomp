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
} UnkObj_0204609c;
typedef struct {
    u8 pad0[0x274];
    UnkObj_0204609c obj;   /* 0x274 */
    u8 pad330[0x14];
    u32 flags;             /* 0x344 */
    u8 pad348[0x12e8];
    void *unk1630;         /* 0x1630 */
    u8 unk1634[4];         /* 0x1634 */
} UnkWork_0204609c;

void NNS_SndPlayerStopSeq(void *p, s32 a);
void ClearDemo_Unk_0204622c(void);
void Sound_PlaySeq(void *a, s32 b);
void Sound_PlaySeqArc(void *a, void *b, s32 c);
void Sound_PlayVoice(void *a, void *b, s32 c);

#pragma thumb on
void ClearDemo_Unk_0204609c(UnkWork_0204609c *w) {
    UnkObj_0204609c *o = &w->obj;

    w->flags |= 0x10000;
    o->unkA4 = 0x50000;
    o->unkA8 = 0x8a000;
    o->unkA0 = 0x1000;
    o->unk4 = 0;
    o->fn = ClearDemo_Unk_0204622c;
    if (o->unkBA == 0)
        Sound_PlaySeqArc(NULL, w->unk1630, 3);
    if (o->unkBB == 0) {
        if (gGameState.unk0 == 0) {
            if (gGameState.unk1C == 7)
                Sound_PlayVoice(NULL, w->unk1630, 4);
            else if (gGameState.unk1C >= 9 || gGameState.unk1D >= 2)
                Sound_PlayVoice(NULL, w->unk1630, 5);
            else
                Sound_PlayVoice(NULL, w->unk1630, 4);
        } else {
            if (gGameState.unk1C == 7)
                Sound_PlayVoice(NULL, w->unk1630, 5);
            else if (gGameState.unk1C >= 9 || gGameState.unk1D >= 2)
                Sound_PlayVoice(NULL, w->unk1630, 7);
            else
                Sound_PlayVoice(NULL, w->unk1630, 6);
        }
    }
}
#pragma thumb reset

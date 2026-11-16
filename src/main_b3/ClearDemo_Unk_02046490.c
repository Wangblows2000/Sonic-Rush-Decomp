#include <nitro/types.h>

typedef struct {
    void (*fn)(void);
    s32 unk4;
    u8 pad8[0x4c];
    s16 digits[6][6]; /* 0x54 */
} UnkObj_02046490;
typedef struct {
    u8 pad0[0x344];
    u32 flags;             /* 0x344 */
    u8 pad348[0x12e8];
    void *unk1630;         /* 0x1630 */
    u8 unk1634[4];         /* 0x1634 */
} UnkWork_02046490;

extern u32 gRandSeed;
void ClearDemo_Unk_020463bc(void);
void NNS_SndPlayerStopSeq(void *p, s32 a);
void Sound_PlaySeqArc(void *a, void *b, s32 c);

#pragma thumb on
static inline u16 Rand_02046490(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void ClearDemo_Unk_02046490(UnkWork_02046490 *w, UnkObj_02046490 *o) {
    s32 n;
    s32 k;
    s32 i;

    o->unk4++;
    n = o->unk4 / 16 + 1;
    if (n > 6) {
        n = 6;
        o->fn = ClearDemo_Unk_020463bc;
        o->unk4 = 0;
        w->flags |= 0x400000;
        NNS_SndPlayerStopSeq(w->unk1634, 0);
        Sound_PlaySeqArc(NULL, w->unk1630, 2);
    }
    k = 6 - n;
    for (i = 5; i >= 0 && n > 0; i--, n--)
        o->digits[4][i] = o->digits[5][i];
    for (i = 0; i < k; i++)
        o->digits[4][i] = Rand_02046490() % 10;
}
#pragma thumb reset

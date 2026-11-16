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
    u8 padA4[0x16];
    u8 unkBA;
} UnkObj_0204633c;
typedef struct {
    u8 pad0[0x1630];
    void *unk1630;
} UnkWork_0204633c;

void ClearDemo_Unk_0204625c(void);
void Sound_PlaySeq(void *a, s32 b);
void Sound_PlaySeqArc(void *a, void *b, s32 c);

#pragma thumb on
static inline s32 FX_Mul_0204633c(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void ClearDemo_Unk_0204633c(void *w, UnkObj_0204633c *o) {
    s32 t;
    s32 v;
    s32 n;

    o->unk4++;
    t = (s16)(o->unk4 << 8);
    if (t > 0x1000)
        t = 0x1000;
    n = 5;
    v = 0x1000;
    do {
        v = FX_Mul_0204633c(v - 0x2000, t) + 0x2000;
    } while (n--);
    o->unkA0 = v;
    if (o->unk4 >= 0x10) {
        o->unkA0 = 0x1000;
        o->fn = ClearDemo_Unk_0204625c;
        o->unk4 = 0;
    }
}
#pragma thumb reset

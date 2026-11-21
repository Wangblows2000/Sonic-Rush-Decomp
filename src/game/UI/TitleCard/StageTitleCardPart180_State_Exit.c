#include <nitro/types.h>

typedef struct {
    void (*fn)(void);
    u8 pad4[0x48];
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    u8 pad5C[0xc];
    s32 unk68;
    u16 delay;  /* 0x6c */
    s16 t;      /* 0x6e */
} UnkObj_0205c048;
typedef struct { u8 pad0[0x1c]; u8 unk1C; } UnkStruct_022c4560;
typedef struct { s32 a; s32 b; } UnkEnt_0205c048;
extern UnkStruct_022c4560 gGameState;
extern UnkEnt_0205c048 data_02076bb8[];

void StageTitleCardPart180_State_Done(void);

static inline s32 FX_Mul_0205c048(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void StageTitleCardPart180_State_Exit(void *w, UnkObj_0205c048 *o) {
    o->t += 0x28;
    if (o->t >= 0x1000) {
        o->t = 0x1000;
        o->fn = StageTitleCardPart180_State_Done;
    }
    o->unk4C = o->unk54 + FX_Mul_0205c048(0x1c2000 - o->unk54, o->t);
    o->unk50 = o->unk58 + FX_Mul_0205c048(o->unk68 + data_02076bb8[gGameState.unk1C].a - o->unk58, o->t);
}

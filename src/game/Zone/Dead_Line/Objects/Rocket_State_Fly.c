#include <nitro/types.h>

typedef struct UnkStruct_02313cfc {
    u8 unk0;
    u8 unk1;
    u8 pad2[0x34 - 2];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x1DC - 0x5E];
    void *unk1DC;
    u8 pad1E0[0x204 - 0x1E0];
    struct UnkStruct_02313cfc *unk204;
    u8 pad208[0x3E8 - 0x208];
    struct UnkStruct_02313cfc *unk3E8;
} UnkStruct_02313cfc;

typedef struct {
    u8 pad0[0xDA];
    s16 unkDA;
    u8 padDC[0x122 - 0xDC];
    s16 unk122;
} UnkStruct_02313cfc_Eff;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_02313cfc_4560;

extern UnkStruct_02313cfc_4560 gGameState;
extern void *gPlayers[];
extern u32 gRandSeed;

void Sound_SetPositional(void *a, UnkStruct_02313cfc *w, void *b);
void Math_RotateVec2S16(s16 *x, s16 *y, u8 a);
UnkStruct_02313cfc_Eff *EffZakoBombPuff_Create(UnkStruct_02313cfc *w, s16 x, s16 y);
void Rocket_Explode(UnkStruct_02313cfc *w);
s16 ObjSpd_UpSet(s16 v, s32 a, s32 b);

static inline u16 UnkRand_02313cfc(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void Rocket_State_Fly(UnkStruct_02313cfc *w) {
    if (w->unk204 != NULL) {
        if (w->unk204->unk3E8 == w) {
            w->unk1 = w->unk204->unk1;
        } else {
            w->unk204 = NULL;
        }
    } else {
        Sound_SetPositional(w->unk1DC, w, gPlayers[0]);
    }
    if (!(gGameState.unk20 & 3)) {
        s16 x;
        s16 y;
        UnkStruct_02313cfc_Eff *e;
        x = 0x33 - (UnkRand_02313cfc() & 6);
        y = 3 - (UnkRand_02313cfc() & 6);
        Math_RotateVec2S16(&x, &y, w->unk1);
        e = EffZakoBombPuff_Create(w, x, y);
        e->unkDA = 0x15;
        e->unk122 = 0x15;
    }
    if ((w->unk58 & 0xf) || w->unk34 == 0) {
        Rocket_Explode(w);
    }
    if (w->unk5C & 1) {
        w->unk34 = ObjSpd_UpSet(w->unk34, 0x10, 0xc00);
    } else {
        w->unk34 = ObjSpd_UpSet(w->unk34, -0x10, 0xc00);
    }
}

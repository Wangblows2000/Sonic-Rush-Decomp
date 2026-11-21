#include <nitro/types.h>

typedef struct UnkStruct_022dccc8_a {
    u8 pad[0x10c];
    u8 unk10C;
    u8 unk10D;
    u8 pad1[0x138 - 0x10e];
} UnkStruct_022dccc8_a;

typedef struct UnkStruct_022dccc8 {
    u8 pad[0x5d0];
    UnkStruct_022dccc8_a unk5D0;
    u8 pad1[0xab0 - 0x708];
    void *unkAB0;
    void *unkAB4;
    u8 pad2[0xbd0 - 0xab8];
    s32 unkBD0;
    s32 unkBD4;
    s32 unkBD8;
} UnkStruct_022dccc8;

typedef struct UnkStruct_022dccc8_7 {
    u8 pad[8];
    u16 unk8;
} UnkStruct_022dccc8_7;

extern UnkStruct_022dccc8_7 gBossStatus;
extern s16 data_ov000_02363570[];

void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);
void Boss4_LoadModelPalettes2Warn(s32 a);

void Boss4_ActivateAuxModel(UnkStruct_022dccc8 *s) {
    if (s->unkBD0 == 0) {
        UnkStruct_022dccc8_a *a = &s->unk5D0;
        Sound_PlaySeqArc(0, 0, 0x88);
        Model_SetAnimation(a, 0, s->unkAB0, 0, 0);
        Model_SetAnimation(a, 1, s->unkAB4, 0, 0);
        a->unk10C |= 4;
        a->unk10D |= 4;
    }
    s->unkBD0 = 1;
    s->unkBD4 = data_ov000_02363570[gBossStatus.unk8 - 1];
    s->unkBD8 = 0;
    Boss4_LoadModelPalettes2Warn(0);
}

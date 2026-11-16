#include <nitro/types.h>

typedef struct UnkStruct_022dcc10 {
    u8 pad[0xbd0];
    s32 unkBD0;
    s32 unkBD4;
} UnkStruct_022dcc10;

typedef struct UnkStruct_022dcc10_7 {
    u8 pad[8];
    u16 unk8;
} UnkStruct_022dcc10_7;

typedef struct UnkStruct_022dcc10_4 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022dcc10_4;

extern UnkStruct_022dcc10_7 gBossStatus;
extern UnkStruct_022dcc10_4 gGameState;
extern u32 gRandSeed;
extern s16 data_ov000_023636a8[][8][2];

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

static inline s32 GetIdx(void) {
    BOOL b = TRUE;
    if (gGameState.unk10 != 2 && gGameState.unk10 != 3)
        b = FALSE;
    if (b)
        return 1;
    return gGameState.unk14;
}

void Boss4_DeactivateAuxModel(UnkStruct_022dcc10 *s) {
    u16 r;
    s->unkBD0 = 0;
    r = Rand();
    s->unkBD4 = data_ov000_023636a8[GetIdx()][gBossStatus.unk8 - 1][r & 1];
    s->unkBD4 -= 0x3c;
}

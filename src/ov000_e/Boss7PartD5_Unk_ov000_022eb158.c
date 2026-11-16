#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u8 pad[0x10 - 4];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    u8 pad[8];
    u16 unk8;
} UnkStruct_02369ac0;

typedef struct {
    u8 pad[0x220];
    u32 unk220;
} UnkStruct_02369b78;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    u8 pad14[0x2E - 0x14];
    s16 unk2E;
    u8 pad30[0x328 - 0x30];
    s16 unk328;
} UnkStruct_022eb158;

extern UnkStruct_022c4560 gGameState;
extern UnkStruct_02369ac0 gBossStatus;
extern UnkStruct_02369b78 *gBoss7;
extern u16 data_ov000_02363be4[][8];
extern UnkStruct_022eb158 *gPlayers;

static inline s32 GetIdxB(void) {
    BOOL b = TRUE;
    if (gGameState.unk10 != 2 && gGameState.unk10 != 3)
        b = FALSE;
    if (b)
        return 1;
    return gGameState.unk14;
}

BOOL Boss7PartD5_Unk_ov000_022eb158(UnkStruct_022eb158 *s) {
    UnkStruct_022eb158 *p;
    s32 d;
    UnkStruct_02369b78 *g = gBoss7;

    if (g->unk220 & data_ov000_02363be4[GetIdxB()][gBossStatus.unk8 - 1]) {
        return FALSE;
    }
    p = gPlayers;
    if (p->unk328 != 0x30) {
        return FALSE;
    }
    d = s->unk10 - p->unk10;
    if (d < 0) {
        d = -d;
    }
    if (d < 0x6000) {
        if ((s->unk10 < p->unk10 && p->unk2E < 0) || (s->unk10 > p->unk10 && p->unk2E > 0)) {
            return TRUE;
        }
    }
    return d < 0x3000;
}

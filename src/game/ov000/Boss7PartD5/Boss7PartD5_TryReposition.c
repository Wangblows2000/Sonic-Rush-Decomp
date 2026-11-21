#include <ov000_c/stageobj.h>

typedef struct {
    u32 unk0;
    s32 unk4;
    u16 unk8;
    u16 unkA;
} UnkStruct_02369ac0;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x43C - 0x20C];
    void *unk43C;
    u8 pad440[0x44E - 0x440];
    u16 unk44E;
    u8 pad450[0x456 - 0x450];
    s16 unk456;
} UnkStruct_022eabfc;

extern UnkStruct_022c4560 gGameState;
extern UnkStruct_02369ac0 gBossStatus;
extern u16 data_ov000_02363ba4[][8];
extern UnkStruct_StageObj *gPlayers;

void Boss7PartD5_InputState_Walk(void);

static inline BOOL IsMode23(void) {
    return gGameState.unk10 == 2 || gGameState.unk10 == 3;
}

static inline s32 GetLevel(void) {
    return IsMode23() ? 1 : gGameState.unk14;
}

BOOL Boss7PartD5_TryReposition(UnkStruct_022eabfc *w) {
    if (w->unk44E > data_ov000_02363ba4[GetLevel()][gBossStatus.unk8 - 1]) {
        s32 x = w->base.unk10;
        s32 rel = x < 0xa000 ? x : x - 0x14000;
        s32 a = rel < 0 ? -rel : rel;
        s32 d;

        if (a < 0x4000) {
            if (rel > 0)
                w->unk456 = (Rand() & 0x3f) + 0x1e;
            else
                w->unk456 = -((Rand() & 0x3f) + 0x1e);
            w->unk43C = Boss7PartD5_InputState_Walk;
            w->unk44E = 0;
            return TRUE;
        }
        d = x - gPlayers->unk10;
        if (d < 0)
            d = -d;
        if (d < 0x2000) {
            if (x < 0xa000)
                w->unk456 = (Rand() & 0x3f) + 0x2d;
            else
                w->unk456 = -((Rand() & 0x3f) + 0x2d);
            w->unk43C = Boss7PartD5_InputState_Walk;
            w->unk44E = 0;
            return TRUE;
        }
    }
    if (w->base.unk10 < gPlayers->unk10 && (w->base.unk5C & 1)) {
        w->unk456 = 10;
        w->unk43C = Boss7PartD5_InputState_Walk;
        return TRUE;
    }
    if (w->base.unk10 > gPlayers->unk10 && !(w->base.unk5C & 1)) {
        w->unk456 = -10;
        w->unk43C = Boss7PartD5_InputState_Walk;
        return TRUE;
    }
    return FALSE;
}

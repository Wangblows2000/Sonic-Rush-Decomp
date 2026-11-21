#include <ov000_c/stageobj.h>

typedef struct {
    u32 unk0;
    u8 pad[0x10 - 4];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x434 - 0x20C];
    u16 unk434;
} UnkStruct_022f32d0;

extern UnkStruct_022c4560 gGameState;

void BossFPartD8_TrackPlayerX_Far(UnkStruct_022f32d0 *s);
void BossFPartD8_State_DoublePressStrike(void);

static inline s32 GetIdxB(void) {
    BOOL b = TRUE;
    if (gGameState.unk10 != 2 && gGameState.unk10 != 3)
        b = FALSE;
    if (b)
        return 1;
    return gGameState.unk14;
}

static inline s32 GetIdxS(void) {
    switch (gGameState.unk10) {
    case 2:
    case 3:
        return 1;
    }
    return gGameState.unk14;
}

void BossFPartD8_State_DoublePressTrack(UnkStruct_022f32d0 *s) {
    if (GetIdxB() == 1) {
        BossFPartD8_TrackPlayerX_Far(s);
    } else if (GetIdxS() == 0) {
        if (s->unk434 <= 0x3c)
            s->base.unk2E = 0;
        else
            BossFPartD8_TrackPlayerX_Far(s);
    }
    if (s->unk434-- == 0)
        s->base.unk68 = BossFPartD8_State_DoublePressStrike;
}

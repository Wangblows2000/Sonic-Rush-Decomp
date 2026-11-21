#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[0x20];
    u16 unk20;
} UnkStruct_0208922c;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x328 - 0x20C];
    s16 unk328;
} UnkStruct_Player;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x434 - 0x20C];
    s32 unk434;
    u8 pad438[0x43C - 0x438];
    s32 unk43C;
    u16 unk440;
    u16 unk442;
    u8 pad444[0x458 - 0x444];
    s32 unk458;
} UnkStruct_022f1374;

extern UnkStruct_022c4560 gGameState;
extern UnkStruct_0208922c gDisplayMain;
extern UnkStruct_Player *gPlayers;
extern u8 data_ov000_02365c5c[][3];

void BossFPartD8_TrackPlayerX_NearSlow(UnkStruct_022f1374 *w);
void BossFPartD8_State_PartDCVolleyFire(void);
void BossFPartD8_State_PartDCVolleyEnd(void);

static inline BOOL IsMode23(void) {
    return gGameState.unk10 == 2 || gGameState.unk10 == 3;
}

static inline s32 GetLevel(void) {
    return IsMode23() ? 1 : gGameState.unk14;
}

void BossFPartD8_State_PartDCVolleyInterval(UnkStruct_022f1374 *w) {
    BossFPartD8_TrackPlayerX_NearSlow(w);
    if ((u16)(s16)(gPlayers->unk328 - 0xf) <= 1)
        w->unk434 = 1;
    if (w->unk442-- != 0)
        return;
    gDisplayMain.unk20 &= ~0xc0;
    w->unk440++;
    w->unk43C = Rand() & 1;
    if (w->unk440 < data_ov000_02365c5c[GetLevel()][w->unk458])
        w->base.unk68 = BossFPartD8_State_PartDCVolleyFire;
    else
        w->base.unk68 = BossFPartD8_State_PartDCVolleyEnd;
}

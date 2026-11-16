#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x48];
    s16 unk48;
} UnkStruct_0208922c;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
} UnkStruct_022c4590;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x3AE - 0x20C];
    s16 unk3AE;
} UnkStruct_Player;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x434 - 0x20C];
    s16 unk434;
    u8 pad436[0x57C - 0x436];
    u8 unk57C;
} UnkStruct_022ef2a4;

extern UnkStruct_0208922c gDisplayMain;
extern UnkStruct_022c4590 data_022c4590;
extern UnkStruct_Player *gPlayers;

void Stage_OnBossCleared(void);
void StageShutdown_Create(void (*fn)(void));

void BossFPartD8_State_Unk_ov000_022ef2a4(UnkStruct_022ef2a4 *w) {
    w->unk434++;
    if (w->unk434 >= 0 && (w->unk434 & 3) == 0) {
        if (gDisplayMain.unk48 > 0)
            gDisplayMain.unk48--;
    }
    if (!(w->unk57C & 2))
        return;
    if (w->unk434 <= 0x12c)
        return;
    w->base.unk54 |= 4;
    data_022c4590.unk10 = gPlayers->unk3AE * 1000;
    StageShutdown_Create(Stage_OnBossCleared);
}

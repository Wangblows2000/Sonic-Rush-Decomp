#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u8 pad4[0x10 - 4];
    s32 unk10;
    u8 pad14[0x20 - 0x14];
    u32 unk20;
    u8 pad24[0x28 - 0x24];
    u16 unk28;
} UnkCfg_0234cfc8;

typedef struct {
    u8 pad0[0x14];
    u8 unk14;
    u8 pad15[0x24 - 0x15];
    u32 unk24;
} UnkCfg2_0234cfc8;

typedef struct {
    u8 pad0[8];
    u32 unk8;
} UnkCfg3_0234cfc8;

extern UnkCfg_0234cfc8 gGameState;
extern UnkCfg2_0234cfc8 data_022c4590;
extern UnkCfg3_0234cfc8 gSaveData;

void ResultsScreen_Create(void);
void SceneChange_ToTryAgain(void);
void SaveData_SetLives(u8 a, u8 b, s32 c);
u16 SaveData_GetSonicEmeralds(void);
void SaveData_CommitOrShowError(s32 a);
void GameOverSeq_StartTimeOver(void);
void GameOverSeq_StartGameOver(void);

void Stage_ExitAfterFail(void) {
    switch (gGameState.unk10) {
    case 1:
        data_022c4590.unk24 &= ~1;
        data_022c4590.unk24 &= ~2;
        ResultsScreen_Create();
        return;
    case 2:
        SceneChange_ToTryAgain();
        return;
    case 0:
        SaveData_SetLives(gGameState.unk0, data_022c4590.unk14, 1);
        gGameState.unk28 = SaveData_GetSonicEmeralds();
        SaveData_CommitOrShowError(0);
        if (!(gSaveData.unk8 & 2) && gGameState.unk20 > 0x8c9f) {
            GameOverSeq_StartTimeOver();
            return;
        }
        GameOverSeq_StartGameOver();
        return;
    default:
        GameOverSeq_StartGameOver();
        return;
    }
}

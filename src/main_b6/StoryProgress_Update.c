#include <nitro/types.h>

typedef struct {
    s32 unk0;
    u8 pad4[0xa];
    u16 unke;
    u8 pad10[0xc];
    u8 zone;
    u8 act;
} UnkState_0204e4dc;

typedef struct {
    u8 pad0[0xc];
    u16 unkc;
    u8 pade[0x56];
    u16 unk64;
} UnkProg_0204e4dc;

extern UnkState_0204e4dc gGameState;
extern UnkProg_0204e4dc gSaveData;
extern u16 data_022c45f8;
extern u8 data_02074bbc[];
extern u8 data_02074bb4[];

u32 StoryProgress_GetZoneIndex(u8 zone);
u8 StoryProgress_GetActIndex(u8 act);
BOOL StoryProgress_IsStageNew(void);
void SaveData_SetSonicProgress(u8 v);
void SaveData_SetBlazeProgress(u8 v);
u16 StoryProgress_CalcMapStep(u16 v);

void StoryProgress_Update(void) {
    u32 z = StoryProgress_GetZoneIndex(gGameState.zone);
    u8 a = StoryProgress_GetActIndex(gGameState.act);

    if (z >= 7)
        return;
    if (a == 0) {
        if (StoryProgress_IsStageNew())
            data_022c45f8 |= 1;
        return;
    }
    if (StoryProgress_IsStageNew()) {
        data_022c45f8 |= 1;
        if (gGameState.unk0 != 1)
            SaveData_SetSonicProgress(a != 2 ? data_02074bbc[z] - 1 : data_02074bbc[z]);
        else
            SaveData_SetBlazeProgress(a != 2 ? data_02074bb4[z] - 1 : data_02074bb4[z]);
    }
    if (gGameState.unk0 != 1)
        gGameState.unke = StoryProgress_CalcMapStep(gSaveData.unkc);
    else
        gGameState.unke = StoryProgress_CalcMapStep(gSaveData.unk64);
}

#include <nitro/types.h>

typedef struct {
    u8 pad[0x10];
    s32 mode;
    u8 pad14[8];
    u8 kind;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 gGameState;
void ClearDemo_Create(void);
void DemoScript_StartAfterBoss(void);
void Stage_FreeZoneFiles(void);

void Stage_OnBossCleared(void) {
    if (gGameState.mode == 2) {
        ClearDemo_Create();
        return;
    }
    switch (gGameState.kind) {
    case 7:
        ClearDemo_Create();
        return;
    case 8:
        DemoScript_StartAfterBoss();
        return;
    default:
        Stage_FreeZoneFiles();
        DemoScript_StartAfterBoss();
        return;
    }
}

#include <nitro/types.h>

typedef struct { u8 unk0[0x1c]; u8 unk1C; u8 unk1D; } UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;
extern void (*gStageZoneSetupFuncTable[][3])(void);
extern void *gZoneSoundData;
void Stage_Unk_ov000_0233bc40(void);
void Stage_LoadEventResources(void);
void Sound_LoadSoundArchiveFromMemory(void *p);
void Stage_PlayBGM(s32 a);
void Stage_SetupZoneResources(void) { Stage_Unk_ov000_0233bc40(); Stage_LoadEventResources(); Sound_LoadSoundArchiveFromMemory(gZoneSoundData); Stage_PlayBGM(0); gStageZoneSetupFuncTable[gGameState.unk1C][gGameState.unk1D](); }

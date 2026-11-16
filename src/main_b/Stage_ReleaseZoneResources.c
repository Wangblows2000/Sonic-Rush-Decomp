#include <nitro/types.h>

typedef struct { u8 unk0[0x1c]; u8 unk1C; u8 unk1D; } UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;
extern void (*gStageZoneReleaseFuncTable[][3])(void);
extern u32 data_ov000_02369ba0;
void NNS_SndStopSoundAll(void);
void NNS_SndStopChannelAll(void);
void Stage_Unk_ov000_0233bc3c(void);
void Stage_FreeEventResources(void);
void Stage_ReleaseZoneResources(void) { NNS_SndStopSoundAll(); NNS_SndStopChannelAll(); gStageZoneReleaseFuncTable[gGameState.unk1C][gGameState.unk1D](); Stage_Unk_ov000_0233bc3c(); Stage_FreeEventResources(); data_ov000_02369ba0 = 0; }

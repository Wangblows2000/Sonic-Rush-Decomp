#include <nitro/types.h>

typedef struct { s32 unk0; u8 unk4[0x18]; u8 unk1C; u8 unk1D; } UnkStruct_022c4560;
typedef struct { u32 unk0; u32 unk4; u8 unk8[0x40]; s32 unk48; u8 unk4C; u8 unk4D; } UnkStruct_022c4490;
extern UnkStruct_022c4560 gGameState;
extern UnkStruct_022c4490 gStageLoader;
void MIi_CpuClear16(u16 data, void *dst, u32 size);
void StageLoader_Init(u32 v) { vu16 z = 0; MIi_CpuClear16(z, &gStageLoader, 0x60); gStageLoader.unk4 = v; gStageLoader.unk48 = gGameState.unk0; gStageLoader.unk4C = gGameState.unk1C; gStageLoader.unk4D = gGameState.unk1D; }

#include <nitro/types.h>

typedef struct { u8 unk0[0x24]; u32 unk24; u8 unk28[0x18]; u32 unk40; } UnkStruct_022c4590;
typedef struct { u8 unk0[0x18]; u32 unk18; u8 unk1C[4]; u32 unk20; u32 unk24; } UnkStruct_022c4560;
extern UnkStruct_022c4590 data_022c4590;
extern UnkStruct_022c4560 gGameState;
void Camera_Reset(void);
void StageEvent_Reset(void);
void Stage_ResetProgress(void) { data_022c4590.unk24 = 0; data_022c4590.unk40 = 0; gGameState.unk20 = 0; gGameState.unk24 = 0; gGameState.unk18 = 0; Camera_Reset(); StageEvent_Reset(); }

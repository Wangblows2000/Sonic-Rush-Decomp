#include <nitro/types.h>

typedef struct { u8 unk0[0xe]; u16 unkE; } UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;
extern u16 gNaviMNewEmeraldMask;
extern u16 gNaviMCounterPending;
extern u16 gNaviMSavedStep;
void NaviM_ResetPending(void) { gNaviMNewEmeraldMask = 0; gNaviMCounterPending = 0; gNaviMSavedStep = gGameState.unkE; }

#include <nitro/types.h>

typedef struct { u8 unk0[0xe]; u16 unkE; } UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;
extern u16 gNaviMSavedStep;
extern u16 gNaviMCounterPending;
u16 NaviM_CheckStepAdvanced(void) { u16 n = gGameState.unkE; if (gNaviMSavedStep < n && n >= 1) return gNaviMCounterPending = 1; return gNaviMCounterPending = 0; }

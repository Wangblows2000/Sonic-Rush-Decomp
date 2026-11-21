#include <nitro/types.h>

typedef struct { u8 unk0[0x18]; u32 unk18; } UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;
void StageRestart_State_Release(void);
void *Task_Create(void (*fn)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void StageRestart_Create(void) { gGameState.unk18 |= 2; Task_Create(StageRestart_State_Release, 0, 0, 0, 0, 0); }

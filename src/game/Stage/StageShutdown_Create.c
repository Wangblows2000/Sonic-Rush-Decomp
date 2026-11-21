#include <nitro/types.h>

typedef struct { u32 unk0; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
void StageShutdown_State_Release(void);
UnkTask *Task_Create(void (*fn)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void StageShutdown_Create(u32 v) { Task_Create(StageShutdown_State_Release, 0, 0, 0, 0, 8)->unk10->unk0 = v; }

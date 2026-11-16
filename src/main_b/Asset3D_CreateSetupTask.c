#include <nitro/types.h>

typedef struct { u32 unk0; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
void Asset3D_SetupTask_Main(void);
UnkTask *Task_Create(void (*fn)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void Asset3D_CreateSetupTask(u32 v) { Task_Create(Asset3D_SetupTask_Main, 0, 0, 0xf001, 0xfffe, 4)->unk10->unk0 = v; }

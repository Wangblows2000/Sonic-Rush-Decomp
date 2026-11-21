#include <nitro/types.h>

typedef struct { u32 unk0; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
void DisableSwapBuffersTask_Main(void);
UnkTask *Task_Create(void (*fn)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void DisableSwapBuffersTask_Create(u32 v) { Task_Create(DisableSwapBuffersTask_Main, 0, 0x30, 0xffff, 0xfffe, 4)->unk10->unk0 = v; }

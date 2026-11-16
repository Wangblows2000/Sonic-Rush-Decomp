#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
void func_0204bc90(void);
UnkTask *Task_Create(void (*fn)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void func_0204d684(u32 a, u32 b) { UnkWork *w = Task_Create(func_0204bc90, 0, 0, 0, 0, 8)->unk10; w->unk4 = a; w->unk0 = b; }

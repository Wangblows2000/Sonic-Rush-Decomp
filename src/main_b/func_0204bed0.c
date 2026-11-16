#include <nitro/types.h>

typedef struct { u32 flags; } UnkWork;
typedef struct { u32 unk0; u32 unk4; void (*fn)(void); u32 unkC; UnkWork *unk10; } UnkTask;
extern UnkTask *gTaskCurrent;
void StageLoader_Init(s32 a);
void func_0204be9c(void);
void func_0204be48(void);
void func_0204bed0(void) { UnkWork *w = gTaskCurrent->unk10; StageLoader_Init(0); if (w->flags & 1) gTaskCurrent->fn = func_0204be9c; else gTaskCurrent->fn = func_0204be48; }

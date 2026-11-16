#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; void (*fn)(void); } UnkTask;
extern UnkTask *gTaskCurrent;
s32 StageLoader_LoadCommonFiles(void);
void func_0204be48(void);
void func_0204be9c(void) { if (StageLoader_LoadCommonFiles() == 1) gTaskCurrent->fn = func_0204be48; }

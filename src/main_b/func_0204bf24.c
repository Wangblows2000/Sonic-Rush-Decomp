#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; void (*fn)(void); } UnkTask;
extern UnkTask *gTaskCurrent;
void Touch_Enable(void);
void func_0204bed0(void);
void func_0204bf24(void) { Touch_Enable(); gTaskCurrent->fn = func_0204bed0; }

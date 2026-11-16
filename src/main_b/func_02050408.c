#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 unk4; void (*fn)(void); } UnkTask;
extern UnkTask *gTaskCurrent;
BOOL func_02051644(void);
void func_020516b4(void);
void func_020503e4(void);
void func_02050408(void) { if (func_02051644()) { func_020516b4(); gTaskCurrent->fn = func_020503e4; } }

#pragma thumb reset

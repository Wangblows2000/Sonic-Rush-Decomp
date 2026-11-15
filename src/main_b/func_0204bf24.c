#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; void (*fn)(void); } UnkTask;
extern UnkTask *data_022b4574;
void func_02037d60(void);
void func_0204bed0(void);
void func_0204bf24(void) { func_02037d60(); data_022b4574->fn = func_0204bed0; }

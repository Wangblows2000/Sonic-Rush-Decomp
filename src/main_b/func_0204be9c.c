#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; void (*fn)(void); } UnkTask;
extern UnkTask *data_022b4574;
s32 func_0204b700(void);
void func_0204be48(void);
void func_0204be9c(void) { if (func_0204b700() == 1) data_022b4574->fn = func_0204be48; }

#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; void (*fn)(void); } UnkTask;
extern UnkTask *data_022b4574;
s32 func_0204b6cc(void);
void func_0204bdf8(void);
void func_0204be14(void) { if (func_0204b6cc() == 1) data_022b4574->fn = func_0204bdf8; }

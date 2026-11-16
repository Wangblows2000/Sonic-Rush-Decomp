#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; void (*fn)(void); } UnkStruct_0204bdf8;
extern UnkStruct_0204bdf8 *gTaskCurrent;
void func_0204bd94(void);
void func_0204bdf8(void) { gTaskCurrent->fn = func_0204bd94; }

#include <nitro/types.h>

typedef struct { void (*fn)(void); u8 unk4[8]; u16 unkC; } UnkStruct_0205cf10;
void func_0205cac8(void);
void func_0205cf10(void *a, UnkStruct_0205cf10 *s) { s->unkC = 0; s->fn = func_0205cac8; }

#include <nitro/types.h>

#pragma thumb on

typedef struct UnkStruct_02056104 { void (*fn)(void *, struct UnkStruct_02056104 *); } UnkStruct_02056104;
void func_02056104(void *a, UnkStruct_02056104 *s) { if (s->fn != NULL) s->fn(a, s); }

#pragma thumb reset

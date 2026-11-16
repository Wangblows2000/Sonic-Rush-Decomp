#include <nitro/types.h>

#pragma thumb on

typedef struct UnkStruct_02056104 { void (*fn)(void *, struct UnkStruct_02056104 *); } UnkStruct_02056104;
void PlayerDemoLight_Update(void *a, UnkStruct_02056104 *s) { if (s->fn != NULL) s->fn(a, s); }

#pragma thumb reset

// cc: 1.2/sp2p3 (thumb pop {r3}; bx r3 epilogue: matches 1.2/base..sp2p3, not sp3)
#include <nitro/types.h>

#pragma thumb on

typedef struct UnkStruct_02056104 { void (*fn)(void *, struct UnkStruct_02056104 *); } UnkStruct_02056104;
void func_02056104(void *a, UnkStruct_02056104 *s) { if (s->fn != NULL) s->fn(a, s); }

#pragma thumb reset

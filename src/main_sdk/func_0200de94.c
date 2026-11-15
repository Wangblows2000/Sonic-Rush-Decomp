// cc: 1.2/sp2p3
#include <nitro/types.h>

typedef struct UnkStruct_0200de94 { void (*fn)(void *); void *arg; struct UnkStruct_0200de94 *next; } UnkStruct_0200de94;
void func_0200de94(UnkStruct_0200de94 *p) { if (p == 0) return; do { p->fn(p->arg); p = p->next; } while (p != 0); }

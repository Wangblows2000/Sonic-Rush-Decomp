#include <nitro/types.h>

typedef struct UnkStruct_0200bf1c { struct UnkStruct_0200bf1c *prev; struct UnkStruct_0200bf1c *next; } UnkStruct_0200bf1c;
void func_0200bf1c(UnkStruct_0200bf1c *p) {
    UnkStruct_0200bf1c *prev = p->prev;
    UnkStruct_0200bf1c *next = p->next;
    if (prev != 0) prev->next = next;
    if (next != 0) next->prev = prev;
    p->prev = 0;
    p->next = p->prev;
}

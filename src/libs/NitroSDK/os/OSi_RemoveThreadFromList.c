#include <nitro/types.h>

typedef struct UnkStruct_02005fe4 { u8 pad[0x68]; struct UnkStruct_02005fe4 *next; } UnkStruct_02005fe4;
typedef struct { u8 pad[0xc]; UnkStruct_02005fe4 *list; } UnkStruct_0207d090;
extern UnkStruct_0207d090 data_0207d090;
void OSi_RemoveThreadFromList(UnkStruct_02005fe4 *t) {
    UnkStruct_02005fe4 *prev = 0;
    UnkStruct_02005fe4 *p = data_0207d090.list;
    while (p != 0 && p != t) { prev = p; p = p->next; }
    if (prev == 0) data_0207d090.list = t->next; else prev->next = t->next;
}

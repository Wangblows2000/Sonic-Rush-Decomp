#include <nitro/types.h>

typedef struct UnkStruct_02005b74 { u8 pad[0x64]; u32 state; struct UnkStruct_02005b74 *next; } UnkStruct_02005b74;
typedef struct { u8 pad[0xc]; UnkStruct_02005b74 *list; } UnkStruct_0207d090;
extern UnkStruct_0207d090 data_0207d090;
UnkStruct_02005b74 *OS_SelectThread(void) { UnkStruct_02005b74 *t = data_0207d090.list; while (t != 0 && t->state != 1) t = t->next; return t; }

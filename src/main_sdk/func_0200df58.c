#include <nitro/types.h>

typedef struct UnkStruct_0200df58 { struct UnkStruct_0200df58 *next; u32 pad; struct UnkStruct_0200df58 *link; } UnkStruct_0200df58;
void func_0200df58(UnkStruct_0200df58 *list, UnkStruct_0200df58 *item) { if (list != 0) { item->link = list->next; list->next = item; } }

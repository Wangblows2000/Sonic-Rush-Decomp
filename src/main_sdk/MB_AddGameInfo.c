#include <nitro/types.h>

typedef struct UnkStruct_02018038 { u8 pad[0x4bc]; struct UnkStruct_02018038 *next; } UnkStruct_02018038;
extern UnkStruct_02018038 *data_02080e74;
void MB_AddGameInfo(UnkStruct_02018038 *p) {
    UnkStruct_02018038 *q = data_02080e74;
    if (q == 0) data_02080e74 = p;
    else { while (q->next != 0) q = q->next; q->next = p; }
    p->next = 0;
}

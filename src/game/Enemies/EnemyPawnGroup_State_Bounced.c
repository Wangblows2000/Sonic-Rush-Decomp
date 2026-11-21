#include <nitro/types.h>

typedef struct UnkStruct_022fcce4 {
    u8 pad[0x5c];
    u16 unk5C;
} UnkStruct_022fcce4;

void EnemyPawnGroup_Action_Walk(UnkStruct_022fcce4 *s);

void EnemyPawnGroup_State_Bounced(UnkStruct_022fcce4 *s) {
    if (!(s->unk5C & 8))
        return;
    EnemyPawnGroup_Action_Walk(s);
}

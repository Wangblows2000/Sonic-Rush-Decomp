#include <nitro/types.h>

typedef struct UnkStruct_022fdf9c {
    u8 pad[0x5c];
    u16 unk5C;
} UnkStruct_022fdf9c;

void EnemyPawnGroup_Action_Walk(UnkStruct_022fdf9c *s);
void StageObj_HandleCollider(UnkStruct_022fdf9c *s, void *p);

void EnemyPawnGroup_State_Attack(UnkStruct_022fdf9c *s) {
    if (s->unk5C & 8) {
        EnemyPawnGroup_Action_Walk(s);
        return;
    }
    StageObj_HandleCollider(s, (u8 *)s + 0x20c);
}

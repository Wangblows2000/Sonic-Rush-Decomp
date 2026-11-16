#include <nitro/types.h>

typedef struct UnkStruct_022fdef0 {
    u8 pad[0xc];
    s32 unkC;
} UnkStruct_022fdef0;

void EnemyPawnGroup_Action_Unk_ov000_022fde64(UnkStruct_022fdef0 *s);

void EnemyPawnGroup_State_Unk_ov000_022fdef0(UnkStruct_022fdef0 *s) {
    s->unkC--;
    if (s->unkC != 0)
        return;
    EnemyPawnGroup_Action_Unk_ov000_022fde64(s);
}

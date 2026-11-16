#include <nitro/types.h>

typedef struct UnkStruct_02300688 {
    u8 pad[0x58];
    u32 unk58;
    u8 pad1[0x68 - 0x5c];
    void (*func)(void);
} UnkStruct_02300688;

void StageObj_SetAnimation(UnkStruct_02300688 *s, s32 a);
void EnemyFlapperGroup_State_PatrolTurn(void);

void EnemyFlapperGroup_Action_PatrolTurn(UnkStruct_02300688 *s) {
    StageObj_SetAnimation(s, 1);
    s->unk58 &= ~0x10;
    s->func = EnemyFlapperGroup_State_PatrolTurn;
}

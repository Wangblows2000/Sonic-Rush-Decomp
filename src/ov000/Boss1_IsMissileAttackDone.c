#include <nitro/types.h>

typedef struct UnkStruct_022cc114_2 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022cc114_2;

typedef struct UnkStruct_022cc114 {
    u8 pad[0x210];
    UnkStruct_022cc114_2 *unk210;
} UnkStruct_022cc114;

extern UnkStruct_022cc114 *gBoss1;

void Boss1Neck_State_MissileDone(void);

BOOL Boss1_IsMissileAttackDone(void) {
    return Boss1Neck_State_MissileDone == gBoss1->unk210->func;
}

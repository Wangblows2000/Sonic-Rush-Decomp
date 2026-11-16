#include <nitro/types.h>

typedef struct UnkStruct_022cc114_2 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022cc114_2;

typedef struct UnkStruct_022cc16c {
    u8 pad[0x210];
    UnkStruct_022cc114_2 *unk210;
} UnkStruct_022cc16c;

extern UnkStruct_022cc16c *gBoss1;

void Boss1Neck_State_HammerRollDone(void);

BOOL Boss1_IsHammerRollDone(void) {
    return Boss1Neck_State_HammerRollDone == gBoss1->unk210->func;
}

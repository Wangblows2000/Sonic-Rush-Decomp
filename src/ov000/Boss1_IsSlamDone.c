#include <nitro/types.h>

typedef struct UnkStruct_022cc114_2 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022cc114_2;

typedef struct UnkStruct_022cc1c4 {
    u8 pad[0x210];
    UnkStruct_022cc114_2 *unk210;
} UnkStruct_022cc1c4;

extern UnkStruct_022cc1c4 *gBoss1;

void Boss1Neck_State_SlamDone(void);

BOOL Boss1_IsSlamDone(void) {
    return Boss1Neck_State_SlamDone == gBoss1->unk210->func;
}

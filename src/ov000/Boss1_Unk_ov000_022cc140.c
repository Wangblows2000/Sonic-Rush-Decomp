#include <nitro/types.h>

typedef struct UnkStruct_022cc114_2 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022cc114_2;

typedef struct UnkStruct_022cc140 {
    u8 pad[0x210];
    UnkStruct_022cc114_2 *unk210;
} UnkStruct_022cc140;

extern UnkStruct_022cc140 *gBoss1;

void Boss1_State_Unk_ov000_022c7f34(void);

BOOL Boss1_Unk_ov000_022cc140(void) {
    return Boss1_State_Unk_ov000_022c7f34 == gBoss1->unk210->func;
}

#include <nitro/types.h>

typedef struct UnkStruct_022cc114_2 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022cc114_2;

typedef struct UnkStruct_022cc198 {
    u8 pad[0x210];
    UnkStruct_022cc114_2 *unk210;
} UnkStruct_022cc198;

extern UnkStruct_022cc198 *gBoss1;

void Boss1_State_Unk_ov000_022c8d64(void);

BOOL Boss1_Unk_ov000_022cc198(void) {
    return Boss1_State_Unk_ov000_022c8d64 == gBoss1->unk210->func;
}

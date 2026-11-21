#include <nitro/types.h>

typedef struct UnkStruct_022da8e4_2 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022da8e4_2;

typedef struct UnkStruct_022da8e4 {
    u8 pad[0x214];
    UnkStruct_022da8e4_2 *unk214;
} UnkStruct_022da8e4;

extern UnkStruct_022da8e4 *gBoss3;

void Boss3PartCC_State_HurtDone(void);

BOOL Boss3_IsHurtDone(void) {
    return gBoss3->unk214->func == Boss3PartCC_State_HurtDone;
}

#include <nitro/types.h>

typedef struct UnkStruct_022da8e4_2 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022da8e4_2;

typedef struct UnkStruct_022da910 {
    u8 pad[0x214];
    UnkStruct_022da8e4_2 *unk214;
} UnkStruct_022da910;

extern UnkStruct_022da910 *gBoss3;

void Boss3PartCC_State_Unk_ov000_022d6684(void);

BOOL Boss3_Unk_ov000_022da910(void) {
    return gBoss3->unk214->func == Boss3PartCC_State_Unk_ov000_022d6684;
}

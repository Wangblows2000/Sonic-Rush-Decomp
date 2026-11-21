#include <nitro/types.h>

typedef struct UnkStruct_022daa5c {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022daa5c;

extern UnkStruct_022daa5c *gBoss3;

void Boss3_SetBallInert(void *p);

void Boss3PartCC_MakeBallInert(void) {
    Boss3_SetBallInert(gBoss3->unk218);
}

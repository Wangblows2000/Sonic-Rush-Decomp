#include <nitro/types.h>

typedef struct UnkStruct_022daa94 {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022daa94;

extern UnkStruct_022daa94 *gBoss3;

void Boss3_SetBallHittable(void *p);

void Boss3_MakeBallHittable(void) {
    Boss3_SetBallHittable(gBoss3->unk218);
}

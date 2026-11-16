#include <nitro/types.h>

typedef struct UnkStruct_022daa94 {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022daa94;

extern UnkStruct_022daa94 *gBoss3;

void Boss3_Unk_ov000_022d5cc4(void *p);

void Boss3_Unk_ov000_022daa94(void) {
    Boss3_Unk_ov000_022d5cc4(gBoss3->unk218);
}

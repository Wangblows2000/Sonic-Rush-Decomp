#include <nitro/types.h>

typedef struct UnkStruct_022daa78 {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022daa78;

extern UnkStruct_022daa78 *gBoss3;

void Boss3_Unk_ov000_022d5bb0(void *p);

void Boss3_Unk_ov000_022daa78(void) {
    Boss3_Unk_ov000_022d5bb0(gBoss3->unk218);
}

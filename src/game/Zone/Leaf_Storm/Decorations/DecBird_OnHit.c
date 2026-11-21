#include <nitro/types.h>

typedef struct UnkStruct_02321d90 {
    u8 pad[0x18];
    void *unk18;
} UnkStruct_02321d90;

void DecBird_Action_FlyAway(void *a, void *b);

void DecBird_OnHit(UnkStruct_02321d90 *a, UnkStruct_02321d90 *b) {
    DecBird_Action_FlyAway(a->unk18, b->unk18);
}

#include <nitro/types.h>

typedef struct UnkStruct_02300e94 {
    u8 pad[0x18];
    void *unk18;
} UnkStruct_02300e94;

void KuragenBomb_Action_Explode(void *a, void *b);

void KuragenBomb_OnHit_Box1(UnkStruct_02300e94 *a, UnkStruct_02300e94 *b) {
    void *pb = b->unk18;
    void *pa = a->unk18;
    if (pb == NULL)
        return;
    KuragenBomb_Action_Explode(pa, pb);
}

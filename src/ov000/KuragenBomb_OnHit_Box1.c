#include <nitro/types.h>

typedef struct UnkStruct_02300e94 {
    u8 pad[0x18];
    void *unk18;
} UnkStruct_02300e94;

void Kuragen_Action_Unk_ov000_02300ec8(void *a, void *b);

void KuragenBomb_OnHit_Box1(UnkStruct_02300e94 *a, UnkStruct_02300e94 *b) {
    void *pb = b->unk18;
    void *pa = a->unk18;
    if (pb == NULL)
        return;
    Kuragen_Action_Unk_ov000_02300ec8(pa, pb);
}

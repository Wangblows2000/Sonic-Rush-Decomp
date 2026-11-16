#include <nitro/types.h>

void *SpecialStageActObj_SpawnAt(s32 a, s32 x, s32 y, s32 z);
void SpecialStage_Unk_ov001_0231066c(void *a, void *b);
void SpecialStageGimmick_Destroy(void *p);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void SpecialStagePlayer_ChangeState(void *p, s32 v);
void SpecialStageCamera_SetMode(s32 v);

void SpecialStagePlayer_OnHurt(u8 *p, u8 *o) {
    u8 *q;
    if (*(s32 *)(p + 0x74) > 0) {
        return;
    }
    switch (*(s16 *)(o + 0x68)) {
    case 0xd:
    case 0xe:
        q = SpecialStageActObj_SpawnAt(1, *(s32 *)(o + 0x20), *(s32 *)(o + 0x24), *(s32 *)(o + 0x28));
        if (q != NULL) {
            *(s32 *)(*(u8 **)(q + 0xac) + 0x114) = 0x2000;
            SpecialStage_Unk_ov001_0231066c(NULL, q);
        }
        SpecialStageGimmick_Destroy(o);
        Sound_PlaySeqArc(0, 0x10, 0x11);
        break;
    }
    SpecialStagePlayer_ChangeState(p, 0x10);
    SpecialStageCamera_SetMode(2);
}

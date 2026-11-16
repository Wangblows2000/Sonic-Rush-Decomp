#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x434 - 0x20C];
    s32 unk434;
    u16 unk438;
    u8 unk43A[2];
    u8 pad43C[0x458 - 0x43C];
    s32 unk458;
    u8 pad45C[0x57C - 0x45C];
    u8 unk57C;
} UnkStruct_022f35e4;

extern u8 data_ov000_02363e48[];

void BossF_UpdateCameraShake(void *p, s32 a, s32 b);
void BossFPartD8_TrackPlayerX_Offset(UnkStruct_022f35e4 *s, s32 a);
void BossF_StartNextAttack(void);
void BossFPartD8_State_HandSlamNext(void);

void BossFPartD8_State_HandSlamLiftWait(UnkStruct_022f35e4 *s) {
    BossF_UpdateCameraShake(s->unk43A, 2, 6);
    if (s->unk438 < data_ov000_02363e48[s->unk458])
        BossFPartD8_TrackPlayerX_Offset(s, s->unk434 == 0);
    if (!(s->unk57C & 2))
        return;
    if (s->unk438 < data_ov000_02363e48[s->unk458]) {
        s->unk434 = s->unk434 == 0;
        s->base.unk68 = BossFPartD8_State_HandSlamNext;
        return;
    }
    BossF_StartNextAttack();
}

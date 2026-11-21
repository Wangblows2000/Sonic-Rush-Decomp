#include <game/Stage/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x328 - 0x20C];
    s16 unk328;
} UnkStruct_Player;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x434 - 0x20C];
    s32 unk434;
    u8 pad438[0x57C - 0x438];
    u8 unk57C;
} UnkStruct_022f12c0;

extern UnkStruct_Player *gPlayers;

void BossFPartD8_TrackPlayerX_NearSlow(UnkStruct_022f12c0 *s);
void BossF_StartNextAttack(void);
void BossFPartD8_RollThrowVariant(void);
void BossFPartD8_TriggerPartDAThrow(void);

void BossFPartD8_State_PartDCVolleyEndWait(UnkStruct_022f12c0 *s) {
    if ((u16)(s16)(gPlayers->unk328 - 0xf) <= 1)
        s->unk434 = 1;
    BossFPartD8_TrackPlayerX_NearSlow(s);
    if (!(s->unk57C & 2))
        return;
    if (s->unk434 != 0) {
        BossF_StartNextAttack();
        return;
    }
    BossFPartD8_RollThrowVariant();
    BossFPartD8_TriggerPartDAThrow();
}

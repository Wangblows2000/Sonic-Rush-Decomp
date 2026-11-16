#include <ov000_c/stageobj.h>

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

void BossFPartD8_Unk_ov000_022f490c(UnkStruct_022f12c0 *s);
void BossF_Unk_ov000_022f898c(void);
void BossFPartD8_Unk_ov000_022f3f8c(void);
void BossFPartD8_Unk_ov000_022f88dc(void);

void BossFPartD8_State_Unk_ov000_022f12c0(UnkStruct_022f12c0 *s) {
    if ((u16)(s16)(gPlayers->unk328 - 0xf) <= 1)
        s->unk434 = 1;
    BossFPartD8_Unk_ov000_022f490c(s);
    if (!(s->unk57C & 2))
        return;
    if (s->unk434 != 0) {
        BossF_Unk_ov000_022f898c();
        return;
    }
    BossFPartD8_Unk_ov000_022f3f8c();
    BossFPartD8_Unk_ov000_022f88dc();
}

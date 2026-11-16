#include <ov000_c/stageobj.h>

typedef struct {
    u32 unk0;
    u8 pad[0x10 - 4];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    u8 pad[8];
    u16 unk8;
} UnkStruct_02369ac0;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x430 - 0x20C];
    u16 unk430;
    u8 pad432[0x436 - 0x432];
    u16 unk436;
    u16 unk438;
    u16 unk43A;
} UnkStruct_022f2724;

extern UnkStruct_022c4560 gGameState;
extern UnkStruct_02369ac0 gBossStatus;
extern u8 data_ov000_02363edc[][8];
extern u16 data_ov000_02363fdc[][8];

void BossF_UpdateCameraShake(u16 *p, s32 a, s32 b);
void BossFPartD8_Unk_ov000_022f8bc8(u32 a, s32 b);
void BossFPartD8_Unk_ov000_022f89f4(void);
void BossFPartD8_State_Unk_ov000_022f2680(void);

static inline s32 GetIdxB(void) {
    BOOL b = TRUE;
    if (gGameState.unk10 != 2 && gGameState.unk10 != 3)
        b = FALSE;
    if (b)
        return 1;
    return gGameState.unk14;
}

void BossFPartD8_State_Unk_ov000_022f2724(UnkStruct_022f2724 *s) {
    BossF_UpdateCameraShake(&s->unk436, 2, 6);
    s->unk43A++;
    if (!(s->unk43A & 3)) {
        if (s->unk43A & 0x8000)
            BossFPartD8_Unk_ov000_022f8bc8(gGameState.unk0, 0);
        else
            BossFPartD8_Unk_ov000_022f8bc8(gGameState.unk0, 1);
        s->unk43A &= ~3;
        s->unk43A ^= 0x8000;
    }
    if (data_ov000_02363edc[GetIdxB()][gBossStatus.unk8 - 1] != 0) {
        if (s->unk430-- == 0) {
            BossFPartD8_Unk_ov000_022f89f4();
            s->unk430 = data_ov000_02363fdc[GetIdxB()][gBossStatus.unk8 - 1];
        }
    }
    if (s->unk438 != 0)
        s->unk438--;
    if (s->unk438 != 0)
        return;
    if (!(s->unk43A & 0x8000))
        s->base.unk68 = BossFPartD8_State_Unk_ov000_022f2680;
}

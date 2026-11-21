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
    u8 pad432[0x434 - 0x432];
    u16 unk434;
} UnkStruct_022f228c;

extern UnkStruct_022c4560 gGameState;
extern UnkStruct_02369ac0 gBossStatus;
extern u8 data_ov000_02363edc[][8];
extern u16 data_ov000_02363fdc[][8];

void BossFPartD8_BlinkPalette(UnkStruct_022f228c *s);
void BossFPartD8_SpawnPartD9(void);
void BossFPartD8_State_HandRideStartWarn(void);

static inline s32 GetIdxB(void) {
    BOOL b = TRUE;
    if (gGameState.unk10 != 2 && gGameState.unk10 != 3)
        b = FALSE;
    if (b)
        return 1;
    return gGameState.unk14;
}

void BossFPartD8_State_HandRideWait(UnkStruct_022f228c *s) {
    BossFPartD8_BlinkPalette(s);
    if (data_ov000_02363edc[GetIdxB()][gBossStatus.unk8 - 1] != 0) {
        if (s->unk430-- == 0) {
            BossFPartD8_SpawnPartD9();
            s->unk430 = data_ov000_02363fdc[GetIdxB()][gBossStatus.unk8 - 1];
        }
    }
    s->unk434--;
    if (s->unk434 == 0)
        s->base.unk68 = BossFPartD8_State_HandRideStartWarn;
}

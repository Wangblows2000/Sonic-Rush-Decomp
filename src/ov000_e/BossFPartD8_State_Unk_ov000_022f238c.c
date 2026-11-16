#include <ov000_c/stageobj.h>

typedef struct {
    u32 unk0;
    u8 pad[0x10 - 4];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    u8 pad[4];
    s32 unk4;
    u16 unk8;
} UnkStruct_02369ac0;

typedef struct {
    u8 pad[0x20C];
    u32 unk20C;
} UnkStruct_Mgr;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x434 - 0x20C];
    u16 unk434;
    u16 unk436;
} UnkStruct_022f238c;

extern UnkStruct_022c4560 gGameState;
extern UnkStruct_02369ac0 gBossStatus;
extern UnkStruct_Mgr *gBossF;
extern u16 data_ov000_023641e8[][8][3];

void func_ov000_022f4850(void);
void BossFPartD8_State_Unk_ov000_022f228c(void);

static inline s32 GetIdxB(void) {
    BOOL b = TRUE;
    if (gGameState.unk10 != 2 && gGameState.unk10 != 3)
        b = FALSE;
    if (b)
        return 1;
    return gGameState.unk14;
}

void BossFPartD8_State_Unk_ov000_022f238c(UnkStruct_022f238c *s) {
    UnkStruct_Mgr *m = gBossF;

    s->unk434 = data_ov000_023641e8[GetIdxB()][gBossStatus.unk8 - 1][s->unk436];
    func_ov000_022f4850();
    m->unk20C |= 0x40;
    gBossStatus.unk4 = 0x33;
    s->base.unk68 = BossFPartD8_State_Unk_ov000_022f228c;
}

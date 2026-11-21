#include <game/Stage/stageobj.h>

typedef struct {
    u32 unk0;
    u8 pad[0x10 - 4];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x42C - 0x20C];
    u16 unk42C;
    u16 unk42E;
    u8 pad430[0x440 - 0x430];
    void *unk440;
} UnkStruct_022e779c;

extern UnkStruct_022c4560 gGameState;
extern u16 data_ov000_02363b94[][4];

void Boss7PartD5_MoveState_StartAirAttackCharge(UnkStruct_022e779c *w);
void Boss7_SetRivalAnimation(UnkStruct_022e779c *w, s32 st);
void Boss7PartD5_MoveState_BlazeSpecialFall(void);

static inline s32 GetIdxB(void) {
    BOOL b = TRUE;
    if (gGameState.unk10 != 2 && gGameState.unk10 != 3)
        b = FALSE;
    if (b)
        return 1;
    return gGameState.unk14;
}

void Boss7PartD5_MoveState_BlazeSpecialEnd(UnkStruct_022e779c *w) {
    u16 t;

    t = data_ov000_02363b94[GetIdxB()][w->unk42E];
    if (t >= (Rand() & 0xf)) {
        w->unk42C = w->unk42E;
        Boss7PartD5_MoveState_StartAirAttackCharge(w);
        return;
    }
    Boss7_SetRivalAnimation(w, 0x13);
    w->base.unk58 |= 0x10;
    w->base.unk58 |= 0x80;
    w->base.unk30 = -0x280;
    w->unk440 = Boss7PartD5_MoveState_BlazeSpecialFall;
}

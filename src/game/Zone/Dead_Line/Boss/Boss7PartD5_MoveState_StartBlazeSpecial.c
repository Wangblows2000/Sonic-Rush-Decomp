#include <game/Stage/stageobj.h>

typedef struct UnkStruct_022e7a84 {
    UnkStruct_StageObj base;
    u8 pad20C[0x42E - 0x20C];
    u16 unk42E;
    u8 pad430[0x440 - 0x430];
    void (*unk440)(struct UnkStruct_022e7a84 *);
    u8 pad444[0x464 - 0x444];
    u16 unk464;
    u16 unk466;
} UnkStruct_022e7a84;

typedef struct {
    UnkStruct_StageObj base;
    s32 unk20C;
    s32 unk210;
} UnkStruct_Boss;

extern UnkStruct_Boss *gBoss7;
extern s16 gPlayerPhysicsTable[][0x18];
extern u16 data_ov000_02363b24[];

void Boss7_SetRivalAnimation(UnkStruct_022e7a84 *s, s32 st);
void Boss7_SetCollidersEnabled(UnkStruct_022e7a84 *s, s32 a, s32 b);
void Boss7PartD5_MoveState_BlazeSpecialRise(UnkStruct_022e7a84 *s);

void Boss7PartD5_MoveState_StartBlazeSpecial(UnkStruct_022e7a84 *w) {
    UnkStruct_Boss *b = gBoss7;

    Boss7_SetRivalAnimation(w, 0x13);
    Boss7_SetCollidersEnabled(w, 0, 0);
    w->base.unk58 |= 0x10;
    w->base.unk30 = -(gPlayerPhysicsTable[1][0x13] + 0x100);
    if (w->base.unk10 < 0x4000) {
        w->base.unk2E = 0x180;
        w->base.unk5C &= ~1;
    } else if (w->base.unk10 > 0x10000) {
        w->base.unk2E = -0x180;
        w->base.unk5C |= 1;
    }
    b->unk210 = 2;
    w->unk464 = data_ov000_02363b24[w->unk42E];
    w->unk466 = 0;
    w->unk440 = Boss7PartD5_MoveState_BlazeSpecialRise;
    w->unk440(w);
}

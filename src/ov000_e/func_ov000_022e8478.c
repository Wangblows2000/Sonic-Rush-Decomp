#include <ov000_c/stageobj.h>

typedef struct UnkStruct_022e8478 {
    UnkStruct_StageObj base;
    u8 pad20C[0x3D0 - 0x20C];
    s32 unk3D0;
    u8 pad3D4[0x440 - 0x3D4];
    void *unk440;
    u8 pad444[0x464 - 0x444];
    u16 unk464;
    u16 unk466;
} UnkStruct_022e8478;

typedef struct {
    UnkStruct_StageObj base;
    s32 unk20C;
    s32 unk210;
} UnkStruct_Boss;

extern UnkStruct_Boss *gBoss7;
extern s16 data_ov000_02368ba2[][0x18];

void func_ov000_022ee198(UnkStruct_022e8478 *s, s32 st);
void func_ov000_022e9b70(UnkStruct_022e8478 *s, s32 a, s32 b);
void Boss7PartD5_MoveState_Unk_ov000_022e8454(void);

void func_ov000_022e8478(UnkStruct_022e8478 *w) {
    UnkStruct_Boss *b = gBoss7;

    func_ov000_022ee198(w, 0x31);
    func_ov000_022e9b70(w, 0, 0);
    if (w->base.unk10 < 0x4000) {
        w->base.unk2E = 0x180;
        w->base.unk5C &= ~1;
    } else if (w->base.unk10 > 0x10000) {
        w->base.unk2E = -0x180;
        w->base.unk5C |= 1;
    }
    w->base.unk58 |= 0x10;
    w->base.unk30 = -data_ov000_02368ba2[w->unk3D0][0];
    b->unk210 = 2;
    w->unk440 = Boss7PartD5_MoveState_Unk_ov000_022e8454;
}

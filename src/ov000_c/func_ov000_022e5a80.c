#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x90];
    u8 unk90[0x10C - 0x90];
    u8 unk10C;
    u8 pad10D[0x114 - 0x10D];
    s32 unk114;
} UnkStruct_Mdl960;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x960 - 0x20C];
    UnkStruct_Mdl960 unk960;
    u8 pad[0xD08 - 0x960 - sizeof(UnkStruct_Mdl960)];
    void *unkD08;
    u8 padD0C[0xD60 - 0xD0C];
    UnkStruct_Coll unkD60[11];
    u8 padF54[0xF48 - 0xD60 - 11 * 0x2C];
    u16 unkF48;
    u8 padF4A[0xF50 - 0xF4A];
    u32 unkF50;
    s32 unkF54;
    s32 unkF58;
    s32 unkF5C;
    s16 unkF60;
    u8 padF62[2];
    s16 unkF64;
    u8 padF66[2];
    u16 unkF68;
    u16 unkF6A;
    u16 unkF6C;
} UnkStruct_Boss;

extern UnkStruct_Boss *gBoss6;
extern UnkStruct_StageObj *gPlayers;
extern u32 data_ov000_023637ac[];

void func_02021c38(void *a, void (*f)(void), s32 b, s32 c, s32 d);
void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);
void func_ov000_022e62fc(void);
void Boss6_State_Unk_ov000_022e5980(void);

void func_ov000_022e5a80(void) {
    UnkStruct_Boss *w = gBoss6;
    UnkStruct_Mdl960 *m = &w->unk960;
    s32 i;

    func_02021c38(m->unk90, func_ov000_022e62fc, 0, 6, 3);
    Model_SetAnimation(m, 0, w->unkD08, 0, 0);
    m->unk10C |= 4;
    m->unk114 = 0x1000;
    for (i = 0; i < 11; i++) {
        w->unkD60[i].unkA &= ~4;
        w->unkD60[i].unkA |= 0x1000;
    }
    w->unkF50 = data_ov000_023637ac[Rand() & 3];
    if (w->unkF48 != 1 && w->unkF48 != 0) {
        if (gPlayers->unk10 < w->base.unk10) {
            w->unkF54 = -0x17;
            w->unkF5C = -0x580;
            w->unkF60 = 0x5b;
            w->unkF64 = 0x1555;
        } else {
            w->unkF54 = 0x17;
            w->unkF5C = 0x580;
            w->unkF60 = -0x5b;
            w->unkF64 = -0x1555;
        }
        w->unkF58 = 0;
    }
    w->unkF68 = 0x549f;
    w->unkF6A = 0;
    w->unkF6C = 0x549f;
    w->base.unk68 = Boss6_State_Unk_ov000_022e5980;
}

#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x10C];
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
    u8 padD0C[0xD5E - 0xD0C];
    u16 unkD5E;
    u8 padD60[0xE1A - 0xD60];
    u16 unkE1A;
    u8 padE1C[0xEF6 - 0xE1C];
    u16 unkEF6;
    u8 padEF8[0xF4C - 0xEF8];
    s16 unkF4C;
    u8 padF4E[0x111C - 0xF4E];
    s16 unk111C;
    u8 pad111E[2];
    s32 unk1120;
    s16 unk1124;
} UnkStruct_Boss;

extern UnkStruct_StageObj *gPlayers;

void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);
void func_ov000_022dffbc(UnkStruct_Boss *w, s32 a);
void func_020366f4(s32 a, s32 b, s32 c);
void func_ov000_022e2d0c(void);

void func_ov000_022e2fbc(UnkStruct_Boss *w) {
    UnkStruct_Mdl960 *m = &w->unk960;
    s32 d;

    w->unk111C += 0xd0;
    w->base.unk14 += w->unk111C;
    if (w->unk111C >= 0) {
        w->base.unk68 = func_ov000_022e2d0c;
        Model_SetAnimation(m, 0, w->unkD08, 0x12, 0);
        m->unk10C &= ~4;
        m->unk114 = 0x1000;
        if (gPlayers->unk10 - w->base.unk10 < 0) {
            if (w->unkD5E != 0) {
                w->unk1124 = 0x38e;
                func_ov000_022dffbc(w, 1);
            } else {
                w->unk1124 = -0x38e;
                func_ov000_022dffbc(w, 0);
            }
        } else {
            if (w->unkD5E != 0) {
                w->unk1124 = -0x38e;
                func_ov000_022dffbc(w, 0);
            } else {
                w->unk1124 = 0x38e;
                func_ov000_022dffbc(w, 1);
            }
        }
        w->unkE1A |= 4;
        w->unkE1A &= ~0x1000;
        w->unkEF6 &= ~4;
        w->unkEF6 |= 0x1000;
        func_020366f4(0, 0, 0xd0);
    }
    if (w->unk1120 != 0) {
        d = gPlayers->unk10 - w->base.unk10;
        w->unk1120--;
        if (d < -0x1000) {
            w->base.unk10 -= 0xa00;
            if (w->base.unk10 < 0x7700)
                w->base.unk10 = 0x7700;
        } else if (d > 0x1000) {
            w->base.unk10 += 0xa00;
            if (w->base.unk10 > 0x2c800)
                w->base.unk10 = 0x2c800;
        }
    }
    w->unkF4C += w->unk1124;
    if (w->unk1124 < 0) {
        if (w->unkF4C < 0)
            w->unkF4C = 0;
    } else if ((u16)w->unkF4C > 0x8000) {
        w->unkF4C = -0x8000;
    }
}

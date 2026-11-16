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
    u8 padD0C[0xD96 - 0xD0C];
    u16 unkD96;
    u8 padD98[0xE46 - 0xD98];
    u16 unkE46;
    u8 padE48[0xEF6 - 0xE48];
    u16 unkEF6;
    u8 padEF8[0xF22 - 0xEF8];
    u16 unkF22;
    u8 padF24[0xF4C - 0xF24];
    s16 unkF4C;
    u8 padF4E[0xF50 - 0xF4E];
    s32 unkF50;
    u8 padF54[0x1068 - 0xF54];
    s32 unk1068;
    u8 pad106C[4];
    s32 unk1070;
    u8 pad1074[0x111C - 0x1074];
    s16 unk111C;
    u8 pad111E[2];
    s32 unk1120;
    s16 unk1124;
} UnkStruct_Boss;

typedef struct {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    u8 pad[8];
    u16 unk8;
} UnkStruct_02369ac0;

typedef struct {
    u8 pad[0x30];
    u16 unk30;
    u8 pad1[0x6c - 0x32];
    u16 unk6C;
} UnkStruct_0236a48c;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad[0x32c - 0x20c];
    u32 unk32C;
} UnkStruct_Player;

extern UnkStruct_Player *data_ov000_0236ac60;
extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_02369ac0 data_ov000_02369ac0;
extern s32 data_ov000_02363958[][8];
extern UnkStruct_0236a48c data_ov000_0236a48c;

void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);
void ScreenShake_Start(s32 a);
void func_ov000_02328670(s32 a, s32 b, s32 c, s32 d);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void func_ov000_022e2bb0(void);

void func_ov000_022e2d0c(UnkStruct_Boss *w) {
    UnkStruct_Mdl960 *m = &w->unk960;
    s32 d;
    BOOL b;
    s32 i;

    w->unk111C += 0xd0;
    w->base.unk14 += w->unk111C;
    if (w->base.unk14 >= 0x20000) {
        w->base.unk14 = 0x20000;
        w->base.unk68 = func_ov000_022e2bb0;
        Model_SetAnimation(m, 0, w->unkD08, 0x14, 0);
        m->unk10C &= ~4;
        b = TRUE;
        if (data_022c4560.unk10 != 2 && data_022c4560.unk10 != 3)
            b = FALSE;
        if (b)
            i = 1;
        else
            i = data_022c4560.unk14;
        m->unk114 = data_ov000_02363958[i][data_ov000_02369ac0.unk8 - 1];
        ScreenShake_Start(0xc);
        w->unkF50 = 0x14;
        func_ov000_02328670(1, w->unk1068 >> 4, 0x20000, w->unk1070 >> 4);
        w->unkD96 |= 4;
        w->unkD96 &= ~0x1000;
        w->unkF22 |= 4;
        w->unkF22 &= ~0x1000;
        w->unkE46 |= 4;
        w->unkE46 &= ~0x1000;
        w->unkEF6 |= 4;
        w->unkEF6 &= ~0x1000;
        Sound_PlaySeqArc(0, 0, 0x86);
    } else if (m->unk10C & 2) {
        Model_SetAnimation(m, 0, w->unkD08, 0x13, 0);
        m->unk10C &= ~4;
        m->unk114 = 0x1000;
    }
    if (w->unk1120 != 0) {
        d = data_ov000_0236ac60->base.unk10 - w->base.unk10;
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
    if (data_ov000_0236ac60->unk32C & 0x400) {
        UnkStruct_0236a48c *c = (UnkStruct_0236a48c *)(u32)&data_ov000_0236a48c;
        c->unk30 = 0x10;
        c->unk6C = 0x10;
    }
}

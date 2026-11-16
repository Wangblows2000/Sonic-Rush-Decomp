#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x10C];
    u8 unk10C;
    u8 pad10D[0x114 - 0x10D];
    s32 unk114;
} UnkStruct_Mdl960;

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    u8 pad210[0x960 - 0x210];
    UnkStruct_Mdl960 unk960;
    u8 pad[0xD5E - 0x960 - sizeof(UnkStruct_Mdl960)];
    u16 unkD5E;
    u8 padD60[0xF50 - 0xD60];
    s32 unkF50;
    u8 padF54[0x1068 - 0xF54];
    s32 unk1068;
    u8 pad106C[4];
    s32 unk1070;
    u8 pad1074[0x1118 - 0x1074];
    s32 unk1118;
    s32 unk111C;
    s32 unk1120;
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
    u8 pad[0x3c];
    s32 unk3C;
    s32 unk40;
} UnkStruct_0236a444;

extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_02369ac0 data_ov000_02369ac0;
extern UnkStruct_0236a444 data_ov000_0236a444;
extern s32 data_ov000_02363a18[][8][4];

s32 ScreenShake_GetOffsetX(void);
s32 ScreenShake_GetOffsetY(void);
s32 func_0200225c(s32 a, s32 b);
void BossEffect_CreatePick(s32 a, s32 b, s32 c, s32 d);
void BossEffect_CreateParts(s32 a, s32 b, s32 c, s32 d);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void func_ov000_022e4834(void);

void func_ov000_022e49b0(UnkStruct_Boss *w) {
    UnkStruct_Mdl960 *m = &w->unk960;
    s32 k;
    u16 r;
    BOOL b;
    s32 i;
    s32 d;

    k = w->unkF50 << 12;
    data_ov000_0236a444.unk3C = func_0200225c(k * ScreenShake_GetOffsetX(), 0x1e000);
    data_ov000_0236a444.unk3C = data_ov000_0236a444.unk3C << 14;
    data_ov000_0236a444.unk3C = data_ov000_0236a444.unk3C >> 12;
    data_ov000_0236a444.unk40 = func_0200225c(k * ScreenShake_GetOffsetY(), 0x1e000);
    data_ov000_0236a444.unk40 = data_ov000_0236a444.unk40 << 14;
    data_ov000_0236a444.unk40 = data_ov000_0236a444.unk3C >> 12;
    w->unkF50--;
    if (w->unkF50 < 0)
        w->unkF50 = 0;
    w->unk20C |= 0x1000;
    w->unk1118--;
    if (w->unk1118 > 0)
        return;
    w->base.unk68 = func_ov000_022e4834;
    r = Rand();
    b = TRUE;
    if (data_022c4560.unk10 != 2 && data_022c4560.unk10 != 3)
        b = FALSE;
    if (b)
        i = 1;
    else
        i = data_022c4560.unk14;
    m->unk114 = data_ov000_02363a18[i][data_ov000_02369ac0.unk8 - 1][r & 3];
    w->unk1120 = 4;
    w->unkF50 = 0x3c;
    if (w->unkD5E != 0)
        d = -0x4200;
    else
        d = 0x4200;
    BossEffect_CreatePick(1, w->unk1068 >> 4, 0x20000, d + (w->unk1070 >> 4));
    BossEffect_CreateParts(1, w->unk1068 >> 4, 0x20000, d + (w->unk1070 >> 4));
    Sound_PlaySeqArc(0, 0, 0x8d);
}

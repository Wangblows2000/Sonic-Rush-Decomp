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
    u8 pad[0xD08 - 0x960 - sizeof(UnkStruct_Mdl960)];
    void *unkD08;
    u8 padD0C[0xD5E - 0xD0C];
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
    u8 pad[0x430];
    u16 unk430;
    u16 unk432;
    u16 unk434;
    u16 unk436;
} UnkStruct_Player;

extern UnkStruct_Player *data_ov000_0236ac60;

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

s32 func_02044690(void);
s32 func_02044670(void);
s32 func_0200225c(s32 a, s32 b);
void func_ov000_023284d8(s32 a, s32 b, s32 c, s32 d);
void func_ov000_023284a0(s32 a, s32 b, s32 c, s32 d);
void func_02036754(s32 a, s32 b, s32 c);
void func_ov000_022e47b8(void);
void func_02031a68(void *s, s32 a, void *b, s32 c, s32 d);
void func_ov000_022e04a8(UnkStruct_Boss *w);

void func_ov000_022e4834(UnkStruct_Boss *w) {
    UnkStruct_Mdl960 *m = &w->unk960;
    s32 k;

    w->unkF50--;
    if (w->unkF50 < 0)
        w->unkF50 = 0;
    k = w->unkF50 << 12;
    data_ov000_0236a444.unk3C = func_0200225c(k * func_02044690(), 0x3c000);
    data_ov000_0236a444.unk3C = data_ov000_0236a444.unk3C << 12;
    data_ov000_0236a444.unk3C = data_ov000_0236a444.unk3C >> 12;
    data_ov000_0236a444.unk40 = func_0200225c(k * func_02044670(), 0x3c000);
    data_ov000_0236a444.unk40 = data_ov000_0236a444.unk40 << 12;
    data_ov000_0236a444.unk40 = data_ov000_0236a444.unk3C >> 12;
    w->unk1120--;
    if (w->unk1120 >= 0)
        w->unk20C |= 0x2000;
    w->unk20C |= 0x4000;
    if (!(m->unk10C & 2))
        return;
    w->base.unk68 = func_ov000_022e47b8;
    func_02031a68(m, 0, w->unkD08, 5, 0);
    m->unk114 = 0x1000;
    func_020446b0(0);
    data_ov000_0236ac60->unk434 = 1;
    data_ov000_0236ac60->unk436 = 2;
    data_ov000_0236ac60->unk430 = 0x40;
    data_ov000_0236ac60->unk432 = 0x80;
    func_ov000_022e04a8(w);
    w->unk111C = 8;
    func_02036754(0, 0, 0x84);
}

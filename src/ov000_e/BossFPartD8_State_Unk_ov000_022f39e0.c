#include <ov000_c/stageobj.h>

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
    u8 pad[0x24];
    s32 unk24;
    s32 unk28;
} UnkStruct_Pos;

typedef struct {
    u8 pad[0xa];
    u16 unkA;
} UnkStruct_Hit;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x238 - 0x20C];
    UnkStruct_Hit unk238;
    u8 pad23C[0x2BC - 0x238 - sizeof(UnkStruct_Hit)];
    UnkStruct_Hit unk2BC;
    u8 pad2C0[0x3D0 - 0x2BC - sizeof(UnkStruct_Hit)];
    UnkStruct_Pos unk3D0;
    u8 pad3FC[0x400 - 0x3D0 - sizeof(UnkStruct_Pos)];
    UnkStruct_Pos unk400;
    u8 pad42C[0x434 - 0x400 - sizeof(UnkStruct_Pos)];
    s32 unk434;
    u8 pad438[2];
    u16 unk43A;
    u8 pad43C[0x458 - 0x43C];
    s32 unk458;
    u8 pad45C[0x57C - 0x45C];
    u8 unk57C;
} UnkStruct_022f39e0;

extern UnkStruct_022c4560 gGameState;
extern UnkStruct_02369ac0 gBossStatus;
extern u8 data_ov000_02363e9c[][8];

void func_ov000_022f8fa0(u16 *p, s32 a, s32 b);
s32 MapObject_Spawn(s32 a, s32 b, s32 c, u16 d, s32 e, s32 f, s32 g, s32 h, s32 i);
void BossEffect_CreateImpactSl(s32 a, s32 b, s32 c, s32 d);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void BossFPartD8_State_Unk_ov000_022f3874(UnkStruct_022f39e0 *s);

void BossFPartD8_State_Unk_ov000_022f39e0(UnkStruct_022f39e0 *s) {
    UnkStruct_Pos *p;
    UnkStruct_Hit *h;
    s32 d;
    BOOL b;
    s32 i;
    u16 fl;

    func_ov000_022f8fa0(&s->unk43A, 2, 6);
    p = s->unk434 ? &s->unk3D0 : &s->unk400;
    h = s->unk434 ? &s->unk238 : &s->unk2BC;
    if (p->unk28 < -0x200000)
        h->unkA |= 4;
    else
        h->unkA &= ~4;
    if (!(s->unk57C & 2))
        return;
    d = s->unk434 ? -0x1000 : 0x1000;
    b = TRUE;
    if (gGameState.unk10 != 2 && gGameState.unk10 != 3)
        b = FALSE;
    if (b)
        i = 1;
    else
        i = gGameState.unk14;
    if (data_ov000_02363e9c[i][gBossStatus.unk8 - 1] != 0) {
        fl = 0;
        if (s->unk458 >= 6 && s->unk434 == 0)
            fl |= 4;
        MapObject_Spawn(0xdb, (p->unk24 >> 4) + 0x3000, 0x30000, fl, 0, 0, 0, 0, 0);
        if (s->unk458 >= 6)
            fl ^= 4;
        MapObject_Spawn(0xdb, (p->unk24 >> 4) - 0x3000, 0x30000, fl | 1, 0, 0, 0, 0, 0);
    }
    BossEffect_CreateImpactSl(1, d + (p->unk24 >> 4), 0x30000, 0);
    s->unk43A = 0x40;
    Sound_PlaySeqArc(0, 0, 0xc0);
    s->base.unk68 = BossFPartD8_State_Unk_ov000_022f3874;
}

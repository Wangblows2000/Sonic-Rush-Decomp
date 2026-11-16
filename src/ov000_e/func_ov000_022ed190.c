#include <ov000_c/stageobj.h>

typedef struct {
    u16 b0 : 5;
    u16 b5 : 11;
} UnkStruct_Bits5;

typedef struct {
    u8 pad0[0x24];
    UnkStruct_Bits5 unk24;
} UnkStruct_0208922c;

typedef struct {
    u8 pad[0x3c];
    s32 unk3C;
    s32 unk40;
    s32 unk44;
} UnkStruct_0236a444;

typedef struct {
    u8 pad[4];
    u16 unk4;
} UnkStruct_Pad;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad[0x328 - 0x20c];
    s16 unk328;
    u8 pad32A[0x32c - 0x32a];
    u32 unk32C;
    u8 pad330[0x3dc - 0x330];
    u16 unk3DC;
} UnkStruct_Player;

typedef struct UnkStruct_022ed190 UnkStruct_022ed190;
struct UnkStruct_022ed190 {
    UnkStruct_StageObj base;
    u8 pad20C[0x214 - 0x20C];
    UnkStruct_Player *unk214;
    UnkStruct_StageObj *unk218;
    UnkStruct_StageObj *unk21C;
    u8 pad220[0x232 - 0x220];
    u16 unk232;
    u8 pad234[0x384 - 0x234];
    s32 unk384;
    u8 pad388[4];
    s32 unk38C;
    s32 unk390;
    u16 unk394;
};

extern UnkStruct_Player *data_ov000_0236ac60;
extern UnkStruct_0208922c data_0208922c;
extern UnkStruct_0236a444 data_ov000_0236a444;
extern UnkStruct_Pad data_022503e0;
extern u8 data_02250454[];

s32 func_02044690(void);
s32 func_02044670(void);
s32 func_02002094(s32 a, s32 b);
s32 func_ov000_022edb98(u16 a, s32 b);
void func_ov000_022edb50(UnkStruct_StageObj *o, s32 a);
void func_020446b0(s32 a);
void func_ov000_022ed9a0(UnkStruct_022ed190 *s, s32 x, s32 y, s32 z);
void func_02036754(s32 a, s32 b, s32 c);
void func_0201be98(void *p, s32 a);
void func_ov000_022ed15c(UnkStruct_022ed190 *s);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_ov000_022ed190(UnkStruct_022ed190 *s) {
    UnkStruct_Player *o;
    UnkStruct_Player *p;
    s32 m;
    s32 v;

    p = data_ov000_0236ac60;
    o = s->unk214;
    s->unk232 -= 0x40;
    if ((s16)s->unk232 <= 0)
        s->unk232 = 0;
    data_0208922c.unk24.b0 = (u16)((s->unk232 + 0xff) >> 8);
    if (s->unk394 < 0x3c) {
        s->unk394++;
        data_ov000_0236a444.unk3C = func_02044690() << 14;
        data_ov000_0236a444.unk40 = func_02044670() << 14;
    } else {
        m = func_02002094(s->unk390 << 12, 0x80);
        data_ov000_0236a444.unk3C = FX_Mul(func_02044690() * 0xccc, m);
        v = FX_Mul(func_02044670() * 0xccc, m);
        data_ov000_0236a444.unk40 = v;
        data_ov000_0236a444.unk44 = v;
    }
    s->unk390 = func_ov000_022edb98(data_022503e0.unk4, s->unk390);
    s->unk38C = func_ov000_022edb98(s->unk214->unk3DC, s->unk38C);
    if (data_022503e0.unk4 & 3) {
        if (s->unk390 > 0x60)
            func_020446b0(4);
        else if (s->unk390 > 0x40)
            func_020446b0(3);
        else if (s->unk390 > 0x20)
            func_020446b0(2);
        else
            func_020446b0(1);
        func_ov000_022ed9a0(s, (o->base.unk10 + p->base.unk10) >> 1, (o->base.unk14 + p->base.unk14) >> 1,
                            ((o->base.unk18 + p->base.unk18) >> 1) + 0x1000);
        func_02036754(0, 0, 0xcb);
        func_0201be98(data_02250454, 0x30);
    }
    func_ov000_022edb50(s->unk21C, s->unk390);
    func_ov000_022edb50(s->unk218, s->unk38C);
    s->unk384 += s->unk390 - s->unk38C;
    if (data_ov000_0236ac60->unk328 == 0x11 || data_ov000_0236ac60->unk328 == 0x12 ||
        (data_ov000_0236ac60->unk32C & 0x400))
        s->unk21C->unk5C |= 0x20;
    v = s->unk384;
    if (v < 0)
        v = -v;
    if (v >= 0x4000)
        s->base.unk68 = func_ov000_022ed15c;
}

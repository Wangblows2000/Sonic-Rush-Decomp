#include <nitro/types.h>

typedef struct UnkStruct_022fce7c_o {
    u8 pad0[0x10c];
    u8 unk10C;
    u8 unk10D;
} UnkStruct_022fce7c_o;

typedef struct UnkStruct_022fce7c_s {
    u8 pad0[4];
    s32 unk4;
} UnkStruct_022fce7c_s;

typedef struct UnkStruct_022fce7c_q {
    u8 pad0[0x40];
    u16 unk40;
    u8 pad42[0x1dc - 0x42];
    UnkStruct_022fce7c_s *unk1DC;
} UnkStruct_022fce7c_q;

typedef struct UnkStruct_022fce7c {
    u8 pad0[0x14];
    u32 unk14;
    u8 pad18[0x40 - 0x18];
    u16 unk40;
    u8 pad42[0x74 - 0x42];
    void (*unk74)(void);
    u8 pad78[0x7c - 0x78];
    UnkStruct_022fce7c_q *unk7C;
    u8 pad80[4];
    UnkStruct_022fce7c_o *unk84;
    u8 pad88[0x1cc - 0x88];
    s32 unk1CC;
} UnkStruct_022fce7c;

typedef struct UnkStruct_022fce7c_g {
    u8 pad0[0x3c];
    s32 unk3C;
} UnkStruct_022fce7c_g;

UnkStruct_022fce7c_g *ExtraBoss_GetWork(void);
void ExtraBoss_SetObjModelAnims(UnkStruct_022fce7c *p, s32 a);
void ExtraBoss_CopyParentPosition(UnkStruct_022fce7c *p);
void ExtraBoss_SwapObjModel(UnkStruct_022fce7c *p, s32 a);
void ExtraBossObj_State_PlayerFxHidden(void);
void ExtraBossObj_State_PlayerDashFxEnd(void);
void ExtraBossObj_State_PlayerFlyFx(void);

void ExtraBossObj_State_PlayerDashFx(UnkStruct_022fce7c *p) {
    UnkStruct_022fce7c_o *o;
    UnkStruct_022fce7c_q *q;
    UnkStruct_022fce7c_s *s;

    q = p->unk7C;
    o = p->unk84;
    s = q->unk1DC;

    if (ExtraBoss_GetWork()->unk3C == 0 && p->unk1CC == 0 && (o->unk10C & 2)) {
        p->unk1CC = 1;
        ExtraBoss_SetObjModelAnims(p, 1);
        o->unk10C |= 4;
        o->unk10D |= 4;
    }
    if (s->unk4 != 3 && s->unk4 != 4) {
        p->unk40 = 0;
        switch (s->unk4) {
        case 5:
        case 6:
        case 0xc:
            p->unk74 = ExtraBossObj_State_PlayerFxHidden;
            p->unk14 |= 0x30000;
            return;
        }
        if (ExtraBoss_GetWork()->unk3C == 0) {
            p->unk74 = ExtraBossObj_State_PlayerDashFxEnd;
            ExtraBoss_SetObjModelAnims(p, 2);
            o->unk10C &= ~4;
            o->unk10D &= ~4;
            return;
        }
        p->unk74 = ExtraBossObj_State_PlayerFlyFx;
        ExtraBoss_SwapObjModel(p, 0x19);
        p->unk84->unk10C |= 4;
        p->unk84->unk10D |= 4;
        return;
    }
    ExtraBoss_CopyParentPosition(p);
    if (s->unk4 == 3)
        p->unk40 = q->unk40;
    else
        p->unk40 = q->unk40 - 0x8000;
}

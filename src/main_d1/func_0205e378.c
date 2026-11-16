#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    u32 ofs;
} UnkRes_0205e378;

typedef struct {
    u8 pad0[0x12];
    u16 pal;
} UnkAnim_0205e378;

typedef struct {
    u32 unk0;
    u32 pad4;
    u16 anim;
    u8 padA[0x22];
    u32 flags;
    u8 pad30[0x14];
    u16 prio;
    u16 unk46;
} UnkSpr_0205e378;

typedef struct {
    u8 pad0[0x1c];
    u8 zone;
    u8 act;
} UnkState_0205e378;

typedef struct {
    u8 pad0[0x47c];
    s32 idx;
    u8 pad480[0x10];
    UnkRes_0205e378 *resA;
    UnkRes_0205e378 *resB;
} UnkWork_0205e378;

extern UnkState_0205e378 data_022c4560;
extern u16 data_02076b50[];
extern u16 data_02076b4c[];

void func_0203363c(UnkSpr_0205e378 *spr, UnkRes_0205e378 *res, u16 anim, u32 flags, s32 d, s32 e, u32 pal, s32 g, u32 h);
void func_0203344c(UnkSpr_0205e378 *spr, s32 a, s32 b);
void func_020331f8(UnkSpr_0205e378 *spr);
u16 func_0206d2d0(UnkRes_0205e378 *res, u16 anim, s32 n);
u32 func_02039c18(s32 a, u32 b);
u32 func_0204e6e8(u8 zone);

static inline u16 AnimPal_0205e378(UnkRes_0205e378 *res, u16 anim) {
    return ((UnkAnim_0205e378 *)((u8 *)res + res->ofs + 0xc))[anim].pal;
}

#define SPR(o) ((UnkSpr_0205e378 *)((u8 *)w + (o)))

void func_0205e378(UnkWork_0205e378 *w) {
    u16 prioA;
    u16 prioB;
    UnkSpr_0205e378 *spr;
    UnkSpr_0205e378 *spr2;
    u32 anim;
    u32 v;
    u16 anim16;
    u16 a2;
    u16 pal;
    UnkSpr_0205e378 *tmpp;
    u32 i;
    UnkSpr_0205e378 tmp;

    if (data_022c4560.zone == 8 && data_022c4560.act == 2) {
        prioA = 0xe;
        prioB = 0xf;
        func_0203363c(&tmp, w->resA, data_02076b50[w->idx], 0x48, 0, 0, 0, 0, 0x05000200);
        tmp.prio = prioA;
        func_0203344c(&tmp, 0, 0);
        tmp.unk0 = 1;
        func_020331f8(&tmp);
        func_0203363c(&tmp, w->resA, 0x19, 0x48, 0, 0, 0, 0, 0x05000200);
        tmp.prio = prioB;
        func_0203344c(&tmp, 0, 0);
        tmp.unk0 = 1;
        func_020331f8(&tmp);
    } else {
        prioA = func_0206d2d0(w->resA, data_02076b50[w->idx], 199);
        prioB = func_0206d2d0(w->resA, 0x19, 200);
    }

    spr = SPR(0x8c);
    anim = data_02076b50[w->idx];
    func_0203363c(spr, w->resA, anim, 0x4000, 1, 0, func_02039c18(1, AnimPal_0205e378(w->resA, anim)), 0, 0x05000600);
    spr->prio = prioA;
    func_0203344c(spr, 0, 0);

    anim = data_02076b4c[w->idx];
    spr = SPR(0x24);
    func_0203363c(spr, w->resA, anim, 0x44010, 1, 0, func_02039c18(1, AnimPal_0205e378(w->resA, anim)), 0, 0x05000600);
    spr->prio = prioA;
    spr->unk46 = 1;
    func_0203344c(spr, 0, 0);

    spr = SPR(0xe4);
    anim = data_022c4560.zone + 0xf;
    func_0203363c(spr, w->resA, anim, 0x44210, 1, 0, func_02039c18(1, AnimPal_0205e378(w->resA, anim)), 0, 0x05000600);
    spr->prio = prioA;
    spr->unk46 = 1;
    func_0203344c(spr, 0, 0);

    spr = SPR(0x12c);
    func_0203363c(spr, w->resA, 0x29, 0x44014, 1, 0, func_02039c18(1, AnimPal_0205e378(w->resA, 0x29)), 0, 0x05000600);
    spr->prio = prioA;
    spr->unk46 = 1;

    spr = SPR(0x184);
    anim = data_022c4560.zone * 2;
    func_0203363c(spr, w->resB, anim, 0x44210, 1, 0, func_02039c18(1, AnimPal_0205e378(w->resB, anim)), 0, 0x05000600);
    spr->prio = prioA;
    func_0203344c(spr, 0, 0);

    spr = SPR(0x1f4);
    anim = data_022c4560.zone * 2 + 1;
    pal = AnimPal_0205e378(w->resB, anim);
    if (pal == 0)
        pal = 1;
    func_0203363c(spr, w->resB, anim, 0x44210, 1, 0, func_02039c18(1, pal), 0, 0x05000600);
    spr->prio = prioA;
    func_0203344c(spr, 0, 0);

    v = func_0204e6e8(data_022c4560.zone);
    anim16 = v < 7 ? 2 : 0x2a;
    spr = v < 7 ? SPR(0x264) : SPR(0x2ac);
    func_0203363c(spr, w->resA, anim16, 0x44210, 1, 0, func_02039c18(1, AnimPal_0205e378(w->resA, anim16)), 0, 0x05000600);
    spr->prio = prioA;
    func_0203344c(spr, 0, 0);

    spr2 = v < 7 ? SPR(0x2ac) : SPR(0x264);
    if (v == 7)
        spr2->anim = 0xd;
    else if (v == 8)
        spr2->anim = 0xe;
    else
        spr2->anim = v + 4;
    func_0203363c(spr2, w->resA, spr2->anim, 0x44210, 1, 0, func_02039c18(1, AnimPal_0205e378(w->resA, spr2->anim)), 0, 0x05000600);
    spr2->prio = prioA;
    func_0203344c(spr2, 0, 0);

    spr = SPR(0x328);
    if (data_022c4560.zone >= 9 || data_022c4560.act >= 2)
        spr->anim = 0xb;
    else
        spr->anim = 3;
    func_0203363c(spr, w->resA, spr->anim, 0x44210, 1, 0, func_02039c18(1, AnimPal_0205e378(w->resA, spr->anim)), 0, 0x05000600);
    spr->prio = prioA;
    func_0203344c(spr, 0, 0);

    spr = SPR(0x370);
    if (data_022c4560.zone >= 9 || data_022c4560.act >= 2)
        spr->anim = 0xc;
    else
        spr->anim = data_022c4560.act + 4;
    func_0203363c(spr, w->resA, spr->anim, 0x44210, 1, 0, func_02039c18(1, AnimPal_0205e378(w->resA, spr->anim)), 0, 0x05000600);
    spr->prio = prioA;
    func_0203344c(spr, 0, 0);

    tmpp = SPR(0x4dc);
    for (i = 0; i < 14; i++) {
        a2 = i + 0x19;
        func_0203363c(tmpp, w->resA, a2, 0x24010, 0, 0, func_02039c18(0, AnimPal_0205e378(w->resA, a2)), 0, 0x05000200);
        tmpp->prio = prioB;
        func_0203344c(tmpp, 0, 0);
        tmpp++;
    }

    spr = SPR(0x424);
    func_0203363c(spr, w->resA, 0x18, 0x24200, 0, 0, func_02039c18(0, AnimPal_0205e378(w->resA, 0x18)), 0, 0x05000200);
    spr->prio = prioB;
    spr->flags &= ~0x60000;
    func_0203344c(spr, 0, 0);
}

#include <nitro/types.h>

typedef struct UnkSpr_0230b848 {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x46 - 0x30];
    u16 unk46;
} UnkSpr_0230b848;

typedef struct UnkObj_0230b848 {
    u8 pad[0x40];
} UnkObj_0230b848;

typedef struct UnkWork_0230b848 {
    u8 pad0[0xc];
    void *task0;
    void *task1;
    void *task2;
    u8 pad18[0x38 - 0x18];
    UnkObj_0230b848 obj[3];
    UnkSpr_0230b848 spr[3];
} UnkWork_0230b848;

typedef struct UnkAnimEntry_0230b848 {
    u8 pad0[0x10];
    u16 unk10;
    u8 pad12[2];
} UnkAnimEntry_0230b848;

typedef struct UnkAnimHdr_0230b848 {
    u8 pad0[0xc];
    UnkAnimEntry_0230b848 e[1];
} UnkAnimHdr_0230b848;

typedef struct UnkAnimRes_0230b848 {
    u8 pad0[0x18];
    u32 hdrOfs;
} UnkAnimRes_0230b848;

typedef struct UnkObjDef_0230b848 {
    u32 a;
    u8 b;
    u8 pad5;
    s16 res;
} UnkObjDef_0230b848;

typedef struct UnkSprDef_0230b848 {
    volatile u32 m;
    u16 idx;
    u8 hide;
    u8 flag;
    s16 res;
    u8 padA[2];
} UnkSprDef_0230b848;

#define ANIM_HDR(r) ((UnkAnimHdr_0230b848 *)((u8 *)(r) + (r)->hdrOfs))

extern UnkWork_0230b848 *data_ov001_0231f5c0;
typedef struct UnkResSet_0230b848 {
    u16 *res[7];
    UnkAnimRes_0230b848 *anim[1];
} UnkResSet_0230b848;

extern UnkResSet_0230b848 *data_ov001_0231f5c4;
extern UnkObjDef_0230b848 data_ov001_023171b0[];
extern UnkSprDef_0230b848 data_ov001_023171c8[];

void *func_020372d0(void (*a)(void), void (*b)(void), s32 c, u32 d, s32 e, s32 f);
void func_ov001_0230b658(void);
void func_ov001_0230b594(void);
void func_ov001_0230b554(void);
void func_ov001_0230b504(void);
void func_0203473c(void *buf, void *res, s32 a, u32 b, u8 c, u16 d, u16 e);
u32 func_02039c18(u32 a, u16 b);
void func_0203363c(UnkSpr_0230b848 *s, void *res, u16 idx, s32 a, u32 b, s32 c, u32 d, s32 e, u32 pal);
void func_0203344c(UnkSpr_0230b848 *s, s32 a, s32 b);
void func_0203438c(void *p);

static inline u16 GetPltt(UnkAnimRes_0230b848 *a, u16 idx) {
    return ANIM_HDR(a)->e[idx].unk10;
}

void func_ov001_0230b848(void) {
    UnkWork_0230b848 *w = data_ov001_0231f5c0;
    UnkResSet_0230b848 *rs = data_ov001_0231f5c4;
    s32 i;
    u32 pal;

    w->task0 = func_020372d0(func_ov001_0230b658, NULL, 0, 0x5000, 0, 0);
    w->task1 = func_020372d0(func_ov001_0230b594, NULL, 0, 0x5000, 0, 0);
    w->task2 = func_020372d0(func_ov001_0230b554, func_ov001_0230b504, 0, 0xf100, 0, 0);
    for (i = 0; i < 3; i++) {
        u16 *r = rs->res[data_ov001_023171b0[i].res];
        func_0203473c(&w->obj[i], r, 0x38, data_ov001_023171b0[i].a, data_ov001_023171b0[i].b, r[0x18 / 2], r[0x1a / 2]);
    }
    for (i = 0; i < 3; i++) {
        pal = data_ov001_023171c8[i].m == 0 ? 0x05000200 : 0x05000600;
        func_0203363c(&w->spr[i], rs->anim[data_ov001_023171c8[i].res], data_ov001_023171c8[i].idx, 0x40, data_ov001_023171c8[i].m, 0,
                      func_02039c18(data_ov001_023171c8[i].m, GetPltt(rs->anim[data_ov001_023171c8[i].res], data_ov001_023171c8[i].idx)), 0, pal);
        w->spr[i].flags |= 0x20000;
        w->spr[i].flags &= ~0x40;
    }
    for (i = 0; i < 3; i++) {
        func_0203344c(&w->spr[i], 0, 0);
        w->spr[i].flags |= 0x10;
        if (data_ov001_023171c8[i].hide == 0)
            w->spr[i].flags |= 8;
        if (data_ov001_023171c8[i].flag != 0)
            w->spr[i].flags |= 4;
        w->spr[i].unk46 = 0;
    }
    func_0203438c(&w->obj[0]);
    func_0203438c(&w->obj[1]);
    func_0203438c(&w->obj[2]);
}

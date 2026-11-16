#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    u32 ofs;
} UnkRes_02063f50;

typedef struct {
    u8 pad0[0x12];
    u16 pal;
} UnkAnim_02063f50;

typedef struct {
    u32 unk0;
    s16 unk4;
    s16 unk6;
    u8 pad8[0x24];
    u32 flags;
    u8 pad30[0x14];
    u16 prio;
    u16 unk46;
} UnkSpr_02063f50;

typedef struct {
    u8 pad0[0x10];
    void *work;
} UnkTask_02063f50;

typedef struct {
    u32 pad0;
    s32 unk4;
    UnkTask_02063f50 *task8;
    UnkTask_02063f50 *taskC;
    UnkSpr_02063f50 spr[24];
    UnkSpr_02063f50 sprX;
    s16 unk718;
    s16 unk71a;
    s32 unk71c;
    u16 unk720;
} UnkWork_02063f50;

typedef struct {
    UnkWork_02063f50 *owner;
    u32 pad4;
    s16 unk8;
    s16 unka;
} UnkSubA_02063f50;

typedef struct {
    UnkWork_02063f50 *owner;
    u32 pad4;
    u16 pad8;
    u16 unka;
    s32 unkc;
} UnkSubB_02063f50;

typedef struct {
    s32 unk0;
    u8 pad4[0xa];
    u16 unke;
    u8 pad10[0x18];
    u16 unk28;
} UnkState_02063f50;

typedef struct {
    u8 pad0[0x14];
    u8 unk14;
} UnkState2_02063f50;

extern UnkTask_02063f50 *data_022c4dc4;
extern UnkRes_02063f50 *gNaviMSprite;
extern UnkState_02063f50 data_022c4560;
extern UnkState2_02063f50 data_022c4590;
extern u16 data_02077910[];
extern u16 data_020778e0[];
extern u16 data_02077888[];
extern u16 data_0207788a[];
extern u16 data_0207788c[];
extern u16 data_022c4dbc;
extern u16 data_022c4db8;
extern u16 data_020778b0[];

void *Task_Create(void (*func)(void), void *dtor, u16 prio, u16 id, u16 unk1a, u32 workSize);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void AnimatorSprite_Init(UnkSpr_02063f50 *spr, UnkRes_02063f50 *res, u16 anim, u32 flags, s32 d, s32 e, u32 pal, s32 g, u32 h);
void AnimatorSprite_ProcessAnimation(UnkSpr_02063f50 *spr, s32 a, s32 b);
u32 ObjVram_Alloc(s32 a, u32 b);
s32 func_02002094(s32 n, s32 d);
s32 func_0200203c(s32 n, s32 d);
void func_02063778(UnkSpr_02063f50 *a, s16 dt);
void Oam_Upload(u32 engine);
void func_02063e10(void);
void func_02063ebc(void);
void func_02063df4(void);
void func_02063c98(void);

#define REG_DB_DISPCNT (*(vu32 *)0x04001000)

static inline u16 AnimPal2_02063f50(UnkRes_02063f50 *res, u16 anim) {
    return *(u16 *)((u8 *)res + res->ofs + anim * 0x14 + 0x1e);
}

static inline u16 AnimPal_02063f50(UnkRes_02063f50 *res, u16 anim) {
    return ((UnkAnim_02063f50 *)((u8 *)res + res->ofs + 0xc))[anim].pal;
}

static inline void MI_CpuClear16_(void *p, u32 size) {
    vu16 z = 0;
    MIi_CpuClear16(z, p, size);
}

void func_02063f50(void) {
    UnkSubA_02063f50 *subA;
    UnkSubB_02063f50 *subB;
    u16 i;
    UnkWork_02063f50 *w;
    UnkSpr_02063f50 *spr;
    u16 sel;
    u16 anim;
    u32 a;
    u16 j;
    UnkSpr_02063f50 *spr2;
    s16 k;
    u16 v;
    u32 plane;
    UnkSpr_02063f50 tmp;

    plane = (REG_DB_DISPCNT & 0x1f00) >> 8;
    REG_DB_DISPCNT = (REG_DB_DISPCNT & ~0x1f00) | ((plane & ~0x10) << 8);
    data_022c4dc4 = Task_Create(func_02063e10, func_02063ebc, 0, 0x3000, 0, 0x724);
    w = data_022c4dc4->work;
    MI_CpuClear16_(w, 0x724);
    w->unk4 = data_022c4560.unk0;
    w->unk718 = func_02002094(data_022c4590.unk14, 10);
    if (w->unk718 == 0)
        w->unk718 = -1;
    w->unk71a = func_0200203c(data_022c4590.unk14, 10);

    spr = w->spr;
    for (i = 0; i <= 12; i++, spr++) {
        AnimatorSprite_Init(spr, gNaviMSprite, i, 0x4030, 1, 0, ObjVram_Alloc(1, AnimPal_02063f50(gNaviMSprite, i)), 0, 0x05000600);
        spr->unk4 = spr->unk6 = 0;
        spr->prio = data_02077910[i];
        spr->flags |= 0x40000;
        AnimatorSprite_ProcessAnimation(spr, 0, 0);
    }

    w->unk720 = data_022c4560.unke;
    if (w->unk4 == 0)
        sel = 0;
    else
        sel = 0xb;
    anim = data_020778e0[w->unk720];
    if (anim == 0xffff)
        anim = 0x23;
    else
        w->unk71c = 1;
    spr = &w->sprX;
    AnimatorSprite_Init(spr, gNaviMSprite, anim, 0x4060, 1, 0, ObjVram_Alloc(1, AnimPal_02063f50(gNaviMSprite, anim)), 0, 0x05000600);
    spr->unk4 = spr->unk6 = 0;
    spr->prio = data_02077910[24];
    spr->flags |= 0x40000;
    AnimatorSprite_ProcessAnimation(spr, 0, 0);

    spr2 = &w->spr[13];
    for (j = 13; j <= 23; j++, spr2++) {
        a = j + sel;
        AnimatorSprite_Init(spr2, gNaviMSprite, a, 0x4030, 1, 0, ObjVram_Alloc(1, AnimPal2_02063f50(gNaviMSprite, a)), 0, 0x05000600);
        spr2->unk4 = spr2->unk6 = 0;
        spr2->prio = data_02077910[j];
        spr2->flags |= 0x40000;
        AnimatorSprite_ProcessAnimation(spr2, 0, 0);
    }

    AnimatorSprite_Init(&tmp, gNaviMSprite, data_02077888[w->unk4 * 3], 0x4048, 1, 0, 0, 0, 0x05000600);
    tmp.prio = 5;
    AnimatorSprite_ProcessAnimation(&tmp, 0, 0);
    tmp.prio = 6;
    AnimatorSprite_ProcessAnimation(&tmp, 0, 0);
    tmp.prio = 7;
    AnimatorSprite_ProcessAnimation(&tmp, 0, 0);
    AnimatorSprite_Init(&tmp, gNaviMSprite, data_0207788a[w->unk4 * 3], 0x4048, 1, 0, 0, 0, 0x05000600);
    tmp.prio = 4;
    AnimatorSprite_ProcessAnimation(&tmp, 0, 0);
    if (data_022c4dbc != 0) {
        AnimatorSprite_Init(&tmp, gNaviMSprite, data_0207788c[w->unk4 * 3], 0x4048, 1, 0, 0, 0, 0x05000600);
        tmp.prio = 8;
        for (k = 0; k < 7; k++) {
            if (data_022c4dbc & (1 << k))
                break;
        }
        if (k != 0)
            func_02063778(&tmp, k * 16 - 1);
        AnimatorSprite_ProcessAnimation(&tmp, 0, 0);
    }

    w->task8 = Task_Create(func_02063df4, 0, 0, 0x4000, 0, 0x18);
    subA = w->task8->work;
    MI_CpuClear16_(subA, 0x18);
    subA->owner = w;
    if (data_022c4db8 != 0)
        v = data_020778b0[w->unk720 - 1];
    else
        v = data_020778b0[w->unk720];
    subA->unk8 = func_02002094(v, 10);
    if (subA->unk8 == 0)
        subA->unk8 = -1;
    subA->unka = func_0200203c(v, 10);

    w->taskC = Task_Create(func_02063c98, 0, 0, 0x4000, 0, 0x14);
    subB = w->taskC->work;
    MI_CpuClear16_(subB, 0x14);
    subB->owner = w;
    subB->unkc = -1;
    subB->unka = data_022c4560.unk28;

    Oam_Upload(1);
    plane = (REG_DB_DISPCNT & 0x1f00) >> 8;
    REG_DB_DISPCNT = (REG_DB_DISPCNT & ~0x1f00) | ((plane | 0x10) << 8);
}

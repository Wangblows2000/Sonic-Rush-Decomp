#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    u32 ofs;
} UnkRes_02044d44;

typedef struct {
    u8 pad0[0x12];
    u16 pal;
} UnkAnim_02044d44;

typedef struct {
    u32 unk0;
    s16 unk4;
    s16 unk6;
    u8 pad8[0x24];
    u32 flags;
    u8 pad30[0x14];
    u16 prio;
    u16 unk46;
} UnkSpr_02044d44;

typedef struct {
    u8 pad0[0x10];
    void *work;
} UnkTask_02044d44;

typedef struct {
    u32 pad0;
    UnkTask_02044d44 *task4;
    UnkTask_02044d44 *task8;
    u32 padc;
    UnkSpr_02044d44 spr[17];
} UnkWork_02044d44;

typedef struct {
    u32 pad0;
    UnkWork_02044d44 *owner;
    u16 unk8;
} UnkSub_02044d44;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad18[4];
    u8 zone;
} UnkState_02044d44;

typedef struct {
    u8 pad0[8];
    u16 unk8;
} UnkG_02044d44;

extern UnkTask_02044d44 *gNaviBTask;
extern UnkRes_02044d44 *gNaviBSprite;
extern UnkState_02044d44 gGameState;
extern UnkG_02044d44 gBossStatus;

void *Task_Create(void (*func)(void), void *dtor, u16 prio, u16 id, u16 unk1a, u32 workSize);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void AnimatorSprite_Init(UnkSpr_02044d44 *spr, UnkRes_02044d44 *res, u16 anim, u32 flags, s32 d, s32 e, u32 pal, s32 g, u32 h);
void AnimatorSprite_ProcessAnimation(UnkSpr_02044d44 *spr, s32 a, s32 b);
u32 ObjVram_Alloc(s32 a, u32 b);
u32 func_0204e6e8(u8 zone);
void Oam_Upload(u32 engine);
void NaviB_Main(void);
void NaviB_Destructor(void);
void func_02044c10(void);
void func_02044ad0(void);

#define REG_DB_DISPCNT (*(vu32 *)0x04001000)

static inline u16 AnimPal_02044d44(UnkRes_02044d44 *res, u16 anim) {
    return ((UnkAnim_02044d44 *)((u8 *)res + res->ofs + 0xc))[anim].pal;
}

static inline void MI_CpuClear16_(void *p, u32 size) {
    vu16 z = 0;
    MIi_CpuClear16(z, p, size);
}

void NaviB_Create(void) {
    UnkSub_02044d44 *sub;
    UnkRes_02044d44 *res;
    u16 i;
    UnkWork_02044d44 *w;
    UnkSpr_02044d44 *spr;
    u16 anim;
    BOOL b;
    s32 v;
    u32 r;
    u32 plane;
    UnkSpr_02044d44 tmp;

    plane = (REG_DB_DISPCNT & 0x1f00) >> 8;
    REG_DB_DISPCNT = (REG_DB_DISPCNT & ~0x1f00) | ((plane & ~0x10) << 8);
    gNaviBTask = Task_Create(NaviB_Main, NaviB_Destructor, 0, 0x3000, 2, 0x4d8);
    w = gNaviBTask->work;
    MI_CpuClear16_(w, 0x4d8);

    spr = w->spr;
    for (i = 0; i <= 14; i++, spr++) {
        AnimatorSprite_Init(spr, gNaviBSprite, i, 0x4030, 1, 0, ObjVram_Alloc(1, AnimPal_02044d44(gNaviBSprite, i)), 0, 0x05000600);
        spr->unk4 = spr->unk6 = 0;
        spr->prio = 6;
        spr->unk46 = 10;
        AnimatorSprite_ProcessAnimation(spr, 0, 0);
    }

    spr = &w->spr[16];
    b = TRUE;
    if (gGameState.unk10 != 2 && gGameState.unk10 != 3)
        b = FALSE;
    if (b)
        v = 1;
    else
        v = gGameState.unk14;
    anim = v == 1 ? 0x1a : 0x19;
    AnimatorSprite_Init(spr, gNaviBSprite, anim, 0x4030, 1, 0, ObjVram_Alloc(1, AnimPal_02044d44(gNaviBSprite, anim)), 0, 0x05000600);
    spr->unk4 = spr->unk6 = 0;
    spr->prio = 6;
    spr->unk46 = 10;
    AnimatorSprite_ProcessAnimation(spr, 0, 0);

    spr = &w->spr[15];
    res = gNaviBSprite;
    r = func_0204e6e8(gGameState.zone);
    AnimatorSprite_Init(spr, gNaviBSprite, r + 0xf, 0x4060, 1, 0, ObjVram_Alloc(1, AnimPal_02044d44(res, gGameState.zone + 0xf)), 0, 0x05000600);
    spr->unk4 = spr->unk6 = 0;
    spr->prio = 6;
    spr->unk46 = 10;
    AnimatorSprite_ProcessAnimation(spr, 0, 0);

    w->spr[0].flags &= ~0x40;
    w->spr[0].flags |= 0x10;

    AnimatorSprite_Init(&tmp, gNaviBSprite, 0x18, 0x4048, 1, 0, 0, 0, 0x05000600);
    tmp.prio = 7;
    AnimatorSprite_ProcessAnimation(&tmp, 0, 0);
    AnimatorSprite_Init(&tmp, gNaviBSprite, 0x17, 0x4048, 1, 0, 0, 0, 0x05000600);
    tmp.prio = 8;
    AnimatorSprite_ProcessAnimation(&tmp, 0, 0);

    w->task4 = Task_Create(func_02044c10, 0, 0, 0x4000, 2, 0xc);
    sub = w->task4->work;
    MI_CpuClear16_(sub, 0xc);
    sub->owner = w;
    sub->unk8 = gBossStatus.unk8;

    w->task8 = Task_Create(func_02044ad0, 0, 0, 0x4000, 2, 0x10);
    sub = w->task8->work;
    MI_CpuClear16_(sub, 0x10);
    sub->owner = w;
    sub->unk8 = gBossStatus.unk8;

    Oam_Upload(1);
    plane = (REG_DB_DISPCNT & 0x1f00) >> 8;
    REG_DB_DISPCNT = (REG_DB_DISPCNT & ~0x1f00) | ((plane | 0x10) << 8);
}

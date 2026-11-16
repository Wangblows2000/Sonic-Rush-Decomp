#include <nitro/types.h>

typedef struct {
    u16 v[4];
} UnkVals_02055918;

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[4];
    void *unk34;
    u8 pad38[0x44 - 0x38];
    u16 unk44;
    u8 pad46[2];
} UnkSpr_02055918;

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u8 pad6[0x10 - 6];
    u8 unk10[8];
    u8 unk18[8];
    UnkSpr_02055918 spr[4];
    s32 unk140;
    s32 unk144;
    s32 unk148;
    s32 unk14C;
    u8 pad150[0x164 - 0x150];
    s32 unk164;
    s32 unk168;
    s32 unk16C;
    s32 unk170;
    u8 pad174[0x17a - 0x174];
    u16 pal0[0x100];
    u16 pal1[0x100];
    u8 pad57A[0x77c - 0x57a];
} UnkWork_02055918;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02055918 *work;
} UnkTask_02055918;

typedef struct {
    u8 pad0[0x48];
    s16 unk48;
} UnkSys_02055918;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    u8 pad14[4];
    u32 unk18;
    u8 pad1C[4];
    s32 unk20;
} UnkGame_02055918;

typedef struct {
    u8 pad0[0x18];
    s32 unk18;
} UnkState_02055918;

typedef struct {
    u8 pad0[4];
    u16 unk4;
} UnkMisc_02055918;

extern UnkVals_02055918 data_0207ba1c;
extern UnkSys_02055918 gDisplayMain;
extern UnkGame_02055918 data_022c4560;
extern UnkSys_02055918 gDisplaySub;
extern UnkState_02055918 data_022c4590;
extern u32 gSystemFlags;
extern UnkMisc_02055918 gPad;
extern void *gActComArchive;
extern u8 data_0207ba34[];
extern u8 data_0207ba50[];
extern u8 data_0207ba68[];
extern u8 data_0207ba80[];
extern u8 gGameLanguage;

UnkTask_02055918 *Task_Create(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void PauseMenu_Main(void);
void PauseMenu_Destructor(void);
u32 func_02006abc(s32 i);
void MIi_CpuClear16(u16 v, void *dst, u32 size);
void MIi_CpuCopy16(const void *src, void *dst, u32 size);
void System_Freeze(s32 a, s32 b, s32 c);
BOOL func_0204eb7c(void);
u32 func_ov001_0230c0b8(void);
u32 func_ov001_0230c0cc(void);
void ObjAction_Load2dBAC(void *bgA, void *bgB, void *name, u32 pal, void *s, void *arc);
void AnimatorSprite_SetAnimation(void *s, u16 a);
void AnimatorSprite_ProcessAnimation(void *s, void *a, void *b);
void func_020551b0(UnkWork_02055918 *w);
void func_0201bf40(s32 a, s32 b);
void Sound_PlaySeqArcEx(s32 a, s32 b, s32 c);

static inline s32 ClampLang_02055918(s32 lang) {
    switch (lang) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        return lang;
    default:
        return 1;
    }
}

void PauseMenu_Create(BOOL alt) {
    UnkVals_02055918 vals = data_0207ba1c;
    UnkTask_02055918 *t;
    UnkWork_02055918 *w;
    vu16 zero;
    u16 i;
    u16 a;
    u16 b;
    u16 c;
    s32 lang;
    s32 k;

    if (gDisplayMain.unk48 != 0)
        return;
    if (data_022c4560.unk18 & 0x20)
        return;
    if (gDisplayMain.unk48 != 0)
        return;
    if (gDisplaySub.unk48 != 0)
        return;
    if (data_022c4590.unk18 == data_022c4560.unk20)
        return;
    t = Task_Create(PauseMenu_Main, PauseMenu_Destructor, 7, 0x6100, 3, sizeof(UnkWork_02055918));
    if (t == (UnkTask_02055918 *)func_02006abc(0))
        return;
    w = t->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkWork_02055918));
    if (!(gSystemFlags & 8))
        System_Freeze(1, 1, 1);
    gPad.unk4 = 0;
    if (!func_0204eb7c() || data_022c4560.unk10 == 2)
        w->unk4 |= 2;
    if (alt) {
        w->unk4 &= ~2;
        if (func_ov001_0230c0b8() & (1 << func_ov001_0230c0cc()))
            w->unk4 |= 2;
        w->unk4 |= 0x20;
        w->unk164 = 0x10;
        w->unk168 = 0x12;
        w->unk16C = 0x13;
        w->unk170 = 0x14;
        for (i = 0; i < 4; i++)
            vals.v[i] <<= 1;
    } else {
        w->unk164 = 0;
        w->unk168 = 0x37;
        w->unk16C = 0x38;
        w->unk170 = 0x39;
    }
    ObjAction_Load2dBAC(&w->spr[0], NULL, data_0207ba34, vals.v[0], w->unk10, gActComArchive);
    ObjAction_Load2dBAC(&w->spr[1], NULL, data_0207ba50, vals.v[1], w->unk18, gActComArchive);
    ObjAction_Load2dBAC(&w->spr[2], NULL, data_0207ba68, vals.v[2], w->unk18, gActComArchive);
    ObjAction_Load2dBAC(&w->spr[3], NULL, data_0207ba80, vals.v[3], w->unk18, gActComArchive);
    b = 2;
    a = 3;
    if (data_022c4560.unk10 == 2 || data_022c4560.unk10 == 4)
        a++;
    lang = gGameLanguage;
    b += ClampLang_02055918(lang) * 3;
    a += ClampLang_02055918(lang) * 3;
    AnimatorSprite_SetAnimation(&w->spr[2], b);
    AnimatorSprite_SetAnimation(&w->spr[3], a);
    c = 0;
    if (ClampLang_02055918(lang) > 3)
        c++;
    AnimatorSprite_SetAnimation(&w->spr[1], c);
    for (i = 0; i < 4; i++) {
        w->spr[i].flags &= ~0x60000;
        if (alt) {
            w->spr[i].flags |= 0x20;
            w->spr[i].unk44 = 0xf;
        } else {
            w->spr[i].flags |= 0x30;
            w->spr[i].unk44 = 1;
        }
        AnimatorSprite_ProcessAnimation(&w->spr[i], NULL, NULL);
    }
    w->unk140 = 0x6000;
    w->unk144 = 0x6000;
    if (w->unk4 & 2) {
        w->unk148 = 0x6000;
        w->unk14C = 0x4000;
        w->unk2 = 1;
        MIi_CpuCopy16(w->spr[2].unk34, w->pal0, 0x200);
        MIi_CpuCopy16(w->spr[3].unk34, w->pal1, 0x200);
        func_020551b0(w);
    } else {
        w->unk148 = 0x6000;
        w->unk14C = 0x5800;
        w->unk2 = 0;
    }
    w->unk4 |= 0x18;
    for (k = 3; k < 10; k++)
        func_0201bf40(k, 1);
    Sound_PlaySeqArcEx(0, w->unk164, w->unk168);
}

#pragma opt_dead_assignments off
#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
} UnkAnim_022cdfdc;

typedef struct {
    s32 id;
    UnkAnim_022cdfdc anim;
} UnkItem_022cdfdc;

typedef struct {
    u8 pad0[4];
    s32 unk4;
    u8 pad8[4];
    s32 count;
    s32 unk10;
    u8 pad14[8];
    s32 unk1c;
    u8 pad20[4];
    s32 unk24;
    u8 pad28[4];
    s32 unk2c;
    s32 unk30;
    s32 unk34;
    u8 pad38[0x1b8 - 0x38];
    s32 unk1b8;
    u8 pad1bc[0x208 - 0x1bc];
    UnkAnim_022cdfdc animA;
    UnkAnim_022cdfdc animB;
    UnkAnim_022cdfdc animC;
    UnkItem_022cdfdc items[6];
    void *parent;
} UnkWork_022cdfdc;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022cdfdc *work;
} UnkTask_022cdfdc;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x30 - 0x18];
    s32 unk30;
    u8 pad34[4];
    s32 unk38;
    u8 pad3c[4];
    s32 unk40;
    u8 pad44[0x44 - 0x44];
} UnkSub_022cdfdc;

typedef struct {
    s32 unk0;
    u8 pad4[0x18 - 4];
    void *ptrs[6];
    UnkSub_022cdfdc sub;
    u8 pad74[0x1e04 - 0x74];
    s32 unk1e04;
    u8 pad1e08[0x1e2c - 0x1e08];
    void *res;
} UnkStruct_022cdfdc;

UnkTask_022cdfdc *Task_Create(void (*fn)(void), void (*dtor)(void), s32 b, s32 c, s32 d, s32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void OptionsMenu_Main(void);
void Options_CreateFadeTask(s32 a, s32 b, s32 c, s32 d);
void KeyframeAnim_Init(UnkAnim_022cdfdc *a, void *res, s32 id);
void KeyframeAnim_SetToLastFrame(UnkAnim_022cdfdc *a);
void OptionsMenu_OpenPage0(UnkWork_022cdfdc *w);

static inline void MI_CpuClear16_022cdfdc(void *dest, u32 size) {
    vu16 zero = 0;
    MIi_CpuClear16(zero, dest, size);
}

#pragma thumb on
void OptionsMenu_Create(UnkStruct_022cdfdc *p, s32 a, s32 b) {
    UnkWork_022cdfdc *w;
    s32 n;
    s32 i;
    void *res;
    UnkSub_022cdfdc *sub;
    s32 k;

    w = Task_Create(OptionsMenu_Main, NULL, 0, 0x2000, 0x1000, sizeof(UnkWork_022cdfdc))->work;
    MI_CpuClear16_022cdfdc(w, sizeof(UnkWork_022cdfdc));
    w->parent = p;
    n = 0;
    w->unk4 = -1;
    w->unk1c = -1;
    w->unk30 = -1;
    w->unk34 = -1;
    w->unk24 = 0;
    w->unk1b8 = p->unk1e04;
    w->unk10 = 1;
    for (i = 0; i < 6; i++) {
        if (p->ptrs[i] != NULL) {
            w->items[n].id = i;
            if (w->items[n].id == 0) {
                w->unk4 = w->items[n].id;
            }
            n++;
        }
    }
    w->count = n;
    for (; n < 6; n++) {
        w->items[n].id = 7;
    }
    Options_CreateFadeTask(-0x10, 0, 0x10, 0);
    w->unk2c = a;
    w->unk30 = b;
    res = p->res;
    sub = &p->sub;
    KeyframeAnim_Init(&w->animB, res, sub->unk38);
    KeyframeAnim_SetToLastFrame(&w->animB);
    KeyframeAnim_Init(&w->animA, res, sub->unk30);
    KeyframeAnim_SetToLastFrame(&w->animA);
    KeyframeAnim_Init(&w->animC, res, sub->unk40);
    KeyframeAnim_SetToLastFrame(&w->animC);
    for (i = 0, k = 0; i < 6; i++) {
        if (w->items[i].id != 7) {
            if (i == w->unk4) {
                KeyframeAnim_Init(&w->items[i].anim, res, sub->unk10);
            } else {
                KeyframeAnim_Init(&w->items[i].anim, res, sub->unk14);
            }
            KeyframeAnim_SetToLastFrame(&w->items[i].anim);
            k++;
        }
    }
    OptionsMenu_OpenPage0(w);
    p->unk0 = 9;
}
#pragma thumb reset

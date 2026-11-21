#include <nitro/types.h>

typedef struct {
    u8 pad[4];
    u16 unk4;
} UnkAnim_02046938;

typedef struct {
    s32 unk0;
    u8 pad4[4];
    UnkAnim_02046938 *anim;
} UnkSprite_02046938;

typedef struct {
    u8 pad0[0xe4];
    UnkSprite_02046938 *sprites[5];
    u8 padf8[0x1c];
    s32 unk114;
} UnkInner_02046938;

typedef struct {
    void (*func)(void *);
    UnkInner_02046938 inner;
    u8 pad11c[0x20];
    s32 unk13c;
    s32 unk140;
    s32 unk144;
    s32 unk148;
    s32 unk14c;
    s32 unk150;
    s16 unk154;
} UnkSub_02046938;

typedef struct {
    u8 pad0[0x10];
    UnkSub_02046938 sub;
    u8 pad[0x344 - 0x10 - sizeof(UnkSub_02046938)];
    u32 flags;
} UnkObj_02046938;

typedef struct {
    s32 unk0;
    u8 pad4[0x18];
    u8 unk1c;
    u8 unk1d;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 gGameState;
extern s32 data_02073478[];
extern s32 data_0207347c[];
extern s32 data_02073480[];
extern s32 data_02073498[];
extern s32 data_0207349c[];
extern s32 data_020734a0[];
extern s32 data_020734a4[];
extern s32 data_020734a8[];
extern s32 data_020734ac[];
extern s32 data_020734b0[];
void ClearDemoModel_State_Done(void *);

#pragma thumb on
void ClearDemoModel_Skip(UnkObj_02046938 *o) {
    s32 idx;
    s32 i;
    UnkSub_02046938 *s;
    UnkInner_02046938 *in;
    s = &o->sub;
    idx = gGameState.unk0;
    if (idx > 1)
        idx = 0;
    o->flags |= 0xa2;
    idx *= 20;
    s->unk13c = data_02073478[idx];
    s->unk140 = data_0207347c[idx];
    s->unk144 = data_02073480[idx];
    if (gGameState.unk1c >= 9 || gGameState.unk1d >= 2) {
        s->unk148 = data_020734a8[idx];
        s->unk14c = data_020734ac[idx];
        s->unk150 = data_020734b0[idx];
    } else {
        s->unk148 = data_02073498[idx];
        s->unk14c = data_0207349c[idx];
        s->unk150 = data_020734a0[idx];
    }
    s->unk154 = data_020734a4[idx];
    in = &s->inner;
    for (i = 0; i < 5; i++) {
        UnkSprite_02046938 *spr = in->sprites[i];
        if (spr != NULL)
            spr->unk0 = spr->anim->unk4 << 12;
    }
    in->unk114 = 0x1000;
    s->func = ClearDemoModel_State_Done;
}
#pragma thumb reset

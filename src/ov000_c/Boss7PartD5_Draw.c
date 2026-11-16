#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x24];
    u8 unk24[0x48 - 0x24];
    UnkStruct_Vec unk48;
    u8 pad54[0x10C - 0x54];
    u8 unk10C;
    u8 pad10D[3];
    u8 unk110;
    u8 pad111[0x138 - 0x111];
} UnkStruct_022eb9f8_Sub;

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_022eb9f8_Sub unk20C;
    u8 pad344[0x3D0 - 0x344];
    s32 unk3D0;
    u8 pad3D4[0x48C - 0x3D4];
    UnkStruct_Vec unk48C[4];
    u16 unk4BC;
} UnkStruct_022eb9f8;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_022eb9f8 *work;
} UnkStruct_022eb9f8_Task;

typedef struct {
    u8 pad0[0x20C];
    u32 unk20C;
    u8 pad210[0x220 - 0x210];
    u32 unk220;
} UnkStruct_02369b78;

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;

extern UnkStruct_022eb9f8_Task *gTaskCurrent;
extern UnkStruct_02369b78 *gBoss7;
extern UnkStruct_022c4560 gGameState;
extern u16 data_02074960[][3][3];
extern u16 data_02074962[][3][3];
extern u16 data_02074964[][3][3];
extern s16 data_02071470[];
extern s16 data_0206f470[];

void NNS_G3dGlbLightColor(s32 a, u32 b);
void Model_Draw(void *s);
void Model_Animate(void *s);
void MTX_RotY33_(void *m, s32 s, s32 c);
void AnimatorSprite_ProcessAnimation(UnkStruct_Spr *s, void (*fn)(void), void *w);
void Boss7PartD5_SpriteCallback(void);

void Boss7PartD5_Draw(void) {
    UnkStruct_022eb9f8 *w = gTaskCurrent->work;
    UnkStruct_02369b78 *mgr = gBoss7;
    UnkStruct_022eb9f8_Sub *sub = &w->unk20C;
    u16 i;

    if (w->base.unk5C & 0x20)
        return;
    w->unk4BC++;
    w->unk4BC &= 3;
    w->unk48C[w->unk4BC] = sub->unk48;
    if (mgr->unk20C & 0x10) {
        i = (w->unk4BC + (mgr->unk220 & 1)) & 3;
        if (w->unk3D0 == 0) {
            NNS_G3dGlbLightColor(0, 0x6182);
            NNS_G3dGlbLightColor(1, 0x6182);
            NNS_G3dGlbLightColor(2, 0x6182);
        } else {
            NNS_G3dGlbLightColor(0, 0x1a98);
            NNS_G3dGlbLightColor(1, 0x1a98);
            NNS_G3dGlbLightColor(2, 0x1a98);
        }
        sub->unk48 = w->unk48C[i];
        Model_Draw(sub);
        NNS_G3dGlbLightColor(0, data_02074960[gGameState.unk1C][gGameState.unk1D][0]);
        NNS_G3dGlbLightColor(1, data_02074962[gGameState.unk1C][gGameState.unk1D][0]);
        NNS_G3dGlbLightColor(2, data_02074964[gGameState.unk1C][gGameState.unk1D][0]);
    }
    w->base.unk5C &= ~8;
    if (w->base.unk5C & 4) {
        sub->unk10C |= 4;
        sub->unk110 |= 4;
    } else {
        sub->unk10C &= ~4;
        sub->unk110 &= ~4;
    }
    if (w->base.unk5C & 1)
        MTX_RotY33_(sub->unk24, data_02071470[0], data_02071470[1]);
    else
        MTX_RotY33_(sub->unk24, data_0206f470[0], data_0206f470[1]);
    sub->unk48.x = w->base.unk10 << 4;
    sub->unk48.y = -(w->base.unk14 << 4);
    sub->unk48.z = w->base.unk18 << 4;
    if (!(w->base.unk5C & 0x10)) {
        AnimatorSprite_ProcessAnimation(&w->base.spr0, Boss7PartD5_SpriteCallback, w);
        Model_Animate(sub);
    }
    Model_Draw(sub);
    if (sub->unk10C & 2)
        w->base.unk5C |= 8;
    if (w->base.spr0.unk2C & 2)
        w->base.unk5C |= 8;
}

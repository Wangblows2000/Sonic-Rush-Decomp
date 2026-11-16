#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x2C];
} UnkStruct_02359e30_Obj;

typedef struct {
    u8 pad0[0x5C4];
    UnkStruct_Spr spr[19];
    s32 unkB1C[19];
    UnkStruct_02359e30_Obj unkB68[9];
    u8 padD1C_[0xD1C - 0xB68 - 9 * 0x2C];
    void *unkD1C;
    void *unkD20;
    void *unkD24;
    void *unkD28;
    void *unkD2C;
} UnkStruct_02359e30;

void func_ov000_02359d5c(UnkStruct_Spr *s, void *res, s32 a, s32 b, s32 c, s32 d);
void func_ov000_02359dc8(UnkStruct_Spr *s, void *res, s32 a, s32 b, s32 c, s32 d);
void AnimatorSprite_ProcessAnimation(UnkStruct_Spr *s, s32 a, s32 b);
void KeyframeAnim_Init(UnkStruct_02359e30_Obj *o, void *res, s32 a);

#pragma thumb on

void func_ov000_02359e30(UnkStruct_02359e30 *w) {
    void *res;
    s32 i;

    res = w->unkD1C;
    func_ov000_02359d5c(&w->spr[0], res, 1, 2, 0, 0);
    func_ov000_02359dc8(&w->spr[1], res, 3, 1, 0, 0);
    func_ov000_02359dc8(&w->spr[2], res, 3, 1, 0, 0);
    w->spr[2].unk2C |= 0x80;
    func_ov000_02359dc8(&w->spr[3], res, 6, 1, 0, 8);
    func_ov000_02359dc8(&w->spr[4], res, 7, 1, 0, 0x10);
    func_ov000_02359dc8(&w->spr[5], res, 8, 2, 0, 0xc);
    func_ov000_02359dc8(&w->spr[6], res, 0xb, 2, 0, 0xc);
    func_ov000_02359dc8(&w->spr[7], res, 0xe, 2, 0, 0xc);
    func_ov000_02359dc8(&w->spr[8], res, 9, 1, 0, 0xc);
    func_ov000_02359dc8(&w->spr[9], res, 0x11, 3, 0, 0);
    func_ov000_02359dc8(&w->spr[10], res, 0x12, 3, 0, 0);
    func_ov000_02359dc8(&w->spr[11], res, 0x13, 3, 0, 0);
    func_ov000_02359dc8(&w->spr[12], res, 0x14, 3, 0, 0);
    func_ov000_02359dc8(&w->spr[15], res, 0x1d, 1, 0, 3);
    AnimatorSprite_ProcessAnimation(&w->spr[15], 0, 0);
    func_ov000_02359dc8(&w->spr[16], res, 0x1e, 1, 0, 0);
    AnimatorSprite_ProcessAnimation(&w->spr[16], 0, 0);
    res = w->unkD20;
    func_ov000_02359d5c(&w->spr[13], res, 0, 0, 0, 0);
    func_ov000_02359dc8(&w->spr[14], res, 2, 1, 0, 4);
    func_ov000_02359dc8(&w->spr[17], w->unkD24, 0, 1, 0, 0);
    func_ov000_02359dc8(&w->spr[18], w->unkD28, 1, 0, 0xf, 0);
    w->spr[18].unk4 = 0xb0;
    w->spr[18].unk6 = 8;
    for (i = 0; i < 19; i++)
        w->unkB1C[i] = -1;
    for (i = 0; i < 9; i++)
        KeyframeAnim_Init(&w->unkB68[i], w->unkD2C, 0);
}

#pragma thumb reset

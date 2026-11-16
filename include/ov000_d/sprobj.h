#ifndef OV000_D_SPROBJ_H
#define OV000_D_SPROBJ_H

#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x2C - 8];
    u32 unk2C;
    u8 pad30[0x48 - 0x30];
} UnkStruct_SprObjSpr;

/* ov000 0x0235dxxx sprite menu object: 17 sprites + current anim per sprite */
typedef struct {
    u32 unk0;
    u32 unk4;
    s32 unk8;
    s32 unkC;
    u8 pad10[0x14 - 0x10];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    UnkStruct_SprObjSpr spr[17];
    u8 pad4EC[0x534 - 0x4EC];
    s32 anim[17];
    u8 pad578[0xC14 - 0x578];
    u8 unkC14[6];
} UnkStruct_SprObj;

typedef struct {
    u8 pad0[8];
    void (*unk8)(void);
    u8 padC[4];
    UnkStruct_SprObj *work;
} UnkStruct_SprObjTask;

extern UnkStruct_SprObjTask *gTaskCurrent;

void func_ov000_0235e124(UnkStruct_SprObj *w);
void func_ov000_0235e068(UnkStruct_SprObj *w);
void func_ov000_0235e018(UnkStruct_SprObj *w);
void func_ov000_0235dfc4(UnkStruct_SprObj *w);
void func_ov000_0235def0(UnkStruct_SprObj *w, s32 sel, s32 scale);
void func_ov000_0235ddec(UnkStruct_SprObj *w, BOOL flag);
void func_ov000_0235cfc4(UnkStruct_SprObj *w);
void func_ov000_0235d888(UnkStruct_SprObj *w, s32 sel);
void func_ov000_0235d7b4(UnkStruct_SprObj *w, s32 sel);

void AnimatorSprite_SetAnimation(UnkStruct_SprObjSpr *spr, u16 anim);
void AnimatorSprite_ProcessAnimation(UnkStruct_SprObjSpr *spr, s32 a, s32 b);
void AnimatorSprite_Draw(UnkStruct_SprObjSpr *spr);
void AnimatorSprite_DrawRotScale(UnkStruct_SprObjSpr *spr, s32 sx, s32 sy, s32 c);

#endif

#ifndef MAIN_B3_W50_H
#define MAIN_B3_W50_H
#include <nitro/types.h>

/* 0x48-byte sprite/animator (AnimatorSprite_ProcessAnimation, AnimatorSprite_Draw, AnimatorSprite_SetAnimation) */
typedef struct {
    u8 unk0[4];
    s16 x;   /* 0x4 */
    s16 y;   /* 0x6 */
    u8 pad8[0x24];
    u32 unk2C;
    u8 pad30[0x14];
    u16 unk44;
    u16 unk46;
} UnkSpr_W50;

/* work of task created by MsgBox_Create (0x588 bytes) */
typedef struct {
    u32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC[2];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad54[0x300];
    s32 unk354;
    s32 unk358;
    s32 unk35C;
    u8 pad360[0x40];
    UnkSpr_W50 spr[5]; /* 0x3a0 */
    s32 anim[5];       /* 0x508 */
    s32 unk51C[8];
    s32 unk53C;
    s32 unk540;
    s32 unk544;
    s32 unk548;
    s32 unk54C;
    u8 pad550[0x34];
    s32 unk584;
} UnkWork_W50;

typedef struct { u8 unk0[0x10]; UnkWork_W50 *work; } UnkTask_W50;

typedef struct { u16 unk0; u16 unk2; u16 trg; u16 unk6; } UnkPad_022503e0;
extern UnkPad_022503e0 gPad;
#endif

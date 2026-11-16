#ifndef OV000_D_SCENE_H
#define OV000_D_SCENE_H

#include <ov000_d/mdl.h>

typedef struct {
    s16 x;
    s16 y;
    s32 unk4;
    u8 pad8[0x2C - 8];
} UnkStruct_ScenePos;

/* task work of the ov000 0x02358xxx menu scene */
typedef struct {
    u32 unk0;
    u32 unk4;
    u32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    UnkStruct_Mdl mdl;
    s32 unk32C;
    s32 unk330;
    s32 unk334;
    s32 unk338;
    s32 unk33C;
    s32 unk340;
    u8 pad344[0x4C4 - 0x344];
    u8 unk4C4[0x504 - 0x4C4];
    u8 unk504[0xAD4 - 0x504];
    u8 unkAD4[0xB68 - 0xAD4];
    UnkStruct_ScenePos posB68;
    UnkStruct_ScenePos pos[8];
    void *res[15];
} UnkStruct_Scene;

typedef struct {
    u8 pad0[8];
    void (*unk8)(void);
    u8 padC[4];
    UnkStruct_Scene *work;
} UnkStruct_SceneTask;

typedef struct {
    u16 x;
    u16 y;
} UnkStruct_SceneBgOfs;

extern UnkStruct_SceneTask *gTaskCurrent;
extern UnkStruct_SceneBgOfs gDisplaySub[4];

void PlayerSelectMenu_AnimateModels(UnkStruct_Mdl *m);
void PlayerSelectMenu_DrawModels(UnkStruct_Mdl *m);
void func_ov000_02357ec8(UnkStruct_Scene *w, s32 sel, s32 scale);
void func_ov000_023582e8(UnkStruct_Scene *w, s32 sel, s32 scroll);
void func_ov000_023579fc(UnkStruct_Scene *w, s32 x, s32 y);
void func_ov000_023579c8(UnkStruct_Scene *w, s32 x, s32 y);
void func_ov000_0235793c(UnkStruct_Scene *w, s32 x, s32 y);
void func_ov000_02357908(UnkStruct_Scene *w, s32 x, s32 y);
void func_ov000_02357994(UnkStruct_Scene *w, s32 x, s32 y);
void func_ov000_023578d0(UnkStruct_Scene *w, s32 x, s32 y);
void func_ov000_02358384(UnkStruct_Scene *w, s32 i, s32 ang);
void func_ov000_02357878(UnkStruct_Scene *w, s32 x, s32 y, s32 scale);
void func_ov000_02358224(UnkStruct_Scene *w, s32 x, s32 scale);
void func_ov000_023580bc(UnkStruct_Scene *w, BOOL flag);
void func_ov000_02358070(UnkStruct_Scene *w, BOOL flag);
void func_ov000_02357970(UnkStruct_Scene *w);
void func_ov000_02357844(UnkStruct_Scene *w, s32 x, s32 y);
s32 PlayerSelectMenu_EaseValue(s32 from, s32 to, s32 total, s32 cur, s32 bias);
void func_ov000_02357090(UnkStruct_Mdl *m, s32 sel);
void PlayerSelectMenu_SetModelSelection(UnkStruct_Mdl *m, s32 sel);
void func_ov000_02356e7c(UnkStruct_Mdl *m, s32 a);
void KeyframeAnim_Update(UnkStruct_ScenePos *p);
void KeyframeAnim_SetToLastFrame(UnkStruct_ScenePos *p);
BOOL KeyframeAnim_IsFinished(UnkStruct_ScenePos *p);
void func_ov000_023581dc(UnkStruct_Scene *w);
void func_ov000_02357810(UnkStruct_Scene *w, s32 a, s32 b);
void PlayerSelectMenu_SetBgScale(UnkStruct_Scene *w, s32 scale);
void func_ov000_02357d20(UnkStruct_Scene *w);
void func_ov000_02357c8c(UnkStruct_Scene *w);
void func_ov000_02357aac(UnkStruct_Scene *w);
void func_ov000_02357ae8(UnkStruct_Scene *w);
void Task_DestroyRange(s32 a, s32 b);
void PlayerSelectMenu_StartBrightnessFade(s32 x, s32 y, s32 a, s32 b);
void MsgBox_Create(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
void func_ov000_02357b9c(UnkStruct_Scene *w);
void func_ov000_02357a30(UnkStruct_Scene *w, s32 sel);
void func_ov000_02357a6c(UnkStruct_Scene *w, s32 sel);
void func_ov000_02357c2c(UnkStruct_Scene *w);
void func_ov000_02357cd0(UnkStruct_Scene *w);
void func_ov000_02357b4c(UnkStruct_Scene *w);
void func_ov000_02357b24(UnkStruct_Scene *w);
void func_ov000_02357be4(UnkStruct_Scene *w);
void func_ov000_02357b74(UnkStruct_Scene *w);
void func_ov000_02357aa8(UnkStruct_Scene *w, s32 i);
void func_ov000_02358138(UnkStruct_Scene *w);
void func_ov000_02358180(UnkStruct_Scene *w);
void KeyframeAnim_SetAnim(UnkStruct_ScenePos *p, s32 n);
void SysSound_PlayMenuVoice(s32 a);
void SysSound_PlayMenuVoiceAlt(s32 a);
void SysSound_PlaySE(s32 a);
void func_ov000_02356810(UnkStruct_Scene *w);
void func_ov000_023582b0(UnkStruct_Scene *w);
void func_ov000_02358278(UnkStruct_Scene *w);
void func_ov000_02358108(UnkStruct_Scene *w, s32 x);

#endif

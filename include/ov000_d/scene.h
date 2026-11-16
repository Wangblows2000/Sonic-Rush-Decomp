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
void PlayerSelectMenu_DrawSelectionCursor(UnkStruct_Scene *w, s32 sel, s32 scale);
void PlayerSelectMenu_SetBackgroundSelection(UnkStruct_Scene *w, s32 sel, s32 scroll);
void PlayerSelectMenu_DrawSprite60C(UnkStruct_Scene *w, s32 x, s32 y);
void PlayerSelectMenu_DrawSprite654(UnkStruct_Scene *w, s32 x, s32 y);
void PlayerSelectMenu_DrawSprite69C(UnkStruct_Scene *w, s32 x, s32 y);
void PlayerSelectMenu_DrawSprite6E4(UnkStruct_Scene *w, s32 x, s32 y);
void PlayerSelectMenu_DrawSprite9B4(UnkStruct_Scene *w, s32 x, s32 y);
void PlayerSelectMenu_DrawSpriteA8C(UnkStruct_Scene *w, s32 x, s32 y);
void PlayerSelectMenu_DrawOrbitSprite(UnkStruct_Scene *w, s32 i, s32 ang);
void PlayerSelectMenu_DrawSprite804Scaled(UnkStruct_Scene *w, s32 x, s32 y, s32 scale);
void PlayerSelectMenu_DrawBottomSpriteScaled(UnkStruct_Scene *w, s32 x, s32 scale);
void PlayerSelectMenu_DrawSubTitleSprite2(UnkStruct_Scene *w, BOOL flag);
void PlayerSelectMenu_DrawSubTitleSprite(UnkStruct_Scene *w, BOOL flag);
void PlayerSelectMenu_DrawSprite9FC(UnkStruct_Scene *w);
void PlayerSelectMenu_DrawSprite804(UnkStruct_Scene *w, s32 x, s32 y);
s32 PlayerSelectMenu_EaseValue(s32 from, s32 to, s32 total, s32 cur, s32 bias);
void PlayerSelectMenu_SetModelSlotSelection(UnkStruct_Mdl *m, s32 sel);
void PlayerSelectMenu_SetModelSelection(UnkStruct_Mdl *m, s32 sel);
void PlayerSelectMenu_SetCameraOrbit(UnkStruct_Mdl *m, s32 a);
void KeyframeAnim_Update(UnkStruct_ScenePos *p);
void KeyframeAnim_SetToLastFrame(UnkStruct_ScenePos *p);
BOOL KeyframeAnim_IsFinished(UnkStruct_ScenePos *p);
void PlayerSelectMenu_SetSprite5C4Anim0(UnkStruct_Scene *w);
void PlayerSelectMenu_DrawSpriteA44(UnkStruct_Scene *w, s32 a, s32 b);
void PlayerSelectMenu_SetBgScale(UnkStruct_Scene *w, s32 scale);
void PlayerSelectMenu_SetSprite60CAnim3(UnkStruct_Scene *w);
void PlayerSelectMenu_SetSprite654Anim3(UnkStruct_Scene *w);
void PlayerSelectMenu_SetSpriteA8CAnim1(UnkStruct_Scene *w);
void PlayerSelectMenu_SetSpriteA8CAnim0(UnkStruct_Scene *w);
void Task_DestroyRange(s32 a, s32 b);
void PlayerSelectMenu_StartBrightnessFade(s32 x, s32 y, s32 a, s32 b);
void MsgBox_Create(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
void PlayerSelectMenu_SetSprite9B4ConfirmAnim(UnkStruct_Scene *w);
void PlayerSelectMenu_SetSprite804ConfirmAnim(UnkStruct_Scene *w, s32 sel);
void PlayerSelectMenu_SetSprite804SelectAnim(UnkStruct_Scene *w, s32 sel);
void PlayerSelectMenu_SetSprite654Anim4(UnkStruct_Scene *w);
void PlayerSelectMenu_SetSprite60CAnim4(UnkStruct_Scene *w);
void PlayerSelectMenu_SetSprite69CAnim6(UnkStruct_Scene *w);
void PlayerSelectMenu_SetSprite6E4Anim7(UnkStruct_Scene *w);
void PlayerSelectMenu_SetSprite9B4Anim(UnkStruct_Scene *w);
void PlayerSelectMenu_SetSprite9FCAnim1D(UnkStruct_Scene *w);
void PlayerSelectMenu_SetSlotSpriteNop(UnkStruct_Scene *w, s32 i);
void PlayerSelectMenu_SetSprite5C4Anim2(UnkStruct_Scene *w);
void PlayerSelectMenu_SetSprite5C4Anim1(UnkStruct_Scene *w);
void KeyframeAnim_SetAnim(UnkStruct_ScenePos *p, s32 n);
void SysSound_PlayMenuVoice(s32 a);
void SysSound_PlayMenuVoiceAlt(s32 a);
void SysSound_PlaySE(s32 a);
void PlayerSelectMenu_UpdateCharLayerFlags(UnkStruct_Scene *w);
void PlayerSelectMenu_SetSprite96CAnim0(UnkStruct_Scene *w);
void PlayerSelectMenu_SetSprite96CAnim1(UnkStruct_Scene *w);
void PlayerSelectMenu_DrawSubSpriteAtX(UnkStruct_Scene *w, s32 x);

#endif

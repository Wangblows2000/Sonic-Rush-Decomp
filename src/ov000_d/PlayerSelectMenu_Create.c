#include <ov000_d/scene.h>

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
} UnkStruct_0235a2e8_Sync;

typedef struct {
    u8 pad0[8];
    s16 unk8;
} UnkStruct_0235a2e8_Bright;

extern UnkStruct_0235a2e8_Bright data_02087d44;
extern UnkStruct_0235a2e8_Bright data_0208926c;

UnkStruct_SceneTask *Task_Create(void (*f)(void), void (*g)(UnkStruct_SceneTask *), s32 b, s32 c, s32 d, s32 e);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void PlayerSelectMenu_Main(void);
void PlayerSelectMenu_Destructor(UnkStruct_SceneTask *t);
BOOL PlayerSelectMenu_IsFullyUnlocked(void);
BOOL func_020513a8(void);
void PlayerSelectMenu_InitDisplay(BOOL a);
void PlayerSelectMenu_LoadAssets(UnkStruct_Scene *w);
void PlayerSelectMenu_InitBackgrounds(UnkStruct_Scene *w);
void PlayerSelectMenu_InitSprites(UnkStruct_Scene *w);
void PlayerSelectMenu_LoadModels(UnkStruct_Mdl *m);
void PlayerSelectMenu_LoadArchives(UnkStruct_Mdl *m, BOOL flag);
void PlayerSelectMenu_CreateLinkIcon(void *p);
void Touch_Enable(void);
void PlayerSelectMenu_CreatePaletteAnim(void *a, void *b);
void SysSound_Init(void);
UnkStruct_0235a2e8_Sync *func_020514e4(void);
void PlayerSelectMenu_State_StartSync(void);
void SysSound_PlayBGM(s32 a);
BOOL PlayerSelectMenu_ShouldShowUnlockNotice(void);
void SysSound_PlayBGMAlt(s32 a);
void SysSound_FadeBGMVolume(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void PlayerSelectMenu_Action_Opening(UnkStruct_SceneTask *t);
void PlayerSelectMenu_MarkUnlockNoticeShown(void);

#pragma thumb on

void PlayerSelectMenu_Create(s32 mode) {
    UnkStruct_SceneTask *t;
    UnkStruct_Scene *w;
    BOOL fade;
    BOOL flag;
    UnkStruct_0235a2e8_Sync *s;
    vu32 zero;

    t = Task_Create(PlayerSelectMenu_Main, PlayerSelectMenu_Destructor, 0, 0x1000, 0x1000, sizeof(UnkStruct_Scene));
    w = t->work;
    zero = 0;
    MIi_CpuClear32(zero, w, sizeof(UnkStruct_Scene));
    w->unk0 = 0;
    w->unkC = mode;
    w->unk14 = 0;
    w->unk18 = 4;
    w->unk1C = 4;
    w->unk24 = 4;
    switch (mode) {
    case 0:
        if (PlayerSelectMenu_IsFullyUnlocked()) {
            w->unk10 = 3;
        } else {
            w->unk10 = 2;
        }
        break;
    case 1:
        w->unk14 = 0;
        w->unk10 = 2;
        break;
    case 2:
        w->unk14 = 1;
        w->unk10 = 2;
        break;
    case 3:
    case 4:
        w->unk10 = 2;
        break;
    }
    w->unk32C = 4;
    w->unk338 = 1;
    w->unk340 = 0;
    switch (mode) {
    case 0:
    case 3:
    case 4:
        fade = TRUE;
        break;
    case 1:
    case 2:
        if (func_020513a8()) {
            fade = FALSE;
        } else {
            fade = TRUE;
        }
        break;
    }
    PlayerSelectMenu_InitDisplay(fade);
    PlayerSelectMenu_LoadAssets(w);
    PlayerSelectMenu_InitBackgrounds(w);
    PlayerSelectMenu_InitSprites(w);
    if ((w->unkC == 1 || w->unkC == 2) && func_020513a8()) {
        PlayerSelectMenu_LoadModels(&w->mdl);
    } else {
        PlayerSelectMenu_LoadArchives(&w->mdl, PlayerSelectMenu_IsFullyUnlocked());
    }
    if (w->unkC == 1 || w->unkC == 2) {
        PlayerSelectMenu_CreateLinkIcon(w->unkAD4);
    }
    Touch_Enable();
    PlayerSelectMenu_CreatePaletteAnim(w->unk4C4, w->unk504);
    SysSound_Init();
    if (w->unkC == 1) {
        s = func_020514e4();
        s->unk0 = 0;
        s->unk2 = 0;
        s->unk1 = 0;
        s->unk3 = 0;
    } else if (w->unkC == 2) {
        s = func_020514e4();
        s->unk0 = 1;
        s->unk2 = 0;
        s->unk1 = 0;
        s->unk3 = 0;
    }
    if (w->unkC == 1 || w->unkC == 2) {
        w->unk0 = 0;
        t->unk8 = PlayerSelectMenu_State_StartSync;
        return;
    }
    if (data_0208926c.unk8 >= 0) {
        data_02087d44.unk8 = 0x10;
        data_0208926c.unk8 = data_02087d44.unk8;
    } else {
        data_02087d44.unk8 = -0x10;
        data_0208926c.unk8 = data_02087d44.unk8;
    }
    PlayerSelectMenu_StartBrightnessFade(data_0208926c.unk8, 0, 0x10, 0);
    flag = FALSE;
    if (w->unkC == 1 || w->unkC == 2) {
        SysSound_PlayBGM(2);
    } else if (w->unkC == 0 && PlayerSelectMenu_IsFullyUnlocked()) {
        SysSound_PlayBGM(flag);
        if (PlayerSelectMenu_ShouldShowUnlockNotice()) {
            flag = TRUE;
            SysSound_PlayBGMAlt(flag);
            w->unk14 = 2;
            SysSound_FadeBGMVolume(0, 0x7f, 0x12c, 0x80, 0x5000, 0x1000);
        }
    } else {
        SysSound_PlayBGM(0);
    }
    if (w->unkC != 1 && !flag) {
        SysSound_PlayMenuVoice(0);
    }
    PlayerSelectMenu_Action_Opening(t);
    if (flag) {
        PlayerSelectMenu_MarkUnlockNoticeShown();
    }
}

#pragma thumb reset

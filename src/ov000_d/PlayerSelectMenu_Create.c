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
BOOL func_ov000_02356408(void);
BOOL func_020513a8(void);
void func_ov000_02356054(BOOL a);
void PlayerSelectMenu_LoadAssets(UnkStruct_Scene *w);
void func_ov000_0235a0a4(UnkStruct_Scene *w);
void func_ov000_02359e30(UnkStruct_Scene *w);
void func_ov000_02357148(UnkStruct_Mdl *m);
void func_ov000_02357458(UnkStruct_Mdl *m, BOOL flag);
void func_ov000_02356494(void *p);
void Touch_Enable(void);
void func_ov000_02357dbc(void *a, void *b);
void SysSound_Init(void);
UnkStruct_0235a2e8_Sync *func_020514e4(void);
void func_ov000_02358524(void);
void func_02052168(s32 a);
BOOL func_ov000_023563d0(void);
void func_02052128(s32 a);
void func_02051e0c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void func_ov000_02359c0c(UnkStruct_SceneTask *t);
void func_ov000_023563b4(void);

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
        if (func_ov000_02356408()) {
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
    func_ov000_02356054(fade);
    PlayerSelectMenu_LoadAssets(w);
    func_ov000_0235a0a4(w);
    func_ov000_02359e30(w);
    if ((w->unkC == 1 || w->unkC == 2) && func_020513a8()) {
        func_ov000_02357148(&w->mdl);
    } else {
        func_ov000_02357458(&w->mdl, func_ov000_02356408());
    }
    if (w->unkC == 1 || w->unkC == 2) {
        func_ov000_02356494(w->unkAD4);
    }
    Touch_Enable();
    func_ov000_02357dbc(w->unk4C4, w->unk504);
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
        t->unk8 = func_ov000_02358524;
        return;
    }
    if (data_0208926c.unk8 >= 0) {
        data_02087d44.unk8 = 0x10;
        data_0208926c.unk8 = data_02087d44.unk8;
    } else {
        data_02087d44.unk8 = -0x10;
        data_0208926c.unk8 = data_02087d44.unk8;
    }
    func_ov000_02356694(data_0208926c.unk8, 0, 0x10, 0);
    flag = FALSE;
    if (w->unkC == 1 || w->unkC == 2) {
        func_02052168(2);
    } else if (w->unkC == 0 && func_ov000_02356408()) {
        func_02052168(flag);
        if (func_ov000_023563d0()) {
            flag = TRUE;
            func_02052128(flag);
            w->unk14 = 2;
            func_02051e0c(0, 0x7f, 0x12c, 0x80, 0x5000, 0x1000);
        }
    } else {
        func_02052168(0);
    }
    if (w->unkC != 1 && !flag) {
        func_020520e0(0);
    }
    func_ov000_02359c0c(t);
    if (flag) {
        func_ov000_023563b4();
    }
}

#pragma thumb reset

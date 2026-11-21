#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 mode;
    u8 pad8[0xd0 - 8];
    s32 sel;
    u8 padd4[4];
    s32 max;
    u8 paddc[0x10a4 - 0xdc];
} UnkWork_022ec1e4;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022ec1e4 *work;
} UnkTask_022ec1e4;

typedef struct {
    u8 pad0[0x1c];
    u8 unk1c;
    u8 unk1d;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[8];
    s16 bright;
} UnkBright_022ec1e4;

extern UnkStruct_022c4560 gGameState;
extern UnkBright_022ec1e4 data_0208926c;
extern s32 gSaveDataBusy;
extern u32 gSystemFlags;
extern u8 gPad[];

UnkTask_022ec1e4 *Task_Create(void (*fn)(void), void (*dtor)(void), s32 b, s32 c, s32 d, s32 size);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void TimeAttackMenu_State_Enter(void);
void TimeAttackMenu_Destructor(void);
s32 StoryProgress_GetZoneIndex(u8 a);
void TimeAttackMenu_InitDisplay(void);
void TimeAttackMenu_LoadAssets(UnkWork_022ec1e4 *w);
void TimeAttackMenu_InitBackgrounds(UnkWork_022ec1e4 *w);
void TimeAttackMenu_InitSprites(UnkWork_022ec1e4 *w);
void TimeAttackMenu_CreateBgScrollTask(void);
void TimeAttackMenu_CreateFadeTask(s32 a, s32 b, s32 c, s32 d);
void Touch_Enable(void);
void TimeAttackMenu_Unk_ov001_022ebc98(UnkTask_022ec1e4 *t);
void SysSound_Init(void);
void SysSound_PlayBGM(s32 a);
void Pad_Reset(void *p, s32 a, void *b);

#pragma thumb on
void TimeAttackMenu_Create(s32 mode) {
    UnkTask_022ec1e4 *t;
    UnkWork_022ec1e4 *w;
    vu16 fill;
    vu32 zero;
    u16 buf[12];

    t = Task_Create(TimeAttackMenu_State_Enter, TimeAttackMenu_Destructor, 0, 0x1000, 0x1000, sizeof(UnkWork_022ec1e4));
    w = t->work;
    zero = 0;
    MIi_CpuClear32(zero, w, sizeof(UnkWork_022ec1e4));
    w->unk0 = 0;
    w->mode = mode;
    w->sel = 0;
    switch (w->mode) {
    case 0:
    case 2:
        w->max = 0xe;
        w->max += 4;
        break;
    case 1:
    case 3:
        w->max = 7;
        break;
    }
    switch (w->mode) {
    case 0:
        w->sel = gGameState.unk1d + StoryProgress_GetZoneIndex(gGameState.unk1c) * 2;
        break;
    case 1:
        w->sel = StoryProgress_GetZoneIndex(gGameState.unk1c);
        break;
    }
    if (w->sel >= w->max) {
        w->sel = 0;
    }
    TimeAttackMenu_InitDisplay();
    TimeAttackMenu_LoadAssets(w);
    TimeAttackMenu_InitBackgrounds(w);
    TimeAttackMenu_InitSprites(w);
    TimeAttackMenu_CreateBgScrollTask();
    if (data_0208926c.bright >= 0) {
        data_0208926c.bright = 0x10;
    } else {
        data_0208926c.bright = -0x10;
    }
    TimeAttackMenu_CreateFadeTask(data_0208926c.bright, 0, 8, 0);
    Touch_Enable();
    TimeAttackMenu_Unk_ov001_022ebc98(t);
    SysSound_Init();
    SysSound_PlayBGM(0);
    gSaveDataBusy = 0;
    gSystemFlags &= ~0x10;
    fill = 4;
    MIi_CpuClear16(fill, buf, sizeof(buf));
    Pad_Reset(gPad, 0, buf);
}
#pragma thumb reset

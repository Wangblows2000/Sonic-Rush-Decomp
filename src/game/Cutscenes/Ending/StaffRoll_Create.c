#include <nitro/types.h>

typedef struct {
    u8 pad0[0x40];
} UnkBg_022d2e3c;

typedef struct {
    s32 mode;
    u8 pad4[4];
    void *task8;
    u8 unkc[0x48];
    UnkBg_022d2e3c bg54;
    UnkBg_022d2e3c bg94;
    u8 padd4[0x80];
    UnkBg_022d2e3c bg154;
    UnkBg_022d2e3c bg194;
    UnkBg_022d2e3c bg1d4;
    UnkBg_022d2e3c bg214;
    UnkBg_022d2e3c bg254;
    UnkBg_022d2e3c bg294;
    u8 pad2d4[0xbb0 - 0x2d4];
    s32 unkbb0;
    u8 padbb4[0x10];
    s32 unkbc4;
} UnkWork_022d2e3c;

typedef struct {
    u8 pad0[0x182];
    u16 unk182;
} UnkGlobal_022d2e3c;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022d2e3c *work;
} UnkTask_022d2e3c;

typedef struct {
    u8 pad0[8];
    u32 flags;
    u16 unkc;
} UnkSave_022d2e3c;

typedef struct {
    u8 pad0[0x24];
    u16 unk24;
} UnkSave2_022d2e3c;

extern UnkGlobal_022d2e3c *gStaffRollWork;
extern UnkSave_022d2e3c gSaveData;
extern UnkSave2_022d2e3c data_022c46c0;

UnkTask_022d2e3c *Task_Create(void (*func)(void), void (*dtor)(void), s32 a, s32 b, s32 c, s32 size);
void MI_CpuFill8(void *dst, s32 v, u32 size);
void Touch_Enable(void);
void StaffRoll_SetPictureState(UnkWork_022d2e3c *w, void (*f)(void));
void StaffRoll_SetState(UnkWork_022d2e3c *w, void (*f)(void));
void StaffRoll_InitDisplay(UnkWork_022d2e3c *w);
void StaffRoll_LoadAssets(UnkWork_022d2e3c *w);
void StaffRoll_InitSprites(UnkWork_022d2e3c *w);
void Background_Draw(void *bg);
void NNS_SndArcStrmStart(void *p, s32 a, s32 b);
void StaffRoll_Main(void);
void StaffRoll_Destructor(void);
void StaffRoll_PictureState_SonicStart(void);
void StaffRoll_PictureState_BlazeStart(void);
void StaffRoll_State_Start(void);
void StaffRoll_ScrollBackgrounds(void);

#pragma thumb on
void StaffRoll_Create(s32 mode) {
    UnkWork_022d2e3c *w;

    *(vu32 *)0x4000000 = *(vu32 *)0x4000000 & ~0x1f00;
    *(vu32 *)0x4001000 = *(vu32 *)0x4001000 & ~0x1f00;
    w = Task_Create(StaffRoll_Main, StaffRoll_Destructor, 0, 0x5000, 0x1173, 0x150c)->work;
    MI_CpuFill8(w, 0, 0x150c);
    gStaffRollWork = (UnkGlobal_022d2e3c *)w;
    Touch_Enable();
    w->mode = mode;
    switch (w->mode) {
    case 0:
        if (gSaveData.unkc >= 0x1a) {
            w->unkbb0 = 1;
        }
        StaffRoll_SetPictureState(w, StaffRoll_PictureState_SonicStart);
        break;
    case 1:
        if (data_022c46c0.unk24 >= 0x1c) {
            w->unkbb0 = 1;
        }
        StaffRoll_SetPictureState(w, StaffRoll_PictureState_BlazeStart);
        break;
    case 2:
        if (gSaveData.flags & 0x1000) {
            w->unkbb0 = 1;
        }
        StaffRoll_SetPictureState(w, NULL);
        break;
    }
    StaffRoll_SetState(w, StaffRoll_State_Start);
    StaffRoll_InitDisplay(w);
    StaffRoll_LoadAssets(w);
    StaffRoll_InitSprites(w);
    w->unkbc4 = gStaffRollWork->unk182 * 8 - 0x1d0;
    w->task8 = Task_Create(StaffRoll_ScrollBackgrounds, NULL, 0, 0xf100, 0x1173, 0);
    Background_Draw(&w->bg54);
    Background_Draw(&w->bg154);
    Background_Draw(&w->bg194);
    Background_Draw(&w->bg1d4);
    Background_Draw(&w->bg94);
    Background_Draw(&w->bg214);
    Background_Draw(&w->bg254);
    Background_Draw(&w->bg294);
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0x1f00;
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1f00;
    if (w->mode == 2) {
        NNS_SndArcStrmStart(w->unkc, 1, 0);
        return;
    }
    NNS_SndArcStrmStart(w->unkc, 0, 0);
}
#pragma thumb reset

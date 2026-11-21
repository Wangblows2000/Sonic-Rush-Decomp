#include <nitro/types.h>

typedef struct {
    s32 state;
    s32 unk4;
    s32 unk8;
    u8 padc[0x82c - 0xc];
} UnkWork_022d94b4;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022d94b4 *work;
} UnkTask_022d94b4;

extern u8 gPad[];

UnkTask_022d94b4 *Task_Create(void (*fn)(void), void (*dtor)(void), s32 b, s32 c, s32 d, s32 e);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void TitleMenu_Main(void);
void TitleMenu_Destructor(void);
s32 TitleMenu_IsPlayerSelectPageUnlocked(void);
void TitleMenu_LoadAssets(UnkWork_022d94b4 *w);
void TitleMenu_InitDisplay(void);
void TitleMenu_InitSprites(UnkWork_022d94b4 *w);
void Touch_Enable(void);
void Pad_Reset(void *p, s32 a, s32 b);
void SysSound_Init(void);
void SysSound_PlayBGM(s32 a);

#pragma thumb on
UnkWork_022d94b4 *TitleMenu_Create(void) {
    UnkWork_022d94b4 *w = Task_Create(TitleMenu_Main, TitleMenu_Destructor, 0, 0x1000, 0x1000, sizeof(UnkWork_022d94b4))->work;
    {
        vu16 z = 0;
        MIi_CpuClear16(z, w, sizeof(UnkWork_022d94b4));
    }
    w->state = 9;
    w->unk4 = 0;
    w->unk8 = TitleMenu_IsPlayerSelectPageUnlocked();
    TitleMenu_LoadAssets(w);
    TitleMenu_InitDisplay();
    TitleMenu_InitSprites(w);
    Touch_Enable();
    Pad_Reset(gPad, 0, 0);
    SysSound_Init();
    SysSound_PlayBGM(0);
    return w;
}
#pragma thumb reset

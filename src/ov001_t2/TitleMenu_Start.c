#include <nitro/types.h>

extern s32 gSaveDataBusy;
extern u32 gSystemFlags;

void *TitleMenu_Create(void);
void TitleMenu_OpenRootPage(void *p, s32 a, s32 b);
void TitleMenu_CreateFadeTask(s32 a, s32 b);
void TitleMenu_Unk_ov001_022d74dc(void *p, s32 a);

#pragma thumb on
void TitleMenu_Start(void) {
    void *p = TitleMenu_Create();

    TitleMenu_OpenRootPage(p, 0, 0);
    TitleMenu_CreateFadeTask(8, 0);
    TitleMenu_Unk_ov001_022d74dc(p, 0);
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0x1f00;
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1f00;
    gSaveDataBusy = 0;
    gSystemFlags &= ~0x10;
}
#pragma thumb reset

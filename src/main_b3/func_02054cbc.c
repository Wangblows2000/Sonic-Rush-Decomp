#include <nitro/types.h>

typedef struct { u32 timer; } UnkWork_02054cbc;
typedef struct { u8 pad0[0x10]; UnkWork_02054cbc *work; } UnkTask_02054cbc;
typedef struct { u8 pad0[8]; s16 unk8; } UnkBlend_02054cbc;
extern UnkTask_02054cbc *gTaskCurrent;
extern UnkBlend_02054cbc data_0208926c;
extern UnkBlend_02054cbc data_02087d44;
extern s32 gSaveDataBusy;
extern u32 gSystemFlags;

void GXx_SetMasterBrightness_(vu16 *reg, s32 v);
void Task_DestroyAll(void);
void SysSound_Release(void);
void SceneChange_ToTitleMenu(void);

#pragma thumb on
void func_02054cbc(void) {
    UnkWork_02054cbc *w = gTaskCurrent->work;

    data_0208926c.unk8 = w->timer << 1;
    data_02087d44.unk8 = w->timer << 1;
    w->timer++;
    if (w->timer >= 8) {
        data_0208926c.unk8 = 0x10;
        data_02087d44.unk8 = 0x10;
        GXx_SetMasterBrightness_((vu16 *)0x0400006c, data_0208926c.unk8);
        GXx_SetMasterBrightness_((vu16 *)0x0400106c, data_02087d44.unk8);
        Task_DestroyAll();
        SysSound_Release();
        gSaveDataBusy = 0;
        gSystemFlags &= ~0x10;
        SceneChange_ToTitleMenu();
    }
}
#pragma thumb reset

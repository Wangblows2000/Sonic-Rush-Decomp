#include <nitro/types.h>

extern s32 data_022c4560;
extern s32 gSaveDataBusy;
extern u32 gSystemFlags;

void func_0204ec40(void);
void func_ov001_022c5d1c(void);
void SysSound_Release(void);
void SysSound_Init(void);
void func_02052168(s32 a);
void NaviBg_Load(s32 a, s32 b);
void NaviM_Load(void);
void NaviMsg_Create(void);
void func_0206488c(void);
void func_02063f50(void);
void func_02053b04(s32 a);
void func_ov001_022e190c(s32 a, s32 b, s32 c);
void Touch_Enable(void);
void *Task_Create(void (*fn)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void func_ov001_022c5fa0(void);

#pragma thumb on
void func_ov001_022c6004(void) {
    data_022c4560 = 0;
    func_0204ec40();
    func_ov001_022c5d1c();
    SysSound_Release();
    SysSound_Init();
    func_02052168(4);
    NaviBg_Load(0, 0);
    NaviM_Load();
    NaviMsg_Create();
    func_0206488c();
    func_02063f50();
    func_02053b04(0);
    func_ov001_022e190c(0, 0, 3);
    Touch_Enable();
    gSaveDataBusy = 0;
    gSystemFlags &= ~0x10;
    Task_Create(func_ov001_022c5fa0, 0, 0, 0x5000, 0x2000, 0);
}
#pragma thumb reset

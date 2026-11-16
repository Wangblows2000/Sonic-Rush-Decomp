#include <nitro/types.h>

typedef struct {
    s32 mode;
} UnkWork_022d2db8;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022d2db8 *work;
} UnkTask_022d2db8;

extern UnkTask_022d2db8 *gTaskCurrent;
extern s32 gStaffRollWork;

void Task_DestroyByTag(s32 a);
void Touch_Disable(void);
void func_02059bdc(s32 a);
void func_02059bcc(s32 a);
void func_02059b2c(void);
BOOL func_0205a4ec(s32 a);
void SceneChange_Unk_02054e30(void);

#pragma thumb on
void func_ov001_022d2db8(void) {
    s32 mode;

    mode = gTaskCurrent->work->mode;
    gStaffRollWork = 0;
    Task_DestroyByTag(0x1173);
    Touch_Disable();
    switch (mode) {
    case 0:
        func_02059bdc(0x1a);
        break;
    case 1:
        func_02059bcc(0x1c);
        break;
    case 2:
        func_02059b2c();
        break;
    }
    if (func_0205a4ec(0)) {
        SceneChange_Unk_02054e30();
    }
}
#pragma thumb reset

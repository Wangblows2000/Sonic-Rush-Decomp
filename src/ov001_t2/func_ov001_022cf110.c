#include <nitro/types.h>

typedef struct {
    s32 timer;
    s32 exit;
} UnkWork_022cf110;

typedef struct UnkTask_022cf110 {
    u8 pad0[8];
    void (*fn)(void);
    u8 padc[4];
    UnkWork_022cf110 *work;
} UnkTask_022cf110;

typedef struct {
    u8 pad[8];
    s16 bright;
} UnkBright_022cf110;

extern UnkTask_022cf110 *gTaskCurrent;
extern UnkBright_022cf110 data_0208926c;
extern UnkBright_022cf110 data_02087d44;

void SysSound_Release(void);
void Task_Destroy(UnkTask_022cf110 *t);
void SceneChange_Unk_020550ec(void);
void func_ov001_022cf0b4(void);

#pragma thumb on
void func_ov001_022cf110(void) {
    UnkWork_022cf110 *w = gTaskCurrent->work;
    s32 v = (w->timer << 4) / 16;
    s16 b;

    if (v < -16) v = 16;
    if (v > 16) v = 16;
    b = v;
    data_0208926c.bright = b;
    data_02087d44.bright = b;
    w->timer++;
    if (w->timer >= 16) {
        data_0208926c.bright = 16;
        data_02087d44.bright = 16;
        if (w->exit != 0) {
            SysSound_Release();
            Task_Destroy(gTaskCurrent);
            SceneChange_Unk_020550ec();
        } else {
            w->timer = 0;
            gTaskCurrent->fn = func_ov001_022cf0b4;
        }
    }
}
#pragma thumb reset

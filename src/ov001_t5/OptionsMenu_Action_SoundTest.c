#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    void *work;
} UnkTask_022cddf0;

extern UnkTask_022cddf0 *gTaskCurrent;
extern u8 gPad[];

void SysSound_Release(void);
void func_ov001_022cb770(void *w);
void Pad_Reset(void *a, s32 b, u16 *c);
void OptionsMenu_State_SoundTest(void);

#pragma thumb on
void OptionsMenu_Action_SoundTest(void) {
    void *w;
    s32 i;
    u16 buf[12];

    w = gTaskCurrent->work;
    SysSound_Release();
    func_ov001_022cb770(w);
    for (i = 0; i < 12; i++) {
        buf[i] = 4;
    }
    Pad_Reset(gPad, 0, buf);
    gTaskCurrent->func = OptionsMenu_State_SoundTest;
}
#pragma thumb reset

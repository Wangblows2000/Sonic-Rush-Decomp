#include <nitro/types.h>

typedef struct {
    s32 t;
    s32 unk4;
} UnkWork_022cf600;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022cf600 *work;
} UnkTask_022cf600;

typedef struct {
    u8 pad[8];
    s16 bright;
} UnkBright_022cf600;

extern UnkTask_022cf600 *gTaskCurrent;
extern UnkBright_022cf600 data_0208926c;
extern UnkBright_022cf600 data_02087d44;

s32 SegaLogo_Interpolate(s32 from, s32 to, s32 n, s32 i, fx32 ease);
void SegaLogo_State_End(void);

#pragma thumb on
void SegaLogo_State_FadeOut(void) {
    UnkWork_022cf600 *w = gTaskCurrent->work;
    s16 v = SegaLogo_Interpolate(0, 0x10, 0x20, w->t, 0x1000);

    data_0208926c.bright = v;
    data_02087d44.bright = v;
    w->t++;
    if (w->t >= 0x20) {
        w->t = 0;
        data_0208926c.bright = 0x10;
        data_02087d44.bright = 0x10;
        gTaskCurrent->func = SegaLogo_State_End;
    }
}
#pragma thumb reset

#include <nitro/types.h>

typedef struct {
    s32 t;
    s32 unk4;
} UnkWork_022cf688;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022cf688 *work;
} UnkTask_022cf688;

typedef struct {
    u8 pad[8];
    s16 bright;
} UnkBright_022cf688;

extern UnkTask_022cf688 *data_022b4574;
extern UnkBright_022cf688 data_0208926c;
extern UnkBright_022cf688 data_02087d44;

s32 func_ov001_022cf4f0(s32 from, s32 to, s32 n, s32 i, fx32 ease);
void func_ov001_022cf660(void);

#pragma thumb on
void func_ov001_022cf688(void) {
    UnkWork_022cf688 *w = data_022b4574->work;
    s16 v = func_ov001_022cf4f0(w->unk4, 0, 0x20, w->t, 0x1000);

    data_0208926c.bright = v;
    data_02087d44.bright = v;
    w->t++;
    if (w->t >= 0x20) {
        w->t = 0;
        data_0208926c.bright = 0;
        data_02087d44.bright = 0;
        data_022b4574->func = func_ov001_022cf660;
    }
}
#pragma thumb reset

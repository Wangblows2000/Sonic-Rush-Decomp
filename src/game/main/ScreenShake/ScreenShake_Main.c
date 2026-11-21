#include <nitro/types.h>

typedef struct {
    u16 flags;
    s16 t;
    u8 mode;
    u8 pad5;
    s8 x;
    s8 y;
} UnkWork_0204440c;

typedef struct {
    u8 pad0[0x10];
    UnkWork_0204440c *work;
} UnkTask_0204440c;

extern UnkTask_0204440c *gTaskCurrent;
extern s8 data_02073240[];
extern s8 data_0207323c[];
extern s8 data_0207324c[];

void ScreenShake_Stop(void);

void ScreenShake_Main(void) {
    UnkWork_0204440c *w = gTaskCurrent->work;

    w->x = 0;
    w->y = 0;
    if (!(w->flags & 1))
        return;
    switch (w->mode) {
    case 1:
        if (w->t > 8) {
            ScreenShake_Stop();
            break;
        }
    case 5:
        if (w->t >= 12) {
            ScreenShake_Stop();
            break;
        }
    case 9:
        if (w->t >= 12)
            w->t = 0;
        w->x = (data_02073240[w->t] & 0xf) >> 1;
        w->y = data_02073240[w->t] >> 5;
        break;
    case 2:
        if (w->t > 8) {
            ScreenShake_Stop();
            break;
        }
    case 6:
        if (w->t >= 12) {
            ScreenShake_Stop();
            break;
        }
    case 10:
        if (w->t >= 12)
            w->t = 0;
        w->x = data_02073240[w->t] & 0xf;
        w->y = data_02073240[w->t] >> 4;
        break;
    case 3:
        if (w->t >= 8) {
            ScreenShake_Stop();
            break;
        }
    case 7:
        if (w->t >= 16) {
            ScreenShake_Stop();
            break;
        }
    case 11:
        if (w->t >= 16)
            w->t = 0;
        w->x = data_0207323c[w->t & 3] & 0xf;
        w->y = data_0207323c[w->t & 3] >> 4;
        break;
    case 4:
        if (w->t >= 8) {
            ScreenShake_Stop();
            break;
        }
    case 8:
        if (w->t >= 0x16) {
            ScreenShake_Stop();
            break;
        }
    case 12:
        if (w->t >= 0x16)
            w->t = 0;
        w->x = data_0207324c[w->t] & 0xf;
        w->y = data_0207324c[w->t] >> 4;
        break;
    }
    if (w->x & 8)
        w->x = -(0x10 - w->x);
    w->t++;
}

#include <nitro/types.h>

typedef struct {
    u16 flags;
    s16 bright;
    s16 step;
    s16 delay;
} UnkWork_02043b78;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02043b78 *work;
} UnkTask_02043b78;

extern UnkTask_02043b78 *data_022b4574;

void func_020371c8(UnkTask_02043b78 *t);
void func_02043abc(UnkWork_02043b78 *w);

void func_02043b78(void) {
    UnkWork_02043b78 *w = data_022b4574->work;

    if (w->flags & 2) {
        if (w->bright <= 0) {
            w->flags |= 0x20;
            if (!(w->flags & 4)) {
                func_020371c8(data_022b4574);
                return;
            }
        }
        if (w->delay != 0)
            w->delay--;
        else
            w->bright -= w->step;
        if (w->bright < 0)
            w->bright = 0;
    } else {
        if (w->bright >= 0x1000) {
            w->flags |= 0x20;
            if (!(w->flags & 4)) {
                func_020371c8(data_022b4574);
                return;
            }
        }
        w->bright += w->step;
        if (w->bright > 0x1000)
            w->bright = 0x1000;
    }
    func_02043abc(w);
}

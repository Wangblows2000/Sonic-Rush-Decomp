#include <nitro/types.h>

typedef struct {
    void (*state)(void *);
    s32 x;
    s32 y;
    u8 mode;
    u8 padd;
    u16 timer;
} UnkFade_020618a4;

extern u32 data_0208fe70;

BOOL func_02061940(UnkFade_020618a4 *f);

static inline u16 Rand_020618a4(void) {
    data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
    return data_0208fe70 >> 16;
}

#pragma thumb on
BOOL func_020618a4(UnkFade_020618a4 *f) {
    BOOL done = FALSE;
    s32 bx;
    s32 by;

    if (f->mode == 2) {
        bx = 0;
        by = 0x3f00;
    } else {
        bx = 0x5000;
        by = 0x3f00;
    }
    f->timer--;
    if (f->timer != 0) {
        if (f->timer & 2) {
            f->x = bx + (0x300 - (Rand_020618a4() & 0x600));
            f->y = by + (0x300 - (Rand_020618a4() & 0x600));
        }
    } else {
        f->x = bx;
        f->y = by;
        done = TRUE;
    }
    func_02061940(f);
    return done;
}
#pragma thumb reset

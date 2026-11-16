#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u32 mode;
} UnkWork_022e9708;

#pragma thumb on
void TimeAttackMenu_Unk_ov001_022e9708(UnkWork_022e9708 *w, s32 n, s32 *x, s32 *y) {
    switch (w->mode) {
    case 0:
    case 2:
        if (n >= 14) {
            if (x != NULL) *x = 7;
            if (y != NULL) *y = n - 14;
        } else {
            if (x != NULL) *x = n / 2;
            if (y != NULL) *y = n % 2;
        }
        break;
    case 1:
    case 3:
        if (x != NULL) *x = n;
        if (y != NULL) *y = 2;
        break;
    }
}
#pragma thumb reset

#include <nitro/types.h>

typedef struct {
    u16 x;
    u16 y;
    u16 touch;
    u16 validity;
} UnkTP_02037650;

typedef struct {
    u16 x;
    u16 y;
} UnkXY_02037650;

typedef struct {
    u16 x;
    u16 y;
    u16 lastX;
    u16 lastY;
    u8 unk8[8];
    u32 flags;
    u8 count;
} UnkTPState_02037650;

extern UnkTP_02037650 gTouchSampleBuffer[];
extern UnkTPState_02037650 gTouchState;
extern UnkXY_02037650 data_022b65ca[];

void TP_GetCalibratedPoint(UnkTP_02037650 *out, const UnkTP_02037650 *raw);

void Touch_FilterAutoSamples(s32 idx) {
    u8 n = gTouchState.count;
    UnkTP_02037650 tp[4] = {0};
    u8 i;
    u8 j;
    UnkTP_02037650 *raw;
    UnkTPState_02037650 *st;
    UnkXY_02037650 *hist;
    raw = gTouchSampleBuffer;
    st = &gTouchState;
    hist = data_022b65ca;
    for (i = 0; i < n; i++) {
        TP_GetCalibratedPoint(&tp[i], &raw[(u8)((idx - i + 5) % 5)]);
    }
    if (tp[0].validity == 0) {
        st->x = tp[0].x;
        st->y = tp[0].y;
        st->flags &= ~0x100;
    } else {
        for (i = 1; i < n; i++) {
            if (tp[i].validity == 0 && tp[i].touch == 1) {
                st->x = tp[i].x;
                st->y = tp[i].y;
                break;
            }
        }
        if (i < n) {
            st->flags &= ~0x100;
        } else {
            st->x = gTouchState.lastX;
            st->y = gTouchState.lastY;
            st->flags |= 0x100;
        }
    }
    j = n - 1;
    hist[j].x = st->x;
    hist[j].y = st->y;
    j--;
    for (i = 1; i < n; i++, j--) {
        if (tp[i].validity == 0 && tp[i].touch == 1) {
            hist[j].x = tp[i].x;
            hist[j].y = tp[i].y;
        } else {
            hist[j].x = hist[j + 1].x;
            hist[j].y = hist[j + 1].y;
        }
    }
}

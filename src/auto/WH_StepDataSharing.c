/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022c2440[];
extern u8 data_022c1ba0[];
s32 WM_StepDataSharing();
extern u8 sTraceFunc[];
extern u8 data_020786dc[];
s32 WH_SetError();

s32 WH_StepDataSharing(s32 p0) {
    s32 t0 = WM_StepDataSharing(data_022c2440, p0, data_022c1ba0);
    if (t0 == 7) {
        return 1;
    }
    if (t0 == 5) {
        s32 t1 = *(u32 *)sTraceFunc;
        if (t1 != 0) {
            ((s32 (*)())t1)(data_020786dc, t1);
        }
        WH_SetError(t0);
        return 0;
    }
    if (t0 == 0) {
        return 1;
    }
    WH_SetError(t0);
    return 0;
}

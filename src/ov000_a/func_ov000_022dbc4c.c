#include <nitro/types.h>

extern s32 gGameState;
extern s32 data_ov000_02369b30[];
extern u16 data_ov000_02363638[][6];
extern u16 data_ov000_02363620[][6];
extern u32 data_ov000_02365710[];
extern u16 data_ov000_02363564[];

void func_ov000_022c5d4c(s32 a, u32 *p1, u16 *p2, u16 *p3, u16 n);

void func_ov000_022dbc4c(BOOL f) {
    s32 k = gGameState;
    u16 *t;
    if (f)
        t = data_ov000_02363638[k];
    else
        t = data_ov000_02363620[k];
    func_ov000_022c5d4c(data_ov000_02369b30[1], data_ov000_02365710, t, data_ov000_02363564, 6);
}

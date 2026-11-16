#include <nitro/types.h>

extern s32 data_ov000_02369b18[];
extern u16 data_ov000_02363160[][6];
extern u16 data_ov000_02363148[][6];
extern u32 data_ov000_02365488[];
extern u16 data_ov000_023630ac[];

void func_ov000_022c5d4c(s32 a, u32 *p1, u16 *p2, u16 *p3, u16 n);

void func_ov000_022dae28(s32 k, BOOL f) {
    u16 *t;
    if (f)
        t = data_ov000_02363160[k];
    else
        t = data_ov000_02363148[k];
    func_ov000_022c5d4c(data_ov000_02369b18[1], data_ov000_02365488, t, data_ov000_023630ac, 6);
}

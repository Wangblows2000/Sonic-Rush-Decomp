/* paramclone of func_ov000_022dae28 (src/ov000_a/func_ov000_022dae28.c); immediates and relocations remapped. */
#include <nitro/types.h>

extern s32 data_ov000_02369b88[];
extern u16 data_ov000_02363e90[][3];
extern u16 data_ov000_02363e84[][3];
extern u32 data_ov000_02365d60[];
extern u16 data_ov000_02363de8[];

void func_ov000_022c5d4c(s32 a, u32 *p1, u16 *p2, u16 *p3, u16 n);

void func_ov000_022f8bc8(s32 k, BOOL f) {
    u16 *t;
    if (f)
        t = data_ov000_02363e90[k];
    else
        t = data_ov000_02363e84[k];
    func_ov000_022c5d4c(data_ov000_02369b88[1], data_ov000_02365d60, t, data_ov000_02363de8, 3);
}

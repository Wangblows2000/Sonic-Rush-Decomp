/* paramclone of func_ov000_022dae28 (src/ov000_a/func_ov000_022dae28.c); immediates and relocations remapped. */
#include <nitro/types.h>

extern s32 data_ov000_02369b88[];
extern u16 data_ov000_02363e78[][3];
extern u16 data_ov000_02363e6c[][3];
extern u32 data_ov000_02365d54[];
extern u16 data_ov000_02363df8[];

void func_ov000_022c5d4c(s32 a, u32 *p1, u16 *p2, u16 *p3, u16 n);

void func_ov000_022f8c28(s32 k, BOOL f) {
    u16 *t;
    if (f)
        t = data_ov000_02363e78[k];
    else
        t = data_ov000_02363e6c[k];
    func_ov000_022c5d4c(data_ov000_02369b88[1], data_ov000_02365d54, t, data_ov000_02363df8, 3);
}

#include <nitro/types.h>

extern s32 gGameState[];
extern u32 data_ov000_02363530[];
extern u8 data_ov000_02363508;
extern u32 data_ov000_02363518[];
extern void *data_ov000_02369b30[];
extern u8 data_ov000_023655f8;

void func_ov000_022c5d4c(void *a, void *b, void *c, void *d, s32 e);

void func_ov000_022dbab4(BOOL flag) {
    s32 i = gGameState[0];
    u32 *p;
    if (flag)
        p = &data_ov000_02363530[i];
    else
        p = &data_ov000_02363518[i];
    func_ov000_022c5d4c(data_ov000_02369b30[0xc / 4], &data_ov000_023655f8, p, &data_ov000_02363508, 2);
}

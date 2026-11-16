#include <nitro/types.h>

typedef struct {
    s16 key;
    s16 ref;
} UnkSlot_0206d2d0;

extern s8 data_022c521c;
extern s8 data_0207cfc4;
extern UnkSlot_0206d2d0 data_022c5220[];

void func_0206d190(void *res, u16 col, u16 idx, s32 screen);

u8 func_0206d2d0(void *res, u16 col, s16 key) {
    s8 i;

    if (!(key & 0xf000)) {
        for (i = data_022c521c; i < data_0207cfc4; i++) {
            if (key == data_022c5220[i].key) {
                data_022c5220[i].ref++;
                return i;
            }
        }
    }
    for (i = data_022c521c; i < data_0207cfc4; i++) {
        if (data_022c5220[i].ref == 0) {
            data_022c5220[i].ref++;
            break;
        }
    }
    if (i >= data_0207cfc4)
        return i - 1;
    data_022c5220[i].key = key;
    func_0206d190(res, col, (u8)i, 0);
    func_0206d190(res, col, (u8)i, 1);
    return i;
}

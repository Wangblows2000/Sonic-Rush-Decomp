#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
} UnkStruct_022fae34;

extern UnkStruct_022fae34 data_ov000_02369bac[];
extern u32 data_ov000_02369ba8;
extern u32 data_ov000_02369ba4;

void func_ov000_022fae34(UnkStruct_022fae34 *p) {
    UnkStruct_022fae34 *e;
    u8 i;

    e = data_ov000_02369bac;
    for (i = 0; i < 40; i++, e++) {
        if (e == p) {
            if (i < 32)
                data_ov000_02369ba8 &= ~(1 << i);
            else
                data_ov000_02369ba4 &= ~(1 << (i - 32));
            return;
        }
    }
}

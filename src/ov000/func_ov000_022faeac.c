#include <nitro/types.h>

extern u32 data_ov000_02369ba8;
extern u32 data_ov000_02369ba4;

u8 func_ov000_022faeac(void) {
    u32 a = data_ov000_02369ba8;
    u32 b = data_ov000_02369ba4;
    u8 i;
    for (i = 0; i < 0x28; i++) {
        if (i < 0x20) {
            if (!(a & (1 << i))) {
                data_ov000_02369ba8 |= 1 << i;
                return i;
            }
        } else {
            if (!(b & (1 << (i - 0x20)))) {
                data_ov000_02369ba4 |= 1 << (i - 0x20);
                return i;
            }
        }
    }
    return i;
}

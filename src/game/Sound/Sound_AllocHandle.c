#include <nitro/types.h>

extern u8 data_0225045c[];
extern u32 data_02250468[];
extern u32 data_02250460;

u32 *Sound_AllocHandle(void) {
    u32 i;
    for (i = 0; i < 0x20; i++) {
        u32 bit = 1 << (i & 7);
        if (!(data_0225045c[i >> 3] & bit)) {
            data_0225045c[i >> 3] |= bit;
            return &data_02250468[i];
        }
    }
    return &data_02250460;
}

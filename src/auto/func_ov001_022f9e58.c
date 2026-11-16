/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov001_0231f08c[];
extern u8 data_ov001_0231f090[];

void func_ov001_022f9e58(s32 p0, s32 p1) {
    *(u32 *)data_ov001_0231f08c = p0;
    if ((s32)p0 > 0x8c9f) {
        *(u32 *)data_ov001_0231f08c = 0x8c9f;
    } else {
        if ((s32)p0 < 0) {
            *(u32 *)data_ov001_0231f08c = 0;
        }
    }
    *(u32 *)data_ov001_0231f090 = p1;
    if ((s32)p1 > 0x3e7) {
        *(u32 *)data_ov001_0231f090 = 0x3e7;
        return;
    }
    if ((s32)p1 < 0) {
        *(u32 *)data_ov001_0231f090 = 0;
    }
}

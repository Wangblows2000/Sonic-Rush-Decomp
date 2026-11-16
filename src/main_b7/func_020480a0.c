#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1c];
    u8 zone;
    u8 act;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 data_022c4560;
extern u32 data_02078b48[];
extern u32 data_02078b58[];

#pragma thumb on
s16 func_020480a0(u32 v) {
    s16 i;
    u32 *t;
    if (data_022c4560.zone >= 9 || data_022c4560.act >= 2) {
        for (i = 0, t = data_02078b48; i < 4; t++, i++) {
            if (v >= *t)
                break;
        }
    } else {
        for (i = 0, t = data_02078b58; i < 4; t++, i++) {
            if (v >= *t)
                break;
        }
    }
    return i;
}
#pragma thumb reset

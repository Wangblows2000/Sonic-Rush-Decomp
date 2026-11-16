#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    s32 count;
    u8 pad10[0xbc - 0x10];
    s32 offs[8];
} UnkStruct_022e0dfc;

extern s32 data_ov001_0231c3ec;

#pragma thumb on
void ZoneActSelect_Unk_ov001_022e0dfc(UnkStruct_022e0dfc *p) {
    s32 i;

    for (i = 0; i < p->count; i++) {
        if (i == data_ov001_0231c3ec) {
            p->offs[i] -= 2;
            if (p->offs[i] < -8) {
                p->offs[i] = -8;
            }
        } else {
            p->offs[i] += 2;
            if (p->offs[i] > 0) {
                p->offs[i] = 0;
            }
        }
    }
}
#pragma thumb reset

#include <nitro/types.h>

typedef struct {
    u8 pad0[0x30];
    s32 ids[18];
} UnkStruct_022ce960;

#pragma thumb on
void Options_Unk_ov001_022ce960(UnkStruct_022ce960 *p) {
    s32 *a = p->ids;

    a[0] = 0;
    a[1] = 1;
    a[2] = 7;
    a[3] = 0xd;
    a[4] = 0xe;
    a[5] = 0xf;
    a[6] = 0x10;
    a[7] = 0x11;
    a[8] = 0x12;
    a[9] = 0x13;
    a[10] = 0x14;
    a[11] = 0x15;
    a[12] = 0x16;
    a[13] = 0x17;
    a[14] = 0x18;
    a[15] = 0x19;
    a[16] = 0x1a;
    a[17] = 0x1b;
}
#pragma thumb reset

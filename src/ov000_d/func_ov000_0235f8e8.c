#include <nitro/types.h>

typedef struct {
    u8 pad0[0xC];
    u16 unkC;
} UnkStruct_022c4680;

typedef struct {
    u8 pad0[0x24];
    u16 unk24;
} UnkStruct_022c46c0;

extern UnkStruct_022c4680 gSaveData;
extern UnkStruct_022c46c0 data_022c46c0;

#pragma thumb on

u16 func_ov000_0235f8e8(void) {
    u16 m = 1;
    u16 n = gSaveData.unkC;

    if (n >= 1) {
        m |= 3;
    }
    if (n >= 4) {
        m |= 0xc;
    }
    if (n >= 8) {
        m |= 0x30;
    }
    if (n >= 0xc) {
        m |= 0xc0;
    }
    if (n >= 0x10) {
        m |= 0x300;
    }
    if (n >= 0x14) {
        m |= 0xc00;
    }
    if (n >= 0x17) {
        m |= 0x3000;
    }
    n = data_022c46c0.unk24;
    if (n >= 2) {
        m |= 0xc0;
    }
    if (n >= 6) {
        m |= 3;
    }
    if (n >= 9) {
        m |= 0x30;
    }
    if (n >= 0xd) {
        m |= 0xc;
    }
    if (n >= 0x11) {
        m |= 0xc00;
    }
    if (n >= 0x15) {
        m |= 0x300;
    }
    if (n >= 0x19) {
        m |= 0x3000;
    }
    return m;
}

#pragma thumb reset

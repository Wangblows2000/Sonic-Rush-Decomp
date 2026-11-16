#include <nitro/types.h>

extern u32 gRandSeed;
void func_ov000_0232bc4c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void func_ov000_0232be64(void) {
    switch ((u16)(Rand() & 7)) {
    case 0:
        func_ov000_0232bc4c(0, -0x30, 0x30, 0, -0x800, 0);
        func_ov000_0232bc4c(1, 0, 0, 0, -0x800, 0);
        func_ov000_0232bc4c(2, 0x30, 0x30, 0, -0x800, 0);
        break;
    case 1:
        func_ov000_0232bc4c(0, 0x60, 0x18, 0, -0x700, 0);
        func_ov000_0232bc4c(1, 0x40, 0x18, 0, -0x700, 4);
        func_ov000_0232bc4c(2, 0x20, 0x18, 0, -0x700, 8);
        func_ov000_0232bc4c(0, 0, 0x18, 0, -0x700, 0xc);
        func_ov000_0232bc4c(1, -0x20, 0x18, 0, -0x700, 0x10);
        func_ov000_0232bc4c(2, -0x40, 0x18, 0, -0x700, 0x14);
        func_ov000_0232bc4c(0, -0x60, 0x18, 0, -0x700, 0x18);
        break;
    case 2:
        func_ov000_0232bc4c(0, -0x50, 0x18, 0, -0x780, 0);
        func_ov000_0232bc4c(1, -0x20, 0x18, 0, -0x780, 8);
        func_ov000_0232bc4c(2, 0x20, 0x18, 0, -0x780, 8);
        func_ov000_0232bc4c(0, 0x50, 0x18, 0, -0x780, 0);
        break;
    case 3:
        func_ov000_0232bc4c(0, 0x40, 0x18, 0, -0x780, 4);
        func_ov000_0232bc4c(2, 0x20, 0x18, 0, -0x780, 0xc);
        func_ov000_0232bc4c(1, 0, 0x18, 0, -0x800, 0x18);
        func_ov000_0232bc4c(2, -0x20, 0x18, 0, -0x780, 0x10);
        func_ov000_0232bc4c(0, -0x40, 0x18, 0, -0x780, 8);
        break;
    case 4:
        func_ov000_0232bc4c(2, -0x50, 0x18, 0x180, -0x780, 0);
        func_ov000_0232bc4c(1, 0, 0x18, 0, -0x800, 0x10);
        func_ov000_0232bc4c(2, 0x50, 0x18, -0x180, -0x780, 0);
        break;
    case 5:
        func_ov000_0232bc4c(0, 0x64, 0x18, 0, -0x700, 0);
        func_ov000_0232bc4c(1, 0x3c, 0x18, 0, -0x780, 8);
        func_ov000_0232bc4c(2, 0x14, 0x18, 0, -0x800, 0x10);
        func_ov000_0232bc4c(1, -0x14, 0x18, 0, -0x700, 0);
        func_ov000_0232bc4c(2, -0x3c, 0x18, 0, -0x780, 8);
        func_ov000_0232bc4c(0, -0x64, 0x18, 0, -0x800, 0x10);
        break;
    case 6:
        func_ov000_0232bc4c(0, -0x6c, 0x18, 0x500, -0x700, 0);
        func_ov000_0232bc4c(2, -0x30, 0x18, 0x200, -0x800, 8);
        func_ov000_0232bc4c(2, 0x30, 0x18, -0x200, -0x800, 8);
        func_ov000_0232bc4c(0, 0x6c, 0x18, -0x500, -0x700, 0);
        break;
    case 7:
        func_ov000_0232bc4c(1, 0x40, 0x18, 0, -0x780, 0x18);
        func_ov000_0232bc4c(0, 0x20, 0x18, 0, -0x780, 0xc);
        func_ov000_0232bc4c(2, 0, 0x18, 0, -0x800, 0);
        func_ov000_0232bc4c(0, -0x20, 0x18, 0, -0x780, 0xc);
        func_ov000_0232bc4c(1, -0x40, 0x18, 0, -0x780, 0x18);
        break;
    }
}

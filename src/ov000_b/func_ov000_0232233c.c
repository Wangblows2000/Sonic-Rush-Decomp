#include <nitro/types.h>

extern u32 data_0208fe70;

void func_ov000_02322180(s32 x, s32 y, s16 a, s16 b, s32 c);

static inline u16 Rand(void) {
    data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
    return (u16)(data_0208fe70 >> 16);
}

void func_ov000_0232233c(s32 x, s32 y) {
    func_ov000_02322180(x + (0xfff - (Rand() & 0x1ffe)), y + (0x7ff - (Rand() & 0xffe)), 0xff - (Rand() & 0x1fe), 0xff - (Rand() & 0x1fe), 0);
    func_ov000_02322180(x + (0xfff - (Rand() & 0x1ffe)), y + (0x7ff - (Rand() & 0xffe)), 0xff - (Rand() & 0x1fe), -1 - (Rand() & 0x1fe), 1);
    func_ov000_02322180(x + (0xfff - (Rand() & 0x1ffe)), y + (0x7ff - (Rand() & 0xffe)), 0x7f - (Rand() & 0x1fe), 0xff - (Rand() & 0x1fe), 2);
    func_ov000_02322180(x + (0xfff - (Rand() & 0x1ffe)), y + (0x7ff - (Rand() & 0xffe)), 0x17f - (Rand() & 0x1fe), -0x81 - (Rand() & 0x1fe), 3);
}

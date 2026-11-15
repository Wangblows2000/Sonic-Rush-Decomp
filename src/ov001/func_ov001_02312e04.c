#include <nitro/types.h>

extern u32 data_0208fe70;
extern s32 data_ov001_023178e4[];
void func_02036754(s32 a, s32 b, s32 c);
void func_ov001_023122d8(void *p, s32 v);
static inline u16 Rand(void) {
    data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
    return data_0208fe70 >> 16;
}

void func_ov001_02312e04(void *p) {
    func_02036754(0, 0x10, data_ov001_023178e4[(s32)Rand() % 4]);
    func_ov001_023122d8(p, 0x14);
}

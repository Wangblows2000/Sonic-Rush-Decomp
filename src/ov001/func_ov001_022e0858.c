#include <nitro/types.h>

void func_02035704(void *p, s32 a, s32 b, s32 c);

#pragma thumb on
void func_ov001_022e0858(u8 *p, s32 i) {
    func_02035704(p + 0x160 + i * 32, 0x10, *(s32 *)(p + 0x134), *(s32 *)(p + 0x138));
}
#pragma thumb reset

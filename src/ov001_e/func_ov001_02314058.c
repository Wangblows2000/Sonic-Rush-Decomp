#include <nitro/types.h>

void func_02025268(s32 a, void *b, s32 c);

void func_ov001_02314058(s32 v) {
    s32 a = (v << 16) | 0xc0;

    func_02025268(0x29, &a, 1);
}

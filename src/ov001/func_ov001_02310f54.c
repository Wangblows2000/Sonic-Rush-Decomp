#include <nitro/types.h>

s32 func_ov001_0230d6e0(void);
s32 func_0202f148(u32 f);
void func_ov001_0230d6f4(s32 v);

void func_ov001_02310f54(void) {
    s32 v = func_ov001_0230d6e0();
    v -= func_0202f148(0x4336febc);
    if (v < 0) {
        v = 0;
    }
    func_ov001_0230d6f4(v);
}

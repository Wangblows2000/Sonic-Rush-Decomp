#include <nitro/types.h>

extern u8 data_ov000_02367c8c;

void func_ov000_02327ca4(void *p, s32 id, s32 a, s32 b, s32 c, s32 d);

void func_ov000_023280e8(s32 a, s32 b, s32 c, s32 d) {
    func_ov000_02327ca4(&data_ov000_02367c8c, 8, a, b, c, d);
}

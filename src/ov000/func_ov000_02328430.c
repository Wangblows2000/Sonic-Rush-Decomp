#include <nitro/types.h>

extern u8 data_ov000_02367d3c;

void func_ov000_02327ca4(void *p, s32 id, s32 a, s32 b, s32 c, s32 d);

void func_ov000_02328430(s32 a, s32 b, s32 c, s32 d) {
    func_ov000_02327ca4(&data_ov000_02367d3c, 11, a, b, c, d);
}

#include <nitro/types.h>

extern u8 *data_ov001_0231f5bc;
extern s32 data_02250438;
extern s32 data_02250434;
void func_02035704(void *p, s32 a, s32 b, u32 c);
void func_02035754(void);

void func_ov001_0230956c(void) {
    s32 a = data_02250438;
    s32 b = data_02250434;
    data_02250438 = 0;
    data_02250434 = 0;
    func_02035704(data_ov001_0231f5bc + 0xb88, 0x10, 0, 0x05000700);
    func_02035754();
    data_02250438 = a;
    data_02250434 = b;
}

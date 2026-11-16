#include <nitro/types.h>

extern s32 data_02250438;
extern s32 data_02250434;

void func_02035704(u16 *src, s32 n, s32 a, u32 dst);
void func_02035754(void);

#pragma thumb on
void func_ov001_022d9a14(void *p, u16 col) {
    s32 s0 = data_02250438;
    s32 s1 = data_02250434;
    data_02250438 = 0;
    data_02250434 = 0;
    func_02035704(&col, 1, 0, 0x500079e);
    func_02035754();
    data_02250438 = s0;
    data_02250434 = s1;
}
#pragma thumb reset

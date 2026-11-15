#include <nitro/types.h>

extern u8 data_ov001_0231b710[];
extern s32 data_02250458;
void func_02036854(void *p);
void func_0201e7cc(s32 a, s32 b);

#pragma thumb on
void func_ov001_022d4514(void) {
    func_02036854(data_ov001_0231b710);
    func_0201e7cc(0, data_02250458);
}
#pragma thumb reset

#include <nitro/types.h>

extern u32 data_02084de0[];

void func_02022270(u32 index, u32 value) {
    data_02084de0[index] = value | (index << 30);
}

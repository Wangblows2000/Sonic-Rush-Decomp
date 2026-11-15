#include <nitro/types.h>

void *func_02006abc(u32 a);
extern u16 data_022b65dc;
extern void *data_022b65e0[];

void func_02037e68(void *p) {
    if (func_02006abc(0) == p) return;
    data_022b65dc--;
    data_022b65e0[data_022b65dc] = p;
}

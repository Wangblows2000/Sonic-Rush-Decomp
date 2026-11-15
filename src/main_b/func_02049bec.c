#include <nitro/types.h>

void *func_02021264(void *f, s32 i);
void func_020212e0(void *f);
void func_02021310(void *f, void *path, void *arc);
void func_020257f8(void *p);
extern void *data_022c4488;
extern u8 data_0207a088;
extern void *data_ov000_02369ba0;
void func_02049bec(void) { u8 f[0x5c]; data_ov000_02369ba0 = data_022c4488; func_02021310(f, &data_0207a088, data_022c4488); func_020257f8(func_02021264(f, 0)); func_020212e0(f); }

#include <nitro/types.h>

void *func_02021264(void *f, s32 i);
void func_020212e0(void *f);
void func_02021310(void *f, void *path, void *arc);
void func_0202566c(void *p);
extern void *data_022c4488;
extern u8 data_0207a028;
void func_020493ac(void) { u8 f[0x5c]; if (data_022c4488 == NULL) return; func_02021310(f, &data_0207a028, data_022c4488); func_0202566c(func_02021264(f, 0)); func_020212e0(f); }

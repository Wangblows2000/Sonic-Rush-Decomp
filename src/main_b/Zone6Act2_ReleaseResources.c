#include <nitro/types.h>

void *func_02021264(void *f, s32 i);
void func_020212e0(void *f);
void func_02021310(void *f, void *path, void *arc);
void func_0202566c(void *p);
extern void *gZoneActArchive;
extern u8 data_0207a018;
void Zone6Act2_ReleaseResources(void) { u8 f[0x5c]; func_02021310(f, &data_0207a018, gZoneActArchive); func_0202566c(func_02021264(f, 0)); func_020212e0(f); }

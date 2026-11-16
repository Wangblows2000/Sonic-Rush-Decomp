#include <nitro/types.h>

void *func_02021264(void *f, s32 i);
void func_020212e0(void *f);
void func_02021310(void *f, void *path, void *arc);
void func_0202566c(void *p);
extern void *gZoneActArchive;
extern u8 data_0207a048;
void func_02049664(void) { u8 f[0x5c]; if (gZoneActArchive == NULL) return; func_02021310(f, &data_0207a048, gZoneActArchive); func_0202566c(func_02021264(f, 51)); func_020212e0(f); }

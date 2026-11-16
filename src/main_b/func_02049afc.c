#include <nitro/types.h>

void *func_02021264(void *f, s32 i);
void func_020212e0(void *f);
void func_02021310(void *f, void *path, void *arc);
void func_020257f8(void *p);
extern void *gZoneActArchive;
extern u8 data_0207a07c;
extern void *data_ov000_02369ba0;
void func_02049afc(void) { u8 f[0x5c]; data_ov000_02369ba0 = gZoneActArchive; func_02021310(f, &data_0207a07c, gZoneActArchive); func_020257f8(func_02021264(f, 0)); func_020212e0(f); }

#include <nitro/types.h>

void *func_02021264(void *f, s32 i);
void func_020212e0(void *f);
void func_02021310(void *f, void *path, void *arc);
void NNS_G3dResDefaultSetup(void *p);
extern void *gZoneActArchive;
extern u8 data_0207a058;
extern void *data_ov000_02369ba0;
void Zone6Act1_SetupResources(void) { u8 f[0x5c]; data_ov000_02369ba0 = gZoneActArchive; func_02021310(f, &data_0207a058, gZoneActArchive); NNS_G3dResDefaultSetup(func_02021264(f, 0)); func_020212e0(f); }

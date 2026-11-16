#include <nitro/types.h>

void *func_02021264(void *f, s32 i);
void func_020212e0(void *f);
void func_02021310(void *f, void *path, void *arc);
void NNS_G3dResDefaultSetup(void *p);
void func_ov000_0234dedc(void);
extern void *gActComArchive;
extern u8 data_0207a094;
void Stage_SetupCommonResources(void) { u8 f[0x5c]; func_ov000_0234dedc(); func_02021310(f, &data_0207a094, gActComArchive); NNS_G3dResDefaultSetup(func_02021264(f, 45)); NNS_G3dResDefaultSetup(func_02021264(f, 46)); func_020212e0(f); }

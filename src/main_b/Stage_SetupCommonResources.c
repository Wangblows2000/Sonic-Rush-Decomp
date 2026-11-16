#include <nitro/types.h>

void *NNS_FndGetArchiveFileByIndex(void *f, s32 i);
void NNS_FndUnmountArchive(void *f);
void NNS_FndMountArchive(void *f, void *path, void *arc);
void NNS_G3dResDefaultSetup(void *p);
void func_ov000_0234dedc(void);
extern void *gActComArchive;
extern u8 data_0207a094;
void Stage_SetupCommonResources(void) { u8 f[0x5c]; func_ov000_0234dedc(); NNS_FndMountArchive(f, &data_0207a094, gActComArchive); NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByIndex(f, 45)); NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByIndex(f, 46)); NNS_FndUnmountArchive(f); }

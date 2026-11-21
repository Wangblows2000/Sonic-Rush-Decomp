#include <nitro/types.h>

void *NNS_FndGetArchiveFileByIndex(void *f, s32 i);
void NNS_FndUnmountArchive(void *f);
void NNS_FndMountArchive(void *f, void *path, void *arc);
void NNS_G3dResDefaultSetup(void *p);
extern void *gZoneActArchive;
extern u8 data_0207a084;
void Zone1Act3_SetupResources(void) { u8 f[0x5c]; NNS_FndMountArchive(f, &data_0207a084, gZoneActArchive); NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByIndex(f, 51)); NNS_FndUnmountArchive(f); }

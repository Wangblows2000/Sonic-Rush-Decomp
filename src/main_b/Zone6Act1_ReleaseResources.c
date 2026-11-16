#include <nitro/types.h>

void *NNS_FndGetArchiveFileByIndex(void *f, s32 i);
void NNS_FndUnmountArchive(void *f);
void NNS_FndMountArchive(void *f, void *path, void *arc);
void NNS_G3dResDefaultRelease(void *p);
extern void *gZoneActArchive;
extern u8 data_0207a01c;
void Zone6Act1_ReleaseResources(void) { u8 f[0x5c]; NNS_FndMountArchive(f, &data_0207a01c, gZoneActArchive); NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByIndex(f, 0)); NNS_FndUnmountArchive(f); }

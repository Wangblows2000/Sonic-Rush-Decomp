#include <nitro/types.h>

void *NNS_FndGetArchiveFileByIndex(void *f, s32 i);
void NNS_FndUnmountArchive(void *f);
void NNS_FndMountArchive(void *f, void *path, void *arc);
void NNS_G3dResDefaultRelease(void *p);
extern void *gZoneActArchive;
extern u8 data_0207a048;
void Zone1Act3_ReleaseResources(void) { u8 f[0x5c]; if (gZoneActArchive == NULL) return; NNS_FndMountArchive(f, &data_0207a048, gZoneActArchive); NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByIndex(f, 51)); NNS_FndUnmountArchive(f); }

#include <nitro/types.h>

void *NNS_FndGetArchiveFileByIndex(void *f, s32 i);
void NNS_FndUnmountArchive(void *f);
void NNS_FndMountArchive(void *f, void *path, void *arc);
extern void *gZoneActArchive;
extern u8 data_0207a034;
void Zone3Act3_ReleaseResources(void) { u8 f[0x5c]; if (gZoneActArchive == NULL) return; NNS_FndMountArchive(f, &data_0207a034, gZoneActArchive); NNS_FndUnmountArchive(f); }

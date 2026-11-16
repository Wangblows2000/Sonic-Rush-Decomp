#include <nitro/types.h>

void *NNS_FndGetArchiveFileByIndex(void *f, s32 i);
void NNS_FndUnmountArchive(void *f);
void NNS_FndMountArchive(void *f, void *path, void *arc);
void NNS_G3dResDefaultSetup(void *p);
extern void *gZoneActArchive;
extern u8 data_0207a064;
extern void *data_ov000_02369ba0;
void Zone5Act1_SetupResources(void) { u8 f[0x5c]; data_ov000_02369ba0 = gZoneActArchive; NNS_FndMountArchive(f, &data_0207a064, gZoneActArchive); NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByIndex(f, 0)); NNS_FndUnmountArchive(f); }

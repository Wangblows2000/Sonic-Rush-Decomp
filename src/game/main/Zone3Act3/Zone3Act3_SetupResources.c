#include <nitro/types.h>

extern void *gZoneActArchive;
extern u8 data_0207a070;
void NNS_FndMountArchive(void *a, void *b, void *c);
void NNS_FndUnmountArchive(void *a);
void Zone3Act3_SetupResources(void) { u8 buf[0x5c]; NNS_FndMountArchive(buf, &data_0207a070, gZoneActArchive); NNS_FndUnmountArchive(buf); }

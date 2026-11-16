#include <nitro/types.h>

void *NNS_FndGetArchiveFileByIndex(void *f, s32 i);
void NNS_FndUnmountArchive(void *f);
void NNS_FndMountArchive(void *f, void *path, void *arc);
void NNS_G3dResDefaultRelease(void *p);
void func_ov000_0234ded8(void);
extern void *gActComArchive;
extern u8 data_0207a090;
void Stage_ReleaseCommonResources(void) { u8 f[0x5c]; func_ov000_0234ded8(); NNS_FndMountArchive(f, &data_0207a090, gActComArchive); NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByIndex(f, 45)); NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByIndex(f, 46)); NNS_FndUnmountArchive(f); }

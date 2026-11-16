#include <nitro/types.h>

typedef struct { u8 data[0x5c]; } UnkStruct_023029d4;
extern void *gExtraBossArchive;
extern u8 data_ov001_0231df18[];
extern void *data_ov001_0231f534;
extern void *data_ov001_0231f53c;
void NNS_FndMountArchive(UnkStruct_023029d4 *a, void *b, void *c);
void *NNS_FndGetArchiveFileByIndex(UnkStruct_023029d4 *a, s32 i);
void NNS_FndUnmountArchive(UnkStruct_023029d4 *a);

void ExtraBoss_LoadArchiveFiles24_25(void) {
    UnkStruct_023029d4 arc;
    NNS_FndMountArchive(&arc, data_ov001_0231df18, gExtraBossArchive);
    data_ov001_0231f534 = NNS_FndGetArchiveFileByIndex(&arc, 0x24);
    data_ov001_0231f53c = NNS_FndGetArchiveFileByIndex(&arc, 0x25);
    NNS_FndUnmountArchive(&arc);
}

#include <nitro/types.h>

typedef struct { u8 data[0x5c]; } UnkStruct_023029d4;
extern void *gExtraBossArchive;
extern u8 data_ov001_0231df74[];
extern void *data_ov001_0231f5ac;
extern void *data_ov001_0231f5b0[];
void NNS_FndMountArchive(UnkStruct_023029d4 *a, void *b, void *c);
void *NNS_FndGetArchiveFileByIndex(UnkStruct_023029d4 *a, s32 i);
void NNS_FndUnmountArchive(UnkStruct_023029d4 *a);

void func_ov001_023065f8(void) {
    UnkStruct_023029d4 arc;
    NNS_FndMountArchive(&arc, data_ov001_0231df74, gExtraBossArchive);
    data_ov001_0231f5ac = NNS_FndGetArchiveFileByIndex(&arc, 0x21);
    data_ov001_0231f5b0[0] = NNS_FndGetArchiveFileByIndex(&arc, 0x22);
    data_ov001_0231f5b0[1] = NNS_FndGetArchiveFileByIndex(&arc, 0x23);
    NNS_FndUnmountArchive(&arc);
}

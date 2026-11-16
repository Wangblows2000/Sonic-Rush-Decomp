#include <nitro/types.h>

typedef struct { u8 data[0x5c]; } UnkStruct_023029d4;
extern void *gExtraBossArchive;
extern u8 data_ov001_0231df20[];
extern void *data_ov001_0231f550[];
void NNS_FndMountArchive(UnkStruct_023029d4 *a, void *b, void *c);
void *NNS_FndGetArchiveFileByIndex(UnkStruct_023029d4 *a, s32 i);
void NNS_FndUnmountArchive(UnkStruct_023029d4 *a);

void func_ov001_02305368(void) {
    UnkStruct_023029d4 arc;
    s32 i;
    NNS_FndMountArchive(&arc, data_ov001_0231df20, gExtraBossArchive);
    for (i = 0; i < 0x16; i++) {
        data_ov001_0231f550[i] = NNS_FndGetArchiveFileByIndex(&arc, i + 0xb);
    }
    NNS_FndUnmountArchive(&arc);
}

#include <nitro/types.h>

typedef struct UnkStruct_02304518_a {
    u8 data[0x5c];
} UnkStruct_02304518_a;

extern void *gExtraBossArchive;
extern u8 data_ov001_0231df1c[];
extern void *data_ov001_0231f544;

void NNS_FndMountArchive(UnkStruct_02304518_a *arc, void *name, void *narc);
void *NNS_FndGetArchiveFileByIndex(UnkStruct_02304518_a *arc, s32 id);
void NNS_FndUnmountArchive(UnkStruct_02304518_a *arc);

void func_ov001_02304518(void) {
    UnkStruct_02304518_a arc;

    NNS_FndMountArchive(&arc, data_ov001_0231df1c, gExtraBossArchive);
    data_ov001_0231f544 = NNS_FndGetArchiveFileByIndex(&arc, 0x27);
    NNS_FndUnmountArchive(&arc);
}

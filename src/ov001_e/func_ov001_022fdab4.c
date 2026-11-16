#include <nitro/types.h>

typedef struct UnkStruct_022fdab4_a {
    u8 data[0x5c];
} UnkStruct_022fdab4_a;

extern void *gExtraBossArchive;
extern u8 data_ov001_0231d9ac[];
extern void *data_ov001_0231f320;

void NNS_FndMountArchive(UnkStruct_022fdab4_a *arc, void *name, void *narc);
void *NNS_FndGetArchiveFileByIndex(UnkStruct_022fdab4_a *arc, s32 id);
void NNS_FndUnmountArchive(UnkStruct_022fdab4_a *arc);

void func_ov001_022fdab4(void) {
    UnkStruct_022fdab4_a arc;

    NNS_FndMountArchive(&arc, data_ov001_0231d9ac, gExtraBossArchive);
    data_ov001_0231f320 = NNS_FndGetArchiveFileByIndex(&arc, 0x26);
    NNS_FndUnmountArchive(&arc);
}

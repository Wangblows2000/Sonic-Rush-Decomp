#include <nitro/types.h>

typedef struct { u8 data[0x5c]; } UnkStruct_023029d4;
typedef struct { u32 flags; s32 ids[6]; } UnkStruct_0231d6b4;
typedef struct { void *res[6]; } UnkStruct_0231f098;
extern void *gExtraBossArchive;
extern u8 data_ov001_0231d9a8[];
extern UnkStruct_0231d6b4 data_ov001_0231d6b4[];
extern UnkStruct_0231f098 data_ov001_0231f098[];
void NNS_FndMountArchive(UnkStruct_023029d4 *a, void *b, void *c);
void *NNS_FndGetArchiveFileByIndex(UnkStruct_023029d4 *a, s32 i);
void NNS_FndUnmountArchive(UnkStruct_023029d4 *a);
void NNS_G3dResDefaultSetup(void *p);

void ExtraBoss_Unk_ov001_022fd564(void) {
    UnkStruct_023029d4 arc;
    s32 i, j;
    UnkStruct_0231d6b4 *d;
    UnkStruct_0231f098 *r;
    NNS_FndMountArchive(&arc, data_ov001_0231d9a8, gExtraBossArchive);
    d = data_ov001_0231d6b4;
    r = data_ov001_0231f098;
    for (i = 0; i < 0x1b; i++) {
        for (j = 0; j < 6; j++) {
            if (d->ids[j] != -1) {
                r->res[j] = NNS_FndGetArchiveFileByIndex(&arc, d->ids[j]);
            }
        }
        if (d->flags & 1) {
            NNS_G3dResDefaultSetup(r->res[0]);
        }
        d++;
        r++;
    }
    NNS_FndUnmountArchive(&arc);
}

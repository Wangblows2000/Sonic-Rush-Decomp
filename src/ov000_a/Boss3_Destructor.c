#include <nitro/types.h>

typedef struct UnkStruct_022da3f4_b {
    u8 pad[0x23c];
    u8 unk23C[0x138];
} UnkStruct_022da3f4_b;

typedef struct UnkStruct_022da3f4 {
    u8 pad[0x10];
    UnkStruct_022da3f4_b *unk10;
} UnkStruct_022da3f4;

typedef struct { u8 data[0x5c]; } UnkArc_022da3f4;

extern void *gZoneActArchive;
extern char data_ov000_023654ac[];
extern char data_ov000_023654b0[];
extern char data_ov000_023654c4[];
extern char data_ov000_023654d8[];
extern char data_ov000_023654ec[];
extern char data_ov000_02365500[];
extern char data_ov000_02365514[];
extern char data_ov000_02365528[];

BOOL NNS_FndMountArchive(UnkArc_022da3f4 *arc, const char *name, void *narc);
void *NNS_FndGetArchiveFileByName(const char *path);
void NNS_G3dResDefaultRelease(void *p);
BOOL NNS_FndUnmountArchive(UnkArc_022da3f4 *arc);
void Model_Release(void *a);
void MapObject_Destructor(UnkStruct_022da3f4 *t);

void Boss3_Destructor(UnkStruct_022da3f4 *t) {
    UnkArc_022da3f4 arc;
    UnkStruct_022da3f4_b *s = t->unk10;
    NNS_FndMountArchive(&arc, data_ov000_023654ac, gZoneActArchive);
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_023654b0));
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_023654c4));
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_023654d8));
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_023654ec));
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_02365500));
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_02365514));
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_02365528));
    NNS_FndUnmountArchive(&arc);
    Model_Release(s->unk23C);
    MapObject_Destructor(t);
}

#include <nitro/types.h>

typedef struct {
    u8 data[0x138];
} UnkStruct_022dec70_M;

typedef struct {
    u8 pad[0x228];
    UnkStruct_022dec70_M mdl[7];
    u8 padA90[0xABC - 0x228 - 7 * 0x138];
    void *unkABC;
    void *unkAC0;
    u8 padAC4[0xAD4 - 0xAC4];
    void *unkAD4;
    u8 padAD8[0xAE0 - 0xAD8];
    void *unkAE0;
    u8 padAE4[4];
    void *unkAE8;
} UnkStruct_022dec70_B;

typedef struct { u8 data[0x5c]; } UnkArc_022dec70;

extern void *gZoneActArchive;
extern char data_ov000_02365728[];
extern char data_ov000_0236572c[];
extern char data_ov000_02365740[];
extern char data_ov000_0236575c[];
extern UnkStruct_022dec70_B *gBoss4;

BOOL NNS_FndMountArchive(UnkArc_022dec70 *arc, const char *name, void *narc);
void *NNS_FndGetArchiveFileByName(const char *path);
void NNS_G3dResDefaultRelease(void *p);
BOOL NNS_FndUnmountArchive(UnkArc_022dec70 *arc);
void Model_Release(void *a);
void MapObject_Destructor(void *t);

void Boss4_Destructor(void *t) {
    UnkArc_022dec70 arc;
    UnkStruct_022dec70_B *s = gBoss4;
    s32 i;

    for (i = 0; i < 7; i++)
        Model_Release(&s->mdl[i]);
    NNS_FndMountArchive(&arc, data_ov000_02365728, gZoneActArchive);
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_0236572c));
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_02365740));
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_0236575c));
    NNS_FndUnmountArchive(&arc);
    NNS_G3dResDefaultRelease(s->unkABC);
    NNS_G3dResDefaultRelease(s->unkAC0);
    NNS_G3dResDefaultRelease(s->unkAD4);
    NNS_G3dResDefaultRelease(s->unkAE0);
    NNS_G3dResDefaultRelease(s->unkAE8);
    MapObject_Destructor(t);
    gBoss4 = NULL;
}

#include <nitro/types.h>

typedef struct { u8 data[0x5c]; } UnkArc_022d4d28;

typedef struct UnkStruct_022d4d28_a {
    u8 pad[0x138];
} UnkStruct_022d4d28_a;

typedef struct UnkStruct_022d4d28_b {
    u8 pad[0x210];
    s32 unk210;
    UnkStruct_022d4d28_a unk214[11];
    u8 pad1[0x1450 - 0xf7c];
    s32 unk1450;
    s32 unk1454;
    s32 unk1458;
} UnkStruct_022d4d28_b;

typedef struct UnkStruct_022d4d28 {
    u8 pad[0x10];
    UnkStruct_022d4d28_b *unk10;
} UnkStruct_022d4d28;

extern void *gZoneActArchive;
extern char data_ov000_0236528c[];
extern char data_ov000_02365290[];
extern char data_ov000_023652a8[];
extern char data_ov000_023652c0[];
extern char data_ov000_023652d8[];
extern char data_ov000_023652f0[];
extern char data_ov000_02365308[];
extern char data_ov000_0236531c[];
extern char data_ov000_02365334[];
extern UnkStruct_022d4d28_b *gBoss2;

void Model_Release(UnkStruct_022d4d28_a *a);
BOOL NNS_FndMountArchive(UnkArc_022d4d28 *arc, const char *name, void *narc);
void *NNS_FndGetArchiveFileByName(const char *path);
void NNS_G3dResDefaultRelease(void *p);
BOOL NNS_FndUnmountArchive(UnkArc_022d4d28 *arc);
void Task_Destroy(s32 a);
void MapObject_Destructor(UnkStruct_022d4d28 *s);
void ScreenShake_Start(s32 a);
void Sound_FreeHandle(s32 a);

void Boss2_Destructor(UnkStruct_022d4d28 *t) {
    UnkStruct_022d4d28_b *s = t->unk10;
    UnkArc_022d4d28 arc;
    s32 i;
    for (i = 0; i < 11; i++)
        Model_Release(&s->unk214[i]);
    NNS_FndMountArchive(&arc, data_ov000_0236528c, gZoneActArchive);
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_02365290));
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_023652a8));
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_023652c0));
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_023652d8));
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_023652f0));
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_02365308));
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_0236531c));
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_02365334));
    NNS_FndUnmountArchive(&arc);
    Task_Destroy(s->unk210);
    MapObject_Destructor(t);
    ScreenShake_Start(0);
    Sound_FreeHandle(s->unk1450);
    Sound_FreeHandle(s->unk1454);
    Sound_FreeHandle(s->unk1458);
    gBoss2 = NULL;
}

#include <nitro/types.h>

typedef struct UnkStruct_022edee8_b {
    u8 pad[0x24c];
    u8 unk24C[0x388 - 0x24c];
    s32 unk388;
} UnkStruct_022edee8_b;

typedef struct UnkStruct_022edee8 {
    u8 pad[0x10];
    UnkStruct_022edee8_b *unk10;
} UnkStruct_022edee8;

typedef struct { u8 data[0x5c]; } UnkArc_022edee8;

typedef struct {
    u8 pad0[0x20];
    union {
        u16 raw;
        struct {
            u16 b0 : 1;
            u16 b1 : 5;
            u16 b6 : 2;
            u16 b8 : 8;
        } bits;
    } unk20;
} UnkStruct_02087d04;

extern void *gZoneActArchive;
extern UnkStruct_02087d04 gDisplaySub;
extern char data_ov000_023659c4[];
extern char data_ov000_023659c8[];
extern char data_ov000_023659dc[];
extern char data_ov000_023659f4[];
extern char data_ov000_02365a08[];
extern char data_ov000_02365a20[];
extern char data_ov000_02365a38[];
extern char data_ov000_02365a50[];
extern char data_ov000_02365a64[];
extern char data_ov000_02365a80[];
extern char data_ov000_02365a9c[];
extern char data_ov000_02365ab8[];
extern char data_ov000_02365acc[];

BOOL NNS_FndMountArchive(UnkArc_022edee8 *arc, const char *name, void *narc);
void *NNS_FndGetArchiveFileByName(const char *path);
void NNS_G3dResDefaultRelease(void *p);
BOOL NNS_FndUnmountArchive(UnkArc_022edee8 *arc);
void Sound_FreeHandle(s32 a);
void Model_Release(void *a);
void MapObject_Destructor(UnkStruct_022edee8 *t);

void Boss7_Destructor(UnkStruct_022edee8 *t) {
    UnkArc_022edee8 arc;
    UnkStruct_022edee8_b *s = t->unk10;
    NNS_FndMountArchive(&arc, data_ov000_023659c4, gZoneActArchive);
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_023659c8));
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_023659dc));
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_023659f4));
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_02365a08));
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_02365a20));
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_02365a38));
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_02365a50));
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_02365a64));
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_02365a80));
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_02365a9c));
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_02365ab8));
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_02365acc));
    NNS_FndUnmountArchive(&arc);
    gDisplaySub.unk20.bits.b6 = 0;
    Sound_FreeHandle(s->unk388);
    Model_Release(s->unk24C);
    MapObject_Destructor(t);
}

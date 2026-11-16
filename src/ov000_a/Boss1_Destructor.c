#include <nitro/types.h>

typedef struct UnkStruct_022cbcf4 {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_022cbcf4;

extern void *gZoneActArchive;
extern u8 data_ov000_02364fbc[];
extern u8 data_ov000_02364fc0[];
extern u8 data_ov000_02364fd4[];
extern u8 data_ov000_02364ff0[];
extern u8 data_ov000_0236500c[];
extern u8 data_ov000_02365024[];
extern u8 data_ov000_0236503c[];

void NNS_FndMountArchive(void *ctx, void *a, void *b);
void NNS_FndUnmountArchive(void *ctx);
void *NNS_FndGetArchiveFileByName(void *a);
void NNS_G3dResDefaultRelease(void *a);
BOOL func_ov000_022cc6d4(void);
void Model_Release(void *p);
void MapObject_Destructor(UnkStruct_022cbcf4 *s);

void Boss1_Destructor(UnkStruct_022cbcf4 *s) {
    u8 ctx[0x5c];
    u8 *t = s->unk10;
    NNS_FndMountArchive(ctx, data_ov000_02364fbc, gZoneActArchive);
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_02364fc0));
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_02364fd4));
    NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_02364ff0));
    if (func_ov000_022cc6d4()) {
        NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_0236500c));
        NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_02365024));
        NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByName(data_ov000_0236503c));
    }
    NNS_FndUnmountArchive(ctx);
    Model_Release(t + 0x23c);
    MapObject_Destructor(s);
}

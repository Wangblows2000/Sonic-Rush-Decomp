#include <nitro/types.h>

typedef struct { void *ptr; u16 count; } UnkRef_020687b8;
typedef struct { u8 data[0x5c]; } UnkArc_020687b8;

extern char data_0207cf98[];
extern char data_0207cfa0[];
extern char data_0207cfa4[];
extern char data_0207cfac[];
extern u32 gSystemFlags;
extern u32 data_02087cc4;

char *strcat(char *dst, const char *src);
BOOL NNS_FndMountArchive(UnkArc_020687b8 *arc, const char *name, void *narc);
void *NNS_FndGetArchiveFileByName(const char *path);
BOOL NNS_FndUnmountArchive(UnkArc_020687b8 *arc);
void *File_Load(const char *path, s32 b);
void VBlankIntrWait(void);

void *ObjData_Load(UnkRef_020687b8 *ref, const char *name, void *narc) {
    char path[0x28] = {0};
    void *p;

    if (ref == NULL) {
        if (narc != NULL) {
            UnkArc_020687b8 arc;
            strcat(path, data_0207cf98);
            strcat(path, name);
            NNS_FndMountArchive(&arc, data_0207cfa0, narc);
            p = NNS_FndGetArchiveFileByName(path);
            NNS_FndUnmountArchive(&arc);
        } else {
            p = File_Load(name, 0);
        }
        return p;
    }
    if (ref->ptr == NULL) {
        if (narc != NULL) {
            UnkArc_020687b8 arc;
            strcat(path, data_0207cfa4);
            strcat(path, name);
            NNS_FndMountArchive(&arc, data_0207cfac, narc);
            ref->ptr = NNS_FndGetArchiveFileByName(path);
            NNS_FndUnmountArchive(&arc);
            if (ref->ptr == NULL)
                return NULL;
            ref->count++;
            ref->count |= 0x8000;
        } else {
            u32 f = gSystemFlags;
            if (f & 0x400)
                data_02087cc4 &= 0x400;
            if (f & 0x800)
                data_02087cc4 &= 0x800;
            VBlankIntrWait();
            ref->ptr = File_Load(name, 0);
            ref->count++;
        }
    } else {
        ref->count++;
    }
    return ref->ptr;
}

#include <nitro/types.h>

typedef struct { void *ptr; u16 count; } UnkRef_020687b8;
typedef struct { u8 data[0x5c]; } UnkArc_020687b8;

extern char data_0207cf98[];
extern char data_0207cfa0[];
extern char data_0207cfa4[];
extern char data_0207cfac[];
extern u32 data_02087cb8;
extern u32 data_02087cc4;

char *func_0202c474(char *dst, const char *src);
BOOL func_02021310(UnkArc_020687b8 *arc, const char *name, void *narc);
void *func_0202128c(const char *path);
BOOL func_020212e0(UnkArc_020687b8 *arc);
void *func_02033fbc(const char *path, s32 b);
void VBlankIntrWait(void);

void *func_020687b8(UnkRef_020687b8 *ref, const char *name, void *narc) {
    char path[0x28] = {0};
    void *p;

    if (ref == NULL) {
        if (narc != NULL) {
            UnkArc_020687b8 arc;
            func_0202c474(path, data_0207cf98);
            func_0202c474(path, name);
            func_02021310(&arc, data_0207cfa0, narc);
            p = func_0202128c(path);
            func_020212e0(&arc);
        } else {
            p = func_02033fbc(name, 0);
        }
        return p;
    }
    if (ref->ptr == NULL) {
        if (narc != NULL) {
            UnkArc_020687b8 arc;
            func_0202c474(path, data_0207cfa4);
            func_0202c474(path, name);
            func_02021310(&arc, data_0207cfac, narc);
            ref->ptr = func_0202128c(path);
            func_020212e0(&arc);
            if (ref->ptr == NULL)
                return NULL;
            ref->count++;
            ref->count |= 0x8000;
        } else {
            u32 f = data_02087cb8;
            if (f & 0x400)
                data_02087cc4 &= 0x400;
            if (f & 0x800)
                data_02087cc4 &= 0x800;
            VBlankIntrWait();
            ref->ptr = func_02033fbc(name, 0);
            ref->count++;
        }
    } else {
        ref->count++;
    }
    return ref->ptr;
}

#include <nitro/types.h>

typedef struct {
    u8 data[0x138];
} UnkStruct_022e6410_M;

typedef struct {
    u8 pad[0x210];
    UnkStruct_022e6410_M mdl[9];
    u8 padD0C[0xD0C - 0x210 - 9 * 0x138];
    void *unkD0C[4];
} UnkStruct_022e6410_B;

typedef struct {
    u8 pad[0x10];
    UnkStruct_022e6410_B *unk10;
} UnkStruct_022e6410;

typedef struct {
    s16 unk0;
    u8 pad2[6];
} UnkStruct_022e6410_T;

typedef struct { u8 data[0x5c]; } UnkArc_022e6410;

extern void *gZoneActArchive;
extern UnkStruct_022e6410_T data_ov000_02363864[];
extern char data_ov000_02365908[];
extern char data_ov000_0236590c[];
extern char data_ov000_02365920[];
extern char data_ov000_02365934[];
extern char data_ov000_02365950[];
extern void *gBoss6;

BOOL func_02021310(UnkArc_022e6410 *arc, const char *name, void *narc);
void *func_0202128c(const char *path);
void func_0202566c(void *p);
BOOL func_020212e0(UnkArc_022e6410 *arc);
void Model_Release(void *a);
void MapObject_Destructor(UnkStruct_022e6410 *t);

void Boss6_Destructor(UnkStruct_022e6410 *t) {
    UnkArc_022e6410 arc;
    UnkStruct_022e6410_B *s = t->unk10;
    s32 i;

    for (i = 0; i < 9; i++)
        Model_Release(&s->mdl[i]);
    for (i = 0; i < 4; i++) {
        if (data_ov000_02363864[i].unk0 != -1)
            func_0202566c(s->unkD0C[i]);
    }
    func_02021310(&arc, data_ov000_02365908, gZoneActArchive);
    func_0202566c(func_0202128c(data_ov000_0236590c));
    func_0202566c(func_0202128c(data_ov000_02365920));
    func_0202566c(func_0202128c(data_ov000_02365934));
    func_0202566c(func_0202128c(data_ov000_02365950));
    func_020212e0(&arc);
    MapObject_Destructor(t);
    gBoss6 = NULL;
}

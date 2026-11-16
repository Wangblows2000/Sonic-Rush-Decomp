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

BOOL func_02021310(UnkArc_022da3f4 *arc, const char *name, void *narc);
void *func_0202128c(const char *path);
void func_0202566c(void *p);
BOOL func_020212e0(UnkArc_022da3f4 *arc);
void Model_Release(void *a);
void func_ov000_022fb9c8(UnkStruct_022da3f4 *t);

void func_ov000_022da3f4(UnkStruct_022da3f4 *t) {
    UnkArc_022da3f4 arc;
    UnkStruct_022da3f4_b *s = t->unk10;
    func_02021310(&arc, data_ov000_023654ac, gZoneActArchive);
    func_0202566c(func_0202128c(data_ov000_023654b0));
    func_0202566c(func_0202128c(data_ov000_023654c4));
    func_0202566c(func_0202128c(data_ov000_023654d8));
    func_0202566c(func_0202128c(data_ov000_023654ec));
    func_0202566c(func_0202128c(data_ov000_02365500));
    func_0202566c(func_0202128c(data_ov000_02365514));
    func_0202566c(func_0202128c(data_ov000_02365528));
    func_020212e0(&arc);
    Model_Release(s->unk23C);
    func_ov000_022fb9c8(t);
}

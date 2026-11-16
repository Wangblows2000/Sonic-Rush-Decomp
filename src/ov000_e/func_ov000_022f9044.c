#include <nitro/types.h>

typedef struct {
    u8 pad[0x2c];
} UnkStruct_Coll;

typedef struct {
    u8 pad[0x20C];
    UnkStruct_Coll unk20C[7];
} UnkStruct_022f9044;

void ObjRect_SetBox3D(UnkStruct_Coll *c, s16 a, s16 b, s16 d, s16 e, s16 f, s16 g);

void func_ov000_022f9044(UnkStruct_022f9044 *s) {
    UnkStruct_Coll *c;
    ObjRect_SetBox3D(&s->unk20C[0], -0x24, -0x28, 0x24, 8, -0x7d00, 0x7d00);
    ObjRect_SetBox3D(&s->unk20C[1], -0x3c, -0x30, 0x24, 0x38, -0x7d00, 0x7d00);
    ObjRect_SetBox3D(&s->unk20C[2], -0x78, -0x30, 0x10, 0x38, -0x7d00, 0x7d00);
    c = &s->unk20C[3];
    ObjRect_SetBox3D(c, -0x3c, -0x30, 0x24, 0x38, -0x7d00, 0x7d00);
    c = &s->unk20C[4];
    ObjRect_SetBox3D(c, -0x28, -0x30, 0x38, 0x38, -0x7d00, 0x7d00);
    c = &s->unk20C[5];
    ObjRect_SetBox3D(c, -0x14, -0x30, 0x74, 0x38, -0x7d00, 0x7d00);
    c = &s->unk20C[6];
    ObjRect_SetBox3D(c, -0x28, -0x30, 0x38, 0x38, -0x7d00, 0x7d00);
}

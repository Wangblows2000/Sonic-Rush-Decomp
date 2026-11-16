#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    s32 sx;
    s32 sy;
    s32 sz;
    u8 rot[0x24];
    s32 x;
    s32 y;
    s32 z;
} UnkObj_022d58ac;

typedef struct {
    u8 pad0[0x14c];
    UnkObj_022d58ac obj;
    u8 pad68[0x1e0 - 0x14c - sizeof(UnkObj_022d58ac)];
    s32 unka8;
} UnkStruct_022d58ac;

extern s16 data_0206e470[];

void Model_Animate(UnkObj_022d58ac *o);
void MTX_RotY33_(void *m, s32 s, s32 c);
void TitleDemo_Unk_ov001_022d5694(UnkStruct_022d58ac *p, s32 a, s32 b);
void NNS_G3dGeBufferOP_N(s32 a, s32 *b, s32 c);
void Model_Draw(UnkObj_022d58ac *o);

#pragma thumb on
void TitleDemo_Unk_ov001_022d58ac(UnkStruct_022d58ac *p, s32 x, s32 y) {
    UnkObj_022d58ac *o = &p->obj;
    s32 v;

    Model_Animate(o);
    o->x = x << 12;
    o->y = y << 12;
    o->z = -0x50000;
    o->sx = 0xd200;
    o->sy = 0xd200;
    o->sz = 0xd200;
    MTX_RotY33_(o->rot, data_0206e470[0], data_0206e470[1]);
    TitleDemo_Unk_ov001_022d5694(p, p->unka8, 2);
    NNS_G3dGeBufferOP_N(0x11, NULL, 0);
    Model_Draw(o);
    v = 1;
    NNS_G3dGeBufferOP_N(0x12, &v, 1);
}
#pragma thumb reset

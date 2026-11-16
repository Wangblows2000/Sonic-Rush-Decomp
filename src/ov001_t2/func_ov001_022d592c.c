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
} UnkObj_022d592c;

typedef struct {
    u8 pad0[0x14];
    UnkObj_022d592c obj;
    u8 pad68[0xa8 - 0x14 - sizeof(UnkObj_022d592c)];
    s32 unka8;
} UnkStruct_022d592c;

extern s16 data_0206e470[];

void Model_Animate(UnkObj_022d592c *o);
void MTX_RotY33_(void *m, s32 s, s32 c);
void func_ov001_022d5694(UnkStruct_022d592c *p, s32 a, s32 b);
void func_02025268(s32 a, s32 *b, s32 c);
void Model_Draw(UnkObj_022d592c *o);

#pragma thumb on
void func_ov001_022d592c(UnkStruct_022d592c *p, s32 x, s32 y) {
    UnkObj_022d592c *o = &p->obj;
    s32 v;

    Model_Animate(o);
    o->x = x << 12;
    o->y = y << 12;
    o->z = -0x78000;
    o->sx = -0xe000;
    o->sy = 0xe000;
    o->sz = 0xe000;
    MTX_RotY33_(o->rot, data_0206e470[0], data_0206e470[1]);
    func_ov001_022d5694(p, p->unka8, 1);
    func_02025268(0x11, NULL, 0);
    Model_Draw(o);
    v = 1;
    func_02025268(0x12, &v, 1);
}
#pragma thumb reset

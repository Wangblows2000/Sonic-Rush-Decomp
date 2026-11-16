#include <ov000_d/mdl.h>

typedef struct {
    u8 pad0[0xB68];
    UnkStruct_MdlRender r;
    u8 pad[0xCAC - 0xB68 - sizeof(UnkStruct_MdlRender)];
    s32 ang;
} UnkStruct_0236011c;

void Model_Animate(UnkStruct_MdlRender *r);
void func_020221fc(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void func_02002360(s16 *a, s16 *b);
void func_02022284(s32 idx, s32 x, s32 y, s32 z);
void func_02022270(s32 idx, s32 col);
void func_0202224c(s32 a, s32 b, s32 c);
void func_02025dac(void *m, s32 a, s32 b);
void func_02025e1c(void *m, s32 a, s32 b);

#pragma thumb on

void func_ov000_0236011c(UnkStruct_0236011c *w, s32 x, s32 y, BOOL flip) {
    s16 v[3];
    u32 one;
    UnkStruct_MdlRender *r = &w->r;
    void *m = w->r.unk94;
    s32 idx;

    Model_Animate(r);
    x -= 0x80;
    r->trans.x = x << 12;
    y -= 0x60;
    r->trans.y = -y << 12;
    r->trans.z = -0x64000;
    if (flip) {
        r->scale.x = -0xd000;
    } else {
        r->scale.x = 0xd000;
    }
    r->scale.y = 0xd000;
    r->scale.z = 0xd000;
    idx = (s32)((u16)w->ang & 0xffff) >> 4;
    MTX_RotY33_(&r->rot, MDL_SIN_IDX(idx), MDL_COS_IDX(idx));
    func_020221fc(1, 0, 3, 0, 0x1f, 0);
    if (flip) {
        v[0] = 0xfff;
    } else {
        v[0] = -0x1000;
    }
    v[1] = -0x1000;
    v[2] = -0x2000;
    func_02002360(v, v);
    func_02022284(0, v[0], v[1], v[2]);
    func_02022270(0, 0x7fff);
    func_0202224c(0x7fff, 0x6f7b, 1);
    func_02025dac(m, 0, 0xf);
    func_02025e1c(m, 0, 0x40);
    func_02025e1c(m, 0, 0x80);
    func_02025dac(m, 0, 0xc0);
    func_02025268(0x11, NULL, 0);
    Model_Draw(r);
    one = 1;
    func_02025268(0x12, &one, 1);
}

#pragma thumb reset

#include <ov000_d/mdl.h>

typedef struct {
    u8 pad0[0xB68];
    UnkStruct_MdlRender r;
    u8 pad[0xCAC - 0xB68 - sizeof(UnkStruct_MdlRender)];
    s32 ang;
} UnkStruct_0236011c;

void Model_Animate(UnkStruct_MdlRender *r);
void NNS_G3dGlbPolygonAttr(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void VEC_Fx16Normalize(s16 *a, s16 *b);
void func_02022284(s32 idx, s32 x, s32 y, s32 z);
void func_02022270(s32 idx, s32 col);
void NNS_G3dGlbMaterialColorDiffAmb(s32 a, s32 b, s32 c);
void NNSi_G3dModifyPolygonAttrMask(void *m, s32 a, s32 b);
void NNSi_G3dModifyMatFlag(void *m, s32 a, s32 b);

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
    NNS_G3dGlbPolygonAttr(1, 0, 3, 0, 0x1f, 0);
    if (flip) {
        v[0] = 0xfff;
    } else {
        v[0] = -0x1000;
    }
    v[1] = -0x1000;
    v[2] = -0x2000;
    VEC_Fx16Normalize(v, v);
    func_02022284(0, v[0], v[1], v[2]);
    func_02022270(0, 0x7fff);
    NNS_G3dGlbMaterialColorDiffAmb(0x7fff, 0x6f7b, 1);
    NNSi_G3dModifyPolygonAttrMask(m, 0, 0xf);
    NNSi_G3dModifyMatFlag(m, 0, 0x40);
    NNSi_G3dModifyMatFlag(m, 0, 0x80);
    NNSi_G3dModifyPolygonAttrMask(m, 0, 0xc0);
    NNS_G3dGeBufferOP_N(0x11, NULL, 0);
    Model_Draw(r);
    one = 1;
    NNS_G3dGeBufferOP_N(0x12, &one, 1);
}

#pragma thumb reset

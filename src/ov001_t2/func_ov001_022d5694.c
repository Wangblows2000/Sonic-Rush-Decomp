#include <nitro/types.h>

typedef struct { s16 x; s16 y; s16 z; } UnkVec16_022d5694;

typedef struct {
    u8 pad0[0xa8];
    void *obj;
} UnkStruct_022d5694;

void NNS_G3dGlbPolygonAttr(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void VEC_Fx16Normalize(const UnkVec16_022d5694 *src, UnkVec16_022d5694 *dst);
void NNS_G3dGlbLightVector(s32 i, s32 x, s32 y, s32 z);
void NNS_G3dGlbMaterialColorDiffAmb(u16 a, u16 b, s32 c);
void NNSi_G3dModifyPolygonAttrMask(void *o, s32 a, s32 b);
void NNSi_G3dModifyMatFlag(void *o, s32 a, s32 b);

#pragma thumb on
void func_ov001_022d5694(UnkStruct_022d5694 *p, s32 unused, s32 mode) {
    void *obj = p->obj;
    UnkVec16_022d5694 v;

    NNS_G3dGlbPolygonAttr(1, 0, mode, 0, 0x1f, 0);
    if (mode == 1) {
        v.x = 0xfff;
        v.y = -0x1000;
        v.z = -0x2000;
    } else {
        v.x = -0x1000;
        v.y = -0x1000;
        v.z = -0x2000;
    }
    VEC_Fx16Normalize(&v, &v);
    NNS_G3dGlbLightVector(0, v.x, v.y, v.z);
    NNS_G3dGlbMaterialColorDiffAmb(0x7fff, 0x6f7b, 1);
    NNSi_G3dModifyPolygonAttrMask(obj, 0, 0xf);
    NNSi_G3dModifyMatFlag(obj, 0, 0x40);
    NNSi_G3dModifyMatFlag(obj, 0, 0x80);
    NNSi_G3dModifyPolygonAttrMask(obj, 0, 0xc0);
}
#pragma thumb reset

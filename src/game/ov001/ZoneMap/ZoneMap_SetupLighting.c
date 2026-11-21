/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 NNS_G3dGlbPolygonAttr();
s32 NNS_G3dGlbLightVector();
s32 NNS_G3dGlbLightColor();
s32 NNS_G3dGlbMaterialColorDiffAmb();
s32 NNSi_G3dModifyPolygonAttrMask();
s32 NNSi_G3dModifyMatFlag();

void ZoneMap_SetupLighting(s32 p0, s32 p1) {
    s32 v0 = (s32)p1;
    if ((s32)p1 < 0) {
        v0 = 0;
    }
    s32 v1 = v0;
    if (v0 >= 0x20) {
        v1 = 0x1f;
    }
    NNS_G3dGlbPolygonAttr(1, 0, 3, 0, v1, 0);
    NNS_G3dGlbLightVector(0, -4096, -4096, -4096);
    NNS_G3dGlbLightVector(1, 0xfff, -4096, -4096);
    NNS_G3dGlbLightVector(2, 0xfff, 0xfff, -4096);
    NNS_G3dGlbLightVector(3, -4096, 0xfff, -4096);
    NNS_G3dGlbLightColor(0, 0x7fff);
    NNS_G3dGlbLightColor(1, 0x2108);
    NNS_G3dGlbLightColor(2, 0x2108);
    NNS_G3dGlbLightColor(3, 0);
    NNS_G3dGlbMaterialColorDiffAmb(0x7fff, 0x5ef7, 1);
    NNSi_G3dModifyPolygonAttrMask(p0, 0, 0xf);
    NNSi_G3dModifyMatFlag(p0, 0, 0x40);
    NNSi_G3dModifyMatFlag(p0, 0, 0x80);
    NNSi_G3dModifyPolygonAttrMask(p0, 0, 0x1f0000);
}

#pragma thumb reset

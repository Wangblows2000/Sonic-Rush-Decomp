/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 NNS_G3dGlbPolygonAttr();
s32 VEC_Fx16Normalize();
s32 NNS_G3dGlbLightVector();
s32 NNS_G3dGlbLightColor();
s32 NNS_G3dGlbMaterialColorDiffAmb();
s32 NNSi_G3dModifyPolygonAttrMask();
s32 NNSi_G3dModifyMatFlag();

void PlayerSelectMenu_SetupModelLighting(s32 p0, s32 p1, s32 p2) {
    NNS_G3dGlbPolygonAttr(1, 0, p1, 0, 0x1f, 0);
    VEC_Fx16Normalize(p2, p2);
    NNS_G3dGlbLightVector(0, *(s16 *)((u8 *)p2 + 0), *(s16 *)((u8 *)p2 + 2), *(s16 *)((u8 *)p2 + 4));
    NNS_G3dGlbLightColor(0, 0x7fff);
    NNS_G3dGlbMaterialColorDiffAmb(0x7fff, 0x6f7b, 1);
    NNSi_G3dModifyPolygonAttrMask(p0, 0, 0xf);
    NNSi_G3dModifyMatFlag(p0, 0, 0x40);
    NNSi_G3dModifyMatFlag(p0, 0, 0x80);
    NNSi_G3dModifyPolygonAttrMask(p0, 0, 0xc0);
}

#pragma thumb reset

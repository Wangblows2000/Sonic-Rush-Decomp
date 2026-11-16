#include <nitro/types.h>

typedef struct { fx32 frame; fx32 ratio; } UnkAnmObj_02031a68;
typedef struct {
    u32 type;
    u8 unk4[0x8C];
    u8 renderObj[4];
    void *resMdl;
    u8 unk98[0x4C];
    UnkAnmObj_02031a68 *anm[5];
    UnkAnmObj_02031a68 *anm2[5];
    u8 flags[5];
} UnkObj_02031a68;

extern u8 gSystemHeapAllocator[];

void NNS_G3dRenderObjRemoveAnmObj(void *renderObj, void *anm);
void func_02025b38(void *alloc, void *anm);
void *NNS_G3dGetAnmByIdx(void *res, u32 idx);
UnkAnmObj_02031a68 *NNS_G3dAllocAnmObj(void *alloc, void *anm, void *mdl);
void NNS_G3dAnmObjInit(UnkAnmObj_02031a68 *obj, void *res, void *mdl, void *tex);
void NNS_G3dRenderObjAddAnmObj(void *renderObj, UnkAnmObj_02031a68 *anm);

void Model_SetAnimation(UnkObj_02031a68 *o, u32 slot, void *res, u32 idx, void *tex) {
    void *a;
    if (o->anm2[slot] != NULL) {
        NNS_G3dRenderObjRemoveAnmObj(o->renderObj, o->anm2[slot]);
        func_02025b38(gSystemHeapAllocator, o->anm2[slot]);
        o->anm2[slot] = NULL;
    }
    switch (slot) {
    case 0:
    case 1:
    case 4:
        if (o->flags[slot] & 0x40) {
            o->flags[slot] &= ~0x80;
            o->anm2[slot] = o->anm[slot];
            o->anm[slot] = NULL;
            break;
        }
    case 2:
    case 3:
        if (o->anm[slot] != NULL) {
            NNS_G3dRenderObjRemoveAnmObj(o->renderObj, o->anm[slot]);
            func_02025b38(gSystemHeapAllocator, o->anm[slot]);
            o->anm[slot] = NULL;
        }
        break;
    }
    a = NNS_G3dGetAnmByIdx(res, idx);
    o->anm[slot] = NNS_G3dAllocAnmObj(gSystemHeapAllocator, a, o->resMdl);
    NNS_G3dAnmObjInit(o->anm[slot], a, o->resMdl, tex);
    NNS_G3dRenderObjAddAnmObj(o->renderObj, o->anm[slot]);
    if (o->anm2[slot] != NULL) {
        o->anm[slot]->ratio = 0;
        o->anm2[slot]->ratio = 0x1000;
    }
    o->flags[slot] &= ~3;
}

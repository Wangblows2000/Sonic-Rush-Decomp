#include <nitro/types.h>

typedef struct {
    s32 x, y, z;
} VecFx32_022f6ddc;

typedef struct {
    u8 pad0[0x20];
    s32 unk20;
    VecFx32_022f6ddc unk24;
    VecFx32_022f6ddc unk30;
    VecFx32_022f6ddc unk3C;
    VecFx32_022f6ddc unk48;
} UnkStruct_022f6ddc_cam;

typedef struct {
    VecFx32_022f6ddc pos;
    UnkStruct_022f6ddc_cam cam;
    u8 pad[0x144 - 0xc - sizeof(UnkStruct_022f6ddc_cam)];
} UnkStruct_022f6ddc_elem;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x5c - 0x1c];
    u16 unk5C;
    u8 pad5E[0x210 - 0x5e];
    u16 unk210;
    u16 unk212;
    u8 pad214[0x218 - 0x214];
    UnkStruct_022f6ddc_elem elems[2];
    s32 unk4A0;
    s32 unk4A4;
    u8 pad4A8[0x4cc - 0x4a8];
    s32 unk4CC;
} UnkStruct_022f6ddc;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_022f6ddc *unk10;
} UnkStruct_022f6ddc_mgr;

extern UnkStruct_022f6ddc_mgr *gTaskCurrent;
extern s16 data_0206e470[];

void MTX_RotX33_(void *m, s32 s, s32 c);
void MTX_RotZ33_(void *m, s32 s, s32 c);
void MTX_Concat33(void *a, void *b, void *out);
void Model_Draw(void *cam);
void Model_Animate(void *cam);
u8 *NNS_G3dGetMdlSet(s32 a);
void NNSi_G3dModifyPolygonAttrMask(void *p, s32 a, s32 b);
s32 VEC_Distance(VecFx32_022f6ddc *a, VecFx32_022f6ddc *b);
void VEC_Subtract(VecFx32_022f6ddc *a, VecFx32_022f6ddc *b, VecFx32_022f6ddc *out);
void VEC_Normalize(VecFx32_022f6ddc *a, VecFx32_022f6ddc *out);
void VEC_CrossProduct(VecFx32_022f6ddc *a, VecFx32_022f6ddc *b, VecFx32_022f6ddc *out);

void BossFPartDE_Draw(void) {
    u8 mtx[0x24];
    VecFx32_022f6ddc v;
    UnkStruct_022f6ddc *obj;
    UnkStruct_022f6ddc_cam *cam;
    UnkStruct_022f6ddc_elem *e;
    u8 *res;
    u16 i;
    VecFx32_022f6ddc *u;
    VecFx32_022f6ddc *d;
    UnkStruct_022f6ddc_cam *c;

    obj = gTaskCurrent->unk10;
    cam = (UnkStruct_022f6ddc_cam *)((u8 *)obj + 0x4d4);
    if (obj->unk5C & 0x20)
        return;
    cam->unk48.x = obj->unk10 << 4;
    cam->unk48.y = -(obj->unk14 << 4);
    cam->unk48.z = obj->unk18 << 4;
    obj->unk210 += 0x400;
    MTX_RotX33_(&cam->unk24, data_0206e470[((obj->unk210 & 0xffff) >> 4) * 2], data_0206e470[((obj->unk210 & 0xffff) >> 4) * 2 + 1]);
    MTX_RotZ33_(mtx, data_0206e470[((obj->unk212 & 0xffff) >> 4) * 2], data_0206e470[((obj->unk212 & 0xffff) >> 4) * 2 + 1]);
    MTX_Concat33(&cam->unk24, mtx, &cam->unk24);
    Model_Draw(cam);
    if (obj->unk4A0 == 0)
        return;
    if (obj->unk4A4 != 0) {
        res = NNS_G3dGetMdlSet(obj->unk4CC);
        NNSi_G3dModifyPolygonAttrMask(res + *(s32 *)(res + *(u16 *)(res + 0xe) + 0xc), 0, 0x1f0000);
    }
    for (i = 0; i < 2; i++) {
        e = &obj->elems[i];
        v.x = e->pos.x << 4;
        v.y = -(e->pos.y << 4);
        c = &e->cam;
        v.z = e->pos.z << 4;
        c->unk48.x = obj->unk10 << 4;
        c->unk48.y = -(obj->unk14 << 4);
        c->unk48.z = obj->unk18 << 4;
        c->unk20 = VEC_Distance(&v, &c->unk48);
        d = &c->unk3C;
        VEC_Subtract(&v, &c->unk48, d);
        VEC_Normalize(d, d);
        d = &c->unk24;
        u = &c->unk30;
        u->x = 0;
        u->y = 0;
        u->z = 0x1000;
        VEC_CrossProduct(u, &c->unk3C, d);
        VEC_Normalize(d, d);
        Model_Animate(c);
        Model_Draw(c);
    }
    res = NNS_G3dGetMdlSet(obj->unk4CC);
    NNSi_G3dModifyPolygonAttrMask(res + *(s32 *)(res + *(u16 *)(res + 0xe) + 0xc), 1, 0x1f0000);
}

#include <nitro/types.h>

typedef struct UnkObj_022debf0 {
    u8 pad0[0x10c];
    u8 flags[5];
    u8 pad111[0x138 - 0x111];
} UnkObj_022debf0;

typedef struct UnkStruct_022debf0_o {
    u8 pad0[4];
    s32 unk4;
    u8 pad8[0x18 - 8];
    void *res[2];
    u8 pad20[0x24 - 0x20];
    s32 unk24;
    u8 pad28[0x30 - 0x28];
    s32 unk30;
    u8 pad34[0x3c - 0x34];
    s32 unk3C;
    u8 pad40[0x48 - 0x40];
    s32 unk48;
    u8 pad4C[0x52 - 0x4c];
    u16 unk52;
    u8 pad54[0x56 - 0x54];
    u16 unk56;
    u8 pad58[0x5a - 0x58];
    u16 unk5A;
    UnkObj_022debf0 objs[2];
} UnkStruct_022debf0_o;

typedef struct UnkTex_022debf0 {
    u8 pad0[4];
    void *tex[5];
} UnkTex_022debf0;

typedef struct UnkStruct_022debf0 {
    u8 pad0[4];
    u32 unk4;
    u8 pad8[0x454 - 8];
    UnkTex_022debf0 unk454;
} UnkStruct_022debf0;

typedef struct UnkStruct_0231c350 {
    u8 pad0[0x34];
    void *unk34;
    void *unk38;
} UnkStruct_0231c350;

extern UnkStruct_0231c350 gTransformDemoModelList;

void *File_Load(void *p, s32 v);
void Model_Init(UnkObj_022debf0 *o, s32 a);
void NNS_G3dResDefaultSetup(void *res);
void Model_SetResource(UnkObj_022debf0 *o, void *res, s32 a);
void *NNS_G3dGetTex(void *res);
void Model_SetAnimation(UnkObj_022debf0 *o, u32 slot, void *res, u32 idx, void *tex);

void TransformDemo_LoadTransModels(UnkStruct_022debf0 *w, UnkStruct_022debf0_o *o) {
    u8 i;
    UnkTex_022debf0 *t;
    u8 j;
    UnkObj_022debf0 *obj;
    u32 anim;
    void *tex;

    t = &w->unk454;

    o->res[0] = File_Load(gTransformDemoModelList.unk34, -1);
    o->res[1] = File_Load(gTransformDemoModelList.unk38, -1);
    for (i = 0; i < 2; i++) {
        obj = &o->objs[i];
        if (i == 0)
            anim = 0xd;
        else
            anim = 0xe;
        Model_Init(obj, 0);
        NNS_G3dResDefaultSetup(o->res[i]);
        Model_SetResource(obj, o->res[i], 0);
        tex = NNS_G3dGetTex(o->res[i]);
        for (j = 0; j < 5; j++) {
            if (j == 2 || j == 3) {
                if (tex != NULL) {
                    Model_SetAnimation(obj, j, t->tex[j], anim, tex);
                    obj->flags[j] |= 4;
                }
            } else {
                Model_SetAnimation(obj, j, t->tex[j], anim, 0);
                obj->flags[j] |= 4;
            }
        }
    }
    if (w->unk4 & 0x40000000) {
        o->unk52 = 0x4000;
        o->unk56 = 0x4000;
        o->unk24 = 0;
        o->unk30 = 0;
        o->unk5A = 0x8000;
    } else {
        o->unk52 = 0x4000;
        o->unk56 = 0x4000;
        o->unk52 = 0x1700;
        o->unk3C = 0x2000;
        o->unk24 = -0x190000;
        o->unk56 = 0x1700;
        o->unk48 = 0x2000;
        o->unk30 = -0x190000;
        o->unk5A = 0x8000;
    }
    o->unk4 = -0x18b000;
}

#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} VecFx32_0205d608;

typedef struct {
    s32 m[16];
} MtxFx44_0205d608;

typedef struct {
    s32 m[9];
} MtxFx33_0205d608;

typedef struct {
    u8 pad0[1];
    u8 rotZ;
    u8 rotX;
    u8 rotY;
    u8 pad4[0xc];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x5c - 0x18];
    u16 flags;
} UnkObj_0205d608;

typedef struct {
    u8 pad0[4];
    u32 flags;
    u8 pad8[0x24 - 8];
    MtxFx33_0205d608 rot;
    VecFx32_0205d608 pos;
    u8 pad54[0x90 - 0x54];
    u32 unk90;
    u8 pad94[0x10c - 0x94];
    u8 parts[5];
} UnkMdl_0205d608;

typedef struct {
    u8 pad0[0x48c];
    u32 flags;
    u8 pad490[0x4a0 - 0x490];
    u16 angle;
    u8 pad4A2[2];
    s32 unk4a4;
    s32 unk4a8;
    s32 unk4ac;
    u8 pad4B0[4];
    VecFx32_0205d608 v4b4;
    VecFx32_0205d608 v4c0;
    VecFx32_0205d608 v4cc;
} UnkCam_0205d608;

typedef struct {
    u16 angle;
    u8 pad2[2];
    s32 nearZ;
    s32 farZ;
    s32 aspect;
    u8 pad10[0x1c - 0x10];
    s32 unk1c;
} UnkPersp_0205d608;

typedef struct {
    u8 pad0[0x1c];
    u8 zone;
    u8 act;
} UnkStage_0205d608;

typedef struct {
    u8 pad0[4];
    UnkObj_0205d608 *obj;
} UnkTaskA_0205d608;

typedef struct {
    u8 pad0[0x10];
    UnkObj_0205d608 *obj;
} UnkTaskB_0205d608;

typedef struct {
    u32 cmd0;
    u32 mtxmode;
    MtxFx44_0205d608 projMtx;
    u32 cmd1;
    u8 cameraMtx[0x30];
    u8 pad7C[0xfc - 0x7c];
    u32 flag;
    u8 pad100[0x240 - 0x100];
    VecFx32_0205d608 camPos;
    VecFx32_0205d608 camUp;
    VecFx32_0205d608 camTarget;
} NNSG3dGlb_0205d608;

extern UnkStage_0205d608 gGameState;
extern UnkTaskB_0205d608 *gTaskCurrent;
extern UnkCam_0205d608 *data_022c4d98;
extern s16 data_0206e470[];
extern s16 data_02070470[];
extern UnkPersp_0205d608 *data_022c4ef4;
extern NNSG3dGlb_0205d608 NNS_G3dGlb;
extern VecFx32_0205d608 data_ov000_0236a480;

void NNS_G3dGlbLightVector(s32 i, s32 x, s32 y, s32 z);
void MTX_Identity33_(MtxFx33_0205d608 *m);
void MTX_RotY33_(MtxFx33_0205d608 *m, s32 s, s32 c);
void MTX_RotX33_(MtxFx33_0205d608 *m, s32 s, s32 c);
void MTX_RotZ33_(MtxFx33_0205d608 *m, s32 s, s32 c);
void MTX_Concat33(MtxFx33_0205d608 *a, MtxFx33_0205d608 *b, MtxFx33_0205d608 *ab);
BOOL Stage_IsGravityFlipped(s32 a, s32 b);
s32 FX_Div(s32 a, s32 b);
void VEC_Add(VecFx32_0205d608 *a, VecFx32_0205d608 *b, VecFx32_0205d608 *ab);
void G3i_PerspectiveW_(s32 s, s32 c, s32 a, s32 b, s32 d, s32 e, s32 f, MtxFx44_0205d608 *m);
void G3i_FrustumW_(s32 t, s32 b, s32 l, s32 r, s32 n, s32 f, s32 s, s32 g, MtxFx44_0205d608 *m);
void G3i_LookAt_(const VecFx32_0205d608 *pos, const VecFx32_0205d608 *up, const VecFx32_0205d608 *target, s32 a, void *out);
void Model_Animate(UnkMdl_0205d608 *m);
void Model_Draw(UnkMdl_0205d608 *m);

#define SIN_0205d608(a) data_0206e470[(((a) & 0xffff) >> 4) << 1]
#define COS_0205d608(a) data_0206e470[((((a) & 0xffff) >> 4) << 1) + 1]

static inline void GlbPerspectiveW_0205d608(s32 fovySin, s32 fovyCos, s32 aspect, s32 n, s32 f, s32 scaleW) {
    G3i_PerspectiveW_(fovySin, fovyCos, aspect, n, f, scaleW, 0, &NNS_G3dGlb.projMtx);
    NNS_G3dGlb.flag &= ~0x50;
}

static inline void GlbFrustumW_0205d608(s32 t, s32 b, s32 l, s32 r, s32 n, s32 f, s32 scaleW) {
    G3i_FrustumW_(t, b, l, r, n, f, scaleW, 0, &NNS_G3dGlb.projMtx);
    NNS_G3dGlb.flag &= ~0x50;
}

static inline void GlbLookAt_0205d608(const VecFx32_0205d608 *camPos, const VecFx32_0205d608 *camUp, const VecFx32_0205d608 *target) {
    NNS_G3dGlb.camPos = *camPos;
    NNS_G3dGlb.camUp = *camUp;
    NNS_G3dGlb.camTarget = *target;
    G3i_LookAt_(camPos, camUp, target, 0, NNS_G3dGlb.cameraMtx);
    NNS_G3dGlb.flag &= ~0xe8;
}

static inline s32 FX_Mul_0205d608(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void StageTitleCardPlayer_Draw(void) {
    MtxFx44_0205d608 saved;
    MtxFx33_0205d608 m;
    VecFx32_0205d608 pos;
    VecFx32_0205d608 target;
    UnkObj_0205d608 *obj;
    UnkMdl_0205d608 *mdl;
    u8 i;
    u16 flags;
    int ang;

    saved = NNS_G3dGlb.projMtx;
    if (gGameState.zone != 8 || gGameState.act != 2) {
        obj = gTaskCurrent->obj;
        mdl = *(UnkMdl_0205d608 **)((u8 *)obj + 0x124);
    } else {
        obj = ((UnkTaskA_0205d608 *)data_022c4d98)->obj;
        mdl = (UnkMdl_0205d608 *)((u8 *)obj + 0x134);
    }
    mdl->flags &= ~3;
    for (i = 0; i < 5; i++)
        mdl->parts[i] &= ~5;
    obj->flags &= ~8;
    if (obj->flags & 4) {
        for (i = 0; i < 5; i++)
            mdl->parts[i] |= 4;
    }
    flags = obj->flags;
    ang = (flags & 1) ? 0xc000 : 0x4000;
    if (flags & 0x800) {
        if (flags & 1) {
            NNS_G3dGlbLightVector(0, 0xb50, -0xb50, 0);
            NNS_G3dGlbLightVector(1, 0, -0x617, -0xecd);
            NNS_G3dGlbLightVector(2, 0xec7, 0x620, 0);
        } else {
            NNS_G3dGlbLightVector(0, -0xb50, -0xb50, 0);
            NNS_G3dGlbLightVector(1, 0, -0x617, -0xecd);
            NNS_G3dGlbLightVector(2, -0xec7, 0x620, 0);
        }
    } else {
        NNS_G3dGlbLightVector(0, -0xb50, -0xb50, 0);
    }
    MTX_Identity33_(&mdl->rot);
    MTX_RotY33_(&m, SIN_0205d608(ang), COS_0205d608(ang));
    MTX_Concat33(&mdl->rot, &m, &mdl->rot);
    MTX_RotX33_(&m, SIN_0205d608((u16)-(obj->rotX << 8)), COS_0205d608((u16)-(obj->rotX << 8)));
    MTX_Concat33(&mdl->rot, &m, &mdl->rot);
    MTX_RotY33_(&m, SIN_0205d608((u16)-(obj->rotY << 8)), COS_0205d608((u16)-(obj->rotY << 8)));
    MTX_Concat33(&mdl->rot, &m, &mdl->rot);
    MTX_RotZ33_(&m, SIN_0205d608((u16)(-obj->rotZ << 8)), COS_0205d608((u16)(-obj->rotZ << 8)));
    MTX_Concat33(&mdl->rot, &m, &mdl->rot);
    if (!(gGameState.zone == 8 && gGameState.act == 2) && Stage_IsGravityFlipped(obj->unk10, obj->unk14)) {
        s16 s = data_02070470[0];
        s16 c = data_02070470[1];
        MTX_RotY33_(&m, s, c);
        MTX_Concat33(&mdl->rot, &m, &mdl->rot);
        MTX_RotX33_(&m, s, c);
        MTX_Concat33(&mdl->rot, &m, &mdl->rot);
    }
    if (gGameState.zone != 8 || gGameState.act != 2) {
        if (data_022c4d98->flags & 0x400) {
            UnkPersp_0205d608 *p = data_022c4ef4;
            s32 nearZ = p->nearZ;
            s32 h = FX_Mul_0205d608(FX_Div(SIN_0205d608(p->angle), COS_0205d608(p->angle)), nearZ);
            s32 w = FX_Mul_0205d608(h, p->aspect);
            s32 d = FX_Div(p->nearZ, data_022c4ef4->unk1c);
            s32 a = FX_Mul_0205d608(d, mdl->pos.y + 0x60000);
            s32 old = mdl->pos.x;
            s32 b;
            mdl->pos.x = 0x80000;
            b = FX_Mul_0205d608(d, 0x80000 - old);
            mdl->pos.y = -0x60000;
            GlbFrustumW_0205d608(h - a, -(h + a), b - w, w + b, p->nearZ, p->farZ, 0x1000);
            VEC_Add(&data_022c4d98->v4c0, &data_ov000_0236a480, &target);
            VEC_Add(&data_022c4d98->v4b4, &data_ov000_0236a480, &pos);
            GlbLookAt_0205d608(&pos, &data_022c4d98->v4cc, &target);
        } else {
            UnkCam_0205d608 *cam = data_022c4d98;
            GlbPerspectiveW_0205d608(SIN_0205d608(cam->angle), COS_0205d608(cam->angle), cam->unk4ac, cam->unk4a4, cam->unk4a8, 0x1000);
            GlbLookAt_0205d608(&data_022c4d98->v4b4, &data_022c4d98->v4cc, &data_022c4d98->v4c0);
            mdl->pos.x = 0x80000;
            mdl->pos.y = -0x60000;
            mdl->pos.z = 0;
        }
    } else {
        UnkCam_0205d608 *cam = data_022c4d98;
        GlbPerspectiveW_0205d608(SIN_0205d608(cam->angle), COS_0205d608(cam->angle), cam->unk4ac, cam->unk4a4, cam->unk4a8, 0x1000);
        GlbLookAt_0205d608(&data_022c4d98->v4b4, &data_022c4d98->v4cc, &data_022c4d98->v4c0);
        if (!(data_022c4d98->flags & 0x400)) {
            mdl->pos.x = 0x80000;
            mdl->pos.y = -0x60000;
            mdl->pos.z = 0;
        }
    }
    if (obj->flags & 0x10)
        mdl->flags |= 2;
    if (obj->flags & 0x20)
        mdl->flags |= 1;
    mdl->unk90 |= 1;
    Model_Animate(mdl);
    Model_Draw(mdl);
    NNS_G3dGlb.projMtx = saved;
    if (mdl->parts[0] & 0x80)
        obj->flags &= ~0x400;
    if (!(mdl->parts[0] & 2))
        return;
    obj->flags &= ~0x400;
    obj->flags |= 8;
}

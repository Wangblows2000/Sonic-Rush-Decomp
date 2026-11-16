#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} VecFx32_0206a0ac;

typedef struct {
    s32 m[16];
} MtxFx44_0206a0ac;

typedef struct {
    s32 m[9];
} MtxFx33_0206a0ac;

typedef struct {
    s16 x;
    s16 y;
    s16 z;
} UnkLight_0206a0ac;

typedef struct UnkMdl_0206a0ac {
    s32 type;
    u32 flags;
    u8 pad8[0x18 - 8];
    VecFx32_0206a0ac pos;
    MtxFx33_0206a0ac rot;
    VecFx32_0206a0ac trans;
    u8 pad54[0x90 - 0x54];
    u32 unk90;
    u8 pad94[0xa8 - 0x94];
    u32 unka8;
    struct UnkMdl_0206a0ac *next;
    u8 padB0[0xc4 - 0xb0];
    s32 unkc4;
    u8 padC8[0x10c - 0xc8];
    u8 parts[5];
    u8 pad111[3];
    s32 unk114;
    u8 pad118[0x12c - 0x118];
    s16 unk12c;
} UnkMdl_0206a0ac;

typedef struct {
    u8 pad0[0x2a];
    s16 unk2a;
    u32 flags;
} UnkSub_0206a0ac;

typedef struct {
    u8 pad0[2];
    u8 rotX;
    u8 rotY;
    u8 pad4;
    u8 rotZ;
    s8 ofsX;
    s8 ofsY;
    u8 pad8[0x44 - 8];
    s16 scaleX;
    s16 scaleY;
    u8 pad48[0x5c - 0x48];
    u16 flags;
    u8 pad5E[0x128 - 0x5e];
    UnkMdl_0206a0ac *child;
} UnkObj_0206a0ac;

typedef struct {
    u16 angle;
    u8 pad2[2];
    s32 nearZ;
    s32 farZ;
    s32 aspect;
    u8 pad10[0x1c - 0x10];
    s32 unk1c;
} UnkPersp_0206a0ac;

typedef struct {
    u32 cmd0;
    u32 mtxmode;
    MtxFx44_0206a0ac projMtx;
    u32 cmd1;
    u8 cameraMtx[0x30];
    u8 pad7C[0xfc - 0x7c];
    u32 flag;
} NNSG3dGlb_0206a0ac;

extern u16 data_022c4ee8;
extern UnkLight_0206a0ac data_022c4efc[];
extern u32 gObjManagerFlags;
extern s8 data_022c4eec[];
extern s16 data_0207cf78;
extern s16 data_0207cf80[];
extern s16 data_0206e470[];
extern UnkPersp_0206a0ac *data_022c4ef4;
extern NNSG3dGlb_0206a0ac data_02084d38;
extern s16 data_0207cf7c;

void func_02022284(u32 i, s16 x, s16 y, s16 z);
void *func_ov000_0233b654(void);
void func_ov000_0233b2e0(void *a, s32 x, s32 y, s16 *ox, s16 *oy);
void func_02000dfc(MtxFx33_0206a0ac *m);
void MTX_RotY33_(MtxFx33_0206a0ac *m, s32 s, s32 c);
void func_02000e60(MtxFx33_0206a0ac *m, s32 s, s32 c);
void func_02000e98(MtxFx33_0206a0ac *m, s32 s, s32 c);
void func_02000e4c(MtxFx33_0206a0ac *m, s32 x, s32 y, s32 z);
void func_02000f34(MtxFx33_0206a0ac *a, MtxFx33_0206a0ac *b, MtxFx33_0206a0ac *ab);
s32 func_0200225c(s32 a, s32 b);
void func_02004b78(s32 t, s32 b, s32 l, s32 r, s32 n, s32 f, s32 s, s32 g, MtxFx44_0206a0ac *m);
void Model_Animate(UnkMdl_0206a0ac *m);
void Model_Draw(UnkMdl_0206a0ac *m);
void MI_Copy36B(const void *src, void *dst);

#define SIN_0206a0ac(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS_0206a0ac(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

static inline void GlbFrustumW_0206a0ac(s32 t, s32 b, s32 l, s32 r, s32 n, s32 f, s32 scaleW) {
    func_02004b78(t, b, l, r, n, f, scaleW, 0, &data_02084d38.projMtx);
    data_02084d38.flag &= ~0x50;
}

static inline s32 FX_Mul_0206a0ac(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void StageObj_Draw3DEx(s32 x, s32 y, s32 z, u32 rotZ, UnkObj_0206a0ac *obj, UnkMdl_0206a0ac *mdl) {
    MtxFx44_0206a0ac saved;
    MtxFx33_0206a0ac m;
    MtxFx33_0206a0ac ms;
    s16 sx;
    s16 sy;
    int ang = 0;
    UnkMdl_0206a0ac *m0 = NULL;
    UnkSub_0206a0ac *m2 = NULL;
    u16 flags = 0;
    u32 i;
    s16 s2a;
    s32 s114;
    s16 s12c;

    if (obj != NULL) {
        flags = obj->flags;
        if (obj->rotZ != 0)
            rotZ = (u8)(rotZ + obj->rotZ);
    }
    if (mdl->type == 0)
        m0 = mdl;
    if (mdl->type == 2) {
        flags ^= 1;
        m2 = (UnkSub_0206a0ac *)((u8 *)mdl + 0x90);
    }
    mdl->flags &= ~2;
    if (m2 != NULL) {
        m2->flags &= ~0x184;
        if (flags & 4)
            m2->flags |= 4;
        if (flags & 1)
            m2->flags |= 0x80;
        if (flags & 2)
            m2->flags |= 0x100;
    }
    if (m0 != NULL) {
        for (i = 0; i < 5; i++)
            m0->parts[i] &= ~5;
    }
    flags &= ~8;
    if (m0 != NULL && (flags & 4)) {
        for (i = 0; i < 5; i++)
            m0->parts[i] |= 4;
    }
    if (m2 == NULL)
        ang = (flags & 1) ? 0xc000 : 0x4000;
    if (flags & 0x800) {
        if (flags & 1) {
            for (i = 0; i < data_022c4ee8; i++)
                func_02022284(i, data_022c4efc[i].x, data_022c4efc[i].y, data_022c4efc[i].z);
        } else {
            for (i = 0; i < data_022c4ee8; i++)
                func_02022284(i, -data_022c4efc[i].x, data_022c4efc[i].y, data_022c4efc[i].z);
        }
    } else {
        for (i = 0; i < data_022c4ee8; i++)
            func_02022284(i, data_022c4efc[i].x, data_022c4efc[i].y, data_022c4efc[i].z);
    }
    if (!(flags & 0x2000)) {
        sx = 0;
        sy = 0;
        if ((gObjManagerFlags & 4) && !(flags & 0x80)) {
            func_ov000_0233b2e0(func_ov000_0233b654(), x, y, &sx, &sy);
        } else {
            sx = x >> 8;
            sy = y >> 8;
        }
        mdl->trans.x = sx << 12;
        mdl->trans.y = -sy << 12;
        mdl->trans.z = (z >> 8) << 12;
        if (!(flags & 0x4000)) {
            mdl->trans.x += data_022c4eec[0] << 12;
            mdl->trans.y += data_022c4eec[1] << 12;
        }
        if (obj != NULL) {
            mdl->trans.x += obj->ofsX << 12;
            mdl->trans.y -= obj->ofsY << 12;
            if (gObjManagerFlags & 0x100) {
                mdl->trans.z = z << 2;
                mdl->trans.y -= (z * data_0207cf78 >> 16) << 12;
            }
        }
        if (data_0207cf80[0] != 0x100)
            mdl->trans.x = mdl->trans.x * data_0207cf80[0] >> 8;
        if (data_0207cf80[1] != 0x100)
            mdl->trans.y = mdl->trans.y * data_0207cf80[1] >> 8;
    }
    if ((flags & 0x100) && !(flags & 0x2000)) {
        s32 scx = data_0207cf80[0] << 4;
        s32 scy = data_0207cf80[1] << 4;
        if (obj != NULL) {
            if (obj->scaleX != 0)
                scx = scx * obj->scaleX >> 8;
            if (obj->scaleY != 0)
                scy = scy * obj->scaleY >> 8;
        }
        func_02000dfc(&mdl->rot);
        MTX_RotY33_(&m, SIN_0206a0ac(ang), COS_0206a0ac(ang));
        func_02000f34(&mdl->rot, &m, &mdl->rot);
        if (obj != NULL) {
            func_02000e60(&m, SIN_0206a0ac((u16)-(obj->rotX << 8)), COS_0206a0ac((u16)-(obj->rotX << 8)));
            func_02000f34(&mdl->rot, &m, &mdl->rot);
            MTX_RotY33_(&m, SIN_0206a0ac((u16)-(obj->rotY << 8)), COS_0206a0ac((u16)-(obj->rotY << 8)));
            func_02000f34(&mdl->rot, &m, &mdl->rot);
        }
        func_02000e98(&m, SIN_0206a0ac((u16)(-rotZ << 8)), COS_0206a0ac((u16)(-rotZ << 8)));
        func_02000f34(&mdl->rot, &m, &mdl->rot);
        func_02000e4c(&m, scx, scy, 0x1000);
        func_02000f34(&mdl->rot, &m, &mdl->rot);
    } else {
        func_02000e4c(&ms, data_0207cf80[0] << 4, data_0207cf80[1] << 4, 0x1000);
        func_02000f34(&mdl->rot, &ms, &mdl->rot);
    }
    if (flags & 0x200) {
        UnkPersp_0206a0ac *p = data_022c4ef4;
        s32 nearZ = p->nearZ;
        s32 h = FX_Mul_0206a0ac(func_0200225c(SIN_0206a0ac(p->angle), COS_0206a0ac(p->angle)), nearZ);
        s32 w = FX_Mul_0206a0ac(h, p->aspect);
        s32 d = func_0200225c(p->nearZ, data_022c4ef4->unk1c);
        s32 a = FX_Mul_0206a0ac(d, mdl->trans.y + 0x60000);
        s32 old = mdl->trans.x;
        s32 b;
        mdl->trans.x = 0x80000;
        b = FX_Mul_0206a0ac(d, 0x80000 - old);
        mdl->trans.y = -0x60000;
        saved = data_02084d38.projMtx;
        GlbFrustumW_0206a0ac(h - a, -(h + a), b - w, w + b, p->nearZ, p->farZ, 0x1000);
    }
    if (m0 != NULL) {
        s16 mul = data_0207cf7c;
        s114 = m0->unk114;
        m0->unk114 = s114 * mul >> 8;
        s12c = m0->unk12c;
        m0->unk12c = s12c * mul >> 8;
    }
    if (m2 != NULL) {
        s2a = m2->unk2a;
        m2->unk2a = s2a * data_0207cf7c >> 8;
    }
    if (flags & 0x10) {
        mdl->flags |= 2;
        if (m2 != NULL)
            m2->unk2a = 0;
    }
    if (!(flags & 0x1000)) {
        if (m0 != NULL && m0->unkc4 != 0)
            m0->unk90 |= 1;
        Model_Animate(mdl);
    } else if (m0 != NULL) {
        m0->unk90 &= ~1;
    }
    if (m2 != NULL)
        m2->unk2a = s2a;
    if (m0 != NULL) {
        m0->unk114 = s114;
        m0->unk12c = s12c;
    }
    if (!(flags & 0x20)) {
        UnkMdl_0206a0ac *c;
        if (obj != NULL && (c = obj->child) != NULL) {
            for (;;) {
                if (c == NULL)
                    break;
                if (!(c->unka8 & 0x80)) {
                    s32 pz = mdl->pos.z, py = mdl->pos.y, px = mdl->pos.x;
                    s32 tz, ty, tx;
                    c->pos.x = px;
                    c->pos.y = py;
                    c->pos.z = pz;
                    tz = mdl->trans.z; ty = mdl->trans.y; tx = mdl->trans.x;
                    c->trans.x = tx;
                    c->trans.y = ty;
                    c->trans.z = tz;
                    if (!(c->unka8 & 0x40))
                        MI_Copy36B(&mdl->rot, &c->rot);
                    Model_Draw(c);
                }
                c = c->next;
            }
        } else {
            Model_Draw(mdl);
        }
    }
    if (flags & 0x200)
        data_02084d38.projMtx = saved;
    if (m0 != NULL) {
        if (m0->parts[0] & 0x80)
            flags &= ~0x400;
        if (m0->parts[0] & 2) {
            flags &= ~0x400;
            flags |= 8;
        }
    }
    if (m2 != NULL && (m2->flags & 2))
        flags |= 8;
    if (obj != NULL)
        obj->flags = (flags & ~1) | (obj->flags & 1);
}

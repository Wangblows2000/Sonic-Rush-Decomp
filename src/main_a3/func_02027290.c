#include <nitro/types.h>
#include "main_a2/g3drs.h"

typedef struct {
    fx32 x;
    fx32 y;
    fx32 z;
} UnkVec_02027290;

typedef struct {
    u32 flag;
    u8 unk4[0x24];
    fx32 rot[9];
    UnkVec_02027290 trans;
} UnkJnt_02027290;

typedef struct {
    u8 unk0[8];
    u32 flag;
    u32 ofsRot3;
    u32 ofsRot5;
    u16 ofsTag[1];
} UnkJntAnm_02027290;

typedef struct {
    UnkVec_02027290 s;
    UnkVec_02027290 inv;
} UnkScale_02027290;

typedef void (*UnkScaleCb_02027290)(UnkJnt_02027290 *jnt, const UnkScale_02027290 *sc, const u8 *cmd, u32 flag);

extern UnkRS *data_02084f9c;

void func_02026f88(fx32 *out, fx32 frame, const u32 *info, const UnkJntAnm_02027290 *anm);
void func_02027104(fx32 *out, fx32 frame, const u32 *info, const UnkJntAnm_02027290 *anm);
void func_0202644c(fx32 *out, fx32 frame, const u32 *info, const UnkJntAnm_02027290 *anm);
void func_02026798(fx32 *out, fx32 frame, const u32 *info, const UnkJntAnm_02027290 *anm);
void func_02026ba0(fx32 *out, fx32 frame, const u32 *info, const UnkJntAnm_02027290 *anm);
void func_02026d58(fx32 *out, fx32 frame, const u32 *info, const UnkJntAnm_02027290 *anm);
BOOL func_020262ac(fx32 *m, const void *rot3, const void *rot5, u32 info);
void func_02027840(UnkJnt_02027290 *jnt);
void func_02027678(UnkJnt_02027290 *jnt);
void func_020277d8(UnkJnt_02027290 *jnt);

static inline void Cross_02027290(const UnkVec_02027290 *a, const UnkVec_02027290 *b, UnkVec_02027290 *axb)
{
    fx32 x, y, z;
    x = (a->y * b->z - a->z * b->y) >> 12;
    y = (a->z * b->x - a->x * b->z) >> 12;
    z = (a->x * b->y - a->y * b->x) >> 12;
    axb->x = x;
    axb->y = y;
    axb->z = z;
}

void func_02027290(const UnkJntAnm_02027290 *anm, u32 node, fx32 frame, UnkJnt_02027290 *jnt)
{
    fx32 tmp[3][2];
    UnkScale_02027290 sc;
    BOOL interp;
    const u32 *data = (const u32 *)((const u8 *)anm + anm->ofsTag[node]);
    u32 tag = *data;
    const u32 *p;

    if (tag & 1) {
        jnt->flag = 7;
        goto end;
    }
    p = data + 1;
    if ((frame & 0xfff) && (anm->flag & 1))
        interp = TRUE;
    else
        interp = FALSE;
    jnt->flag = 0;

    if (!(tag & 6)) {
        if (!(tag & 8)) {
            if (interp)
                func_02026f88(&jnt->trans.x, frame, p, anm);
            else
                func_02027104(&jnt->trans.x, frame, p, anm);
            p += 2;
        } else {
            jnt->trans.x = *p++;
        }
        if (!(tag & 0x10)) {
            if (interp)
                func_02026f88(&jnt->trans.y, frame, p, anm);
            else
                func_02027104(&jnt->trans.y, frame, p, anm);
            p += 2;
        } else {
            jnt->trans.y = *p++;
        }
        if (!(tag & 0x20)) {
            if (interp)
                func_02026f88(&jnt->trans.z, frame, p, anm);
            else
                func_02027104(&jnt->trans.z, frame, p, anm);
            p += 2;
        } else {
            jnt->trans.z = *p++;
        }
    } else if (tag & 2) {
        jnt->flag |= 4;
    } else {
        func_02027840(jnt);
    }

    if (!(tag & 0xc0)) {
        if (!(tag & 0x100)) {
            if (interp)
                func_0202644c(jnt->rot, frame, p, anm);
            else
                func_02026798(jnt->rot, frame, p, anm);
            p += 2;
        } else {
            if (func_020262ac(jnt->rot, (const u8 *)anm + anm->ofsRot3, (const u8 *)anm + anm->ofsRot5, *p))
                Cross_02027290((UnkVec_02027290 *)&jnt->rot[0], (UnkVec_02027290 *)&jnt->rot[3], (UnkVec_02027290 *)&jnt->rot[6]);
            p += 1;
        }
    } else if (tag & 0x40) {
        jnt->flag |= 2;
    } else {
        func_02027678(jnt);
    }

    if (!(tag & 0x600)) {
        if (!(tag & 0x800)) {
            if (interp)
                func_02026ba0(tmp[0], frame, p, anm);
            else
                func_02026d58(tmp[0], frame, p, anm);
            sc.s.x = tmp[0][0];
            sc.inv.x = tmp[0][1];
        } else {
            sc.s.x = p[0];
            sc.inv.x = p[1];
        }
        if (!(tag & 0x1000)) {
            if (interp)
                func_02026ba0(tmp[1], frame, p + 2, anm);
            else
                func_02026d58(tmp[1], frame, p + 2, anm);
            sc.s.y = tmp[1][0];
            sc.inv.y = tmp[1][1];
        } else {
            sc.s.y = p[2];
            sc.inv.y = p[3];
        }
        if (!(tag & 0x2000)) {
            if (interp)
                func_02026ba0(tmp[2], frame, p + 4, anm);
            else
                func_02026d58(tmp[2], frame, p + 4, anm);
            sc.s.z = tmp[2][0];
            sc.inv.z = tmp[2][1];
        } else {
            sc.s.z = p[4];
            sc.inv.z = p[5];
        }
    } else if (tag & 0x200) {
        jnt->flag |= 1;
    } else {
        func_020277d8(jnt);
        return;
    }
end:
    ((UnkScaleCb_02027290)data_02084f9c->unkE8)(jnt, &sc, data_02084f9c->c, (jnt->flag & 1) ? 4 : 0);
}

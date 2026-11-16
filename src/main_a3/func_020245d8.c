#include <nitro/types.h>
#include <main_a2/g3drs.h>

typedef struct {
    u32 flag;
    u32 prmMatColor0;
    u32 prmMatColor1;
    u32 prmPolygonAttr;
    u32 prmTexImage;
    u32 prmTexPltt;
    fx32 scaleS;
    fx32 scaleT;
    fx16 sinR;
    fx16 cosR;
    fx32 transS;
    fx32 transT;
    u16 origW;
    u16 origH;
    fx32 magW;
    fx32 magH;
} UnkMatAnmResult_020245d8;

typedef struct {
    u16 itemTag;
    u16 size;
    u32 diffAmb;
    u32 specEmi;
    u32 polyAttr;
    u32 polyAttrMask;
    u32 texImageParam;
    u32 unk18;
    u16 texPlttBase;
    u16 flag;
    u16 origW;
    u16 origH;
    fx32 magW;
    fx32 magH;
} UnkResMatData_020245d8;

typedef void (*UnkMatCb_020245d8)(UnkMatAnmResult_020245d8 *res, void *param, u32 idx);

typedef struct {
    u8 unk0[8];
    void *cbParam;
    UnkMatCb_020245d8 cbFunc;
    u8 unk10[0x28];
    UnkMatAnmResult_020245d8 *matCache;
    u32 matBits[1];
} UnkRenderObj_020245d8;

typedef struct {
    u16 ofsTex;
    u16 ofsPltt;
    UnkResDict dict;
} UnkMatSet_020245d8;

typedef struct {
    u8 unk0[0x94];
    u32 prmMatColor0;
    u32 prmMatColor1;
    u32 prmPolygonAttr;
} UnkGlb_020245d8;

extern UnkMatAnmResult_020245d8 data_02084fa0[];
extern UnkGlb_020245d8 data_02084d38;
extern u32 data_02072b00[];

void func_02025268(u32 op, const void *args, u32 n);

void func_020245d8(UnkRS *rs, u32 opt, const UnkResMatData_020245d8 *mat, u32 idx)
{
    int t;
    UnkMatAnmResult_020245d8 *res;

    rs->curMat = idx;
    rs->flag |= 8;
    rs->pMatAnm = rs->unkF4;
    if (!UnkRS_CbA(rs, 4, &t)) {
        UnkMatAnmResult_020245d8 *cache = ((UnkRenderObj_020245d8 *)rs->obj)->matCache;
        UnkRenderObj_020245d8 *obj;
        if (cache != NULL && !(rs->flag & 0x80)) {
            res = &cache[idx];
        } else if ((opt == 0x20 || opt == 0x40) && (((u32 *)rs->unkBC)[idx >> 5] & (1 << (idx & 0x1f)))) {
            if (cache != NULL)
                res = &cache[idx];
            else
                res = &data_02084fa0[idx];
        } else {
            if (cache != NULL) {
                ((u32 *)rs->unkBC)[idx >> 5] |= 1 << (idx & 0x1f);
                res = &((UnkRenderObj_020245d8 *)rs->obj)->matCache[idx];
            } else if (opt == 0x40) {
                ((u32 *)rs->unkBC)[idx >> 5] |= 1 << (idx & 0x1f);
                res = &data_02084fa0[idx];
            } else {
                res = (UnkMatAnmResult_020245d8 *)rs->unkF4;
            }
            res->flag = 0;
            {
                UnkMatSet_020245d8 *ms = rs->resMat;
                UnkResDictEntry *ent = (UnkResDictEntry *)((u8 *)&ms->dict + ms->dict.ofsEntry);
                UnkResMatData_020245d8 *m = (UnkResMatData_020245d8 *)((u8 *)ms + *(u32 *)(ent->data + ent->sizeUnit * idx));
                if (m->flag & 0x20)
                    res->flag |= 0x20;
            }
            {
                u32 mask = data_02072b00[(mat->flag >> 6) & 7];
                res->prmMatColor0 = (data_02084d38.prmMatColor0 & ~mask) | (mat->diffAmb & mask);
            }
            {
                u32 mask = data_02072b00[(mat->flag >> 9) & 7];
                res->prmMatColor1 = (data_02084d38.prmMatColor1 & ~mask) | (mat->specEmi & mask);
            }
            res->prmPolygonAttr = (data_02084d38.prmPolygonAttr & ~mat->polyAttrMask) | (mat->polyAttr & mat->polyAttrMask);
            res->prmTexImage = mat->texImageParam;
            res->prmTexPltt = mat->texPlttBase;
            if (mat->flag & 1) {
                const fx32 *p = (const fx32 *)(mat + 1);
                if (!(mat->flag & 2)) {
                    res->scaleS = p[0];
                    res->scaleT = p[1];
                    p += 2;
                } else {
                    res->flag |= 1;
                }
                if (!(mat->flag & 4)) {
                    res->sinR = ((const fx16 *)p)[0];
                    res->cosR = ((const fx16 *)p)[1];
                    p += 1;
                } else {
                    res->flag |= 2;
                }
                if (!(mat->flag & 8)) {
                    res->transS = p[0];
                    res->transT = p[1];
                } else {
                    res->flag |= 4;
                }
                res->flag |= 8;
            }
            obj = (UnkRenderObj_020245d8 *)rs->obj;
            if (obj->cbParam != NULL && (obj->matBits[idx >> 5] & (1 << (idx & 0x1f))))
                obj->cbFunc(res, obj->cbParam, idx);
            if (res->flag & 0x18) {
                res->origW = mat->origW;
                res->origH = mat->origH;
                res->magW = mat->magW;
                res->magH = mat->magH;
            }
        }
        rs->pMatAnm = res;
    }
    if (!UnkRS_CbB2(rs, 4, &t)) {
        res = rs->pMatAnm;
        if (res->prmPolygonAttr & 0x1f0000) {
            if (res->flag & 0x20)
                res->prmPolygonAttr &= ~0x1f0000;
            rs->flag &= ~2;
            if (!(rs->flag & 0x100)) {
                u32 cmd[7];
                cmd[0] = 0x293130;
                cmd[1] = res->prmMatColor0;
                cmd[2] = res->prmMatColor1;
                cmd[3] = res->prmPolygonAttr;
                cmd[4] = 0x2b2a;
                cmd[5] = res->prmTexImage;
                cmd[6] = res->prmTexPltt;
                func_02025268(cmd[0], &cmd[1], 6);
                if (res->flag & 0x18)
                    ((void (*)(UnkMatAnmResult_020245d8 *))rs->unkF0)(res);
            }
        } else {
            rs->flag |= 2;
        }
    }
    UnkRS_CbC(rs, 4, t);
}

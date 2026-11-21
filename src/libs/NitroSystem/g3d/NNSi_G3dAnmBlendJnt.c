#include <nitro/types.h>

typedef struct {
    fx32 x;
    fx32 y;
    fx32 z;
} UnkVec_0202269c;

typedef struct {
    u32 flag;
    UnkVec_0202269c scale;
    UnkVec_0202269c scaleEx0;
    UnkVec_0202269c scaleEx1;
    fx32 rot[9];
    UnkVec_0202269c trans;
} UnkJntResult_0202269c;

typedef struct UnkAnmObj_0202269c UnkAnmObj_0202269c;
typedef void (*UnkAnmFn_0202269c)(UnkJntResult_0202269c *res, UnkAnmObj_0202269c *obj, u32 idx);
struct UnkAnmObj_0202269c {
    u32 unk0;
    fx32 ratio;
    u32 unk8;
    UnkAnmFn_0202269c func;
    UnkAnmObj_0202269c *next;
    u8 unk14[6];
    u16 mapData[1];
};

void MIi_CpuClearFast(u32 data, void *dst, u32 size);
fx32 FX_Div(fx32 a, fx32 b);
void blendScaleVec_(UnkVec_0202269c *dst, const UnkVec_0202269c *src, fx32 w, u32 flag);
void VEC_CrossProduct(const fx32 *a, const fx32 *b, fx32 *axb);
void VEC_Normalize(const fx32 *src, fx32 *dst);

BOOL NNSi_G3dAnmBlendJnt(UnkJntResult_0202269c *res, UnkAnmObj_0202269c *obj, u32 idx)
{
    UnkJntResult_0202269c r;
    UnkAnmObj_0202269c *p;
    UnkAnmObj_0202269c *last;
    fx32 sum;
    int n;
    fx32 w;

    if (obj->next == NULL) {
        u16 m = obj->mapData[idx];
        if ((m & 0x300) != 0x100)
            return FALSE;
        obj->func(res, obj, m & 0xff);
        return TRUE;
    }
    sum = 0;
    n = 0;
    p = obj;
    do {
        if ((p->mapData[idx] & 0x300) == 0x100) {
            sum += p->ratio;
            last = p;
            n++;
        }
        p = p->next;
    } while (p != NULL);
    if (sum == 0)
        return FALSE;
    if (n == 1) {
        last->func(res, last, last->mapData[idx] & 0xff);
        return TRUE;
    }
    {
        vu32 zero = 0;
        MIi_CpuClearFast(zero, res, sizeof(UnkJntResult_0202269c));
    }
    res->flag = 0xffffffff;
    do {
        u16 m = obj->mapData[idx];
        if ((m & 0x300) == 0x100 && obj->ratio > 0) {
            obj->func(&r, obj, m & 0xff);
            if (sum == 0x1000)
                w = obj->ratio;
            else
                w = FX_Div(obj->ratio, sum);
            blendScaleVec_(&res->scale, &r.scale, w, r.flag & 1);
            blendScaleVec_(&res->scaleEx0, &r.scaleEx0, w, r.flag & 8);
            blendScaleVec_(&res->scaleEx1, &r.scaleEx1, w, r.flag & 0x10);
            if (!(r.flag & 4)) {
                res->trans.x += (fx32)(((s64)w * r.trans.x) >> 12);
                res->trans.y += (fx32)(((s64)w * r.trans.y) >> 12);
                res->trans.z += (fx32)(((s64)w * r.trans.z) >> 12);
            }
            if (!(r.flag & 2)) {
                res->rot[0] += (w * r.rot[0]) >> 12;
                res->rot[1] += (w * r.rot[1]) >> 12;
                res->rot[2] += (w * r.rot[2]) >> 12;
                res->rot[3] += (w * r.rot[3]) >> 12;
                res->rot[4] += (w * r.rot[4]) >> 12;
                res->rot[5] += (w * r.rot[5]) >> 12;
            } else {
                res->rot[0] += w;
                res->rot[4] += w;
            }
            res->flag &= r.flag;
        }
        obj = obj->next;
    } while (obj != NULL);
    VEC_CrossProduct(&res->rot[0], &res->rot[3], &res->rot[6]);
    VEC_Normalize(&res->rot[0], &res->rot[0]);
    VEC_Normalize(&res->rot[6], &res->rot[6]);
    VEC_CrossProduct(&res->rot[6], &res->rot[0], &res->rot[3]);
    return TRUE;
}

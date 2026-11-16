#include <nitro/types.h>

typedef struct {
    u32 info;
    u32 offset;
} UnkAnmInfo_02026798;

typedef struct {
    u8 unk0[4];
    u16 numFrame;
    u8 unk6[2];
    u32 flag;
    u32 ofsRot3;
    u32 ofsRot5;
} UnkRotAnm_02026798;

BOOL getRotDataByIdx_(fx32 *m, const void *rot3, const void *rot5, u32 info);
void VEC_Normalize(const fx32 *src, fx32 *dst);

typedef struct {
    fx32 x;
    fx32 y;
    fx32 z;
} UnkVec_02026798;

static inline void Cross_02026798(const UnkVec_02026798 *a, const UnkVec_02026798 *b, UnkVec_02026798 *axb)
{
    fx32 x, y, z;
    x = (a->y * b->z - a->z * b->y) >> 12;
    y = (a->z * b->x - a->x * b->z) >> 12;
    z = (a->x * b->y - a->y * b->x) >> 12;
    axb->x = x;
    axb->y = y;
    axb->z = z;
}

void getRotData_(fx32 *m, fx32 frame, const UnkAnmInfo_02026798 *ai, const UnkRotAnm_02026798 *anm)
{
    fx32 tA[9];
    fx32 tB[9];
    const void *rot3;
    const void *rot5;
    u32 idx;
    u32 info;
    const u16 *data;
    u32 i0, i1;
    BOOL r;

    idx = frame >> 12;
    rot3 = (const u8 *)anm + anm->ofsRot3;
    rot5 = (const u8 *)anm + anm->ofsRot5;
    info = ai->info;
    data = (const u16 *)((const u8 *)anm + ai->offset);

    if (info & 0xc0000000) {
        u32 last = (info & 0x1fff0000) >> 16;
        if (info & 0x40000000) {
            if (idx & 1) {
                if (idx > last) {
                    idx = (last >> 1) + 1;
                    goto direct;
                }
                i0 = idx >> 1;
                goto interp2;
            }
            idx >>= 1;
            goto direct;
        } else {
            if (idx & 3) {
                if (idx > last) {
                    idx = (idx & 3) + (last >> 2);
                    goto direct;
                }
                if (idx & 1) {
                    r = 0;
                    if (idx & 2) {
                        i1 = idx >> 2;
                        i0 = i1 + 1;
                    } else {
                        i0 = idx >> 2;
                        i1 = i0 + 1;
                    }
                    r |= getRotDataByIdx_(m, rot3, rot5, data[i0]);
                    r |= getRotDataByIdx_(tA, rot3, rot5, data[i1]);
                    m[0] = m[0] * 3 + tA[0];
                    m[1] = m[1] * 3 + tA[1];
                    m[2] = m[2] * 3 + tA[2];
                    m[3] = m[3] * 3 + tA[3];
                    m[4] = m[4] * 3 + tA[4];
                    m[5] = m[5] * 3 + tA[5];
                    VEC_Normalize(&m[0], &m[0]);
                    VEC_Normalize(&m[3], &m[3]);
                    if (!r) {
                        m[6] = m[6] * 3 + tA[6];
                        m[7] = m[7] * 3 + tA[7];
                        m[8] = m[8] * 3 + tA[8];
                        VEC_Normalize(&m[6], &m[6]);
                    } else {
                        Cross_02026798((UnkVec_02026798 *)&m[0], (UnkVec_02026798 *)&m[3], (UnkVec_02026798 *)&m[6]);
                    }
                    return;
                }
                i0 = idx >> 2;
                goto interp2;
            }
            idx >>= 2;
            goto direct;
        }
    }
    goto direct;
interp2:
    r = 0;
    r |= getRotDataByIdx_(m, rot3, rot5, data[i0]);
    r |= getRotDataByIdx_(tB, rot3, rot5, data[i0 + 1]);
    m[0] += tB[0];
    m[1] += tB[1];
    m[2] += tB[2];
    m[3] += tB[3];
    m[4] += tB[4];
    m[5] += tB[5];
    VEC_Normalize(&m[0], &m[0]);
    VEC_Normalize(&m[3], &m[3]);
    if (!r) {
        m[6] += tB[6];
        m[7] += tB[7];
        m[8] += tB[8];
        VEC_Normalize(&m[6], &m[6]);
    } else {
        Cross_02026798((UnkVec_02026798 *)&m[0], (UnkVec_02026798 *)&m[3], (UnkVec_02026798 *)&m[6]);
    }
    return;
direct:
    if (!getRotDataByIdx_(m, rot3, rot5, data[idx]))
        return;
    Cross_02026798((UnkVec_02026798 *)&m[0], (UnkVec_02026798 *)&m[3], (UnkVec_02026798 *)&m[6]);
}

#include <nitro/types.h>

typedef struct {
    u32 info;
    u32 offset;
} UnkAnmInfo_0202644c;

typedef struct {
    u8 unk0[4];
    u16 numFrame;
    u8 unk6[2];
    u32 flag;
    u32 ofsRot3;
    u32 ofsRot5;
} UnkRotAnm_0202644c;

BOOL func_020262ac(fx32 *m, const void *rot3, const void *rot5, u32 info);
void func_020024b4(const fx32 *src, fx32 *dst);

typedef struct {
    fx32 x;
    fx32 y;
    fx32 z;
} UnkVec_0202644c;

static inline void Cross_0202644c(const UnkVec_0202644c *a, const UnkVec_0202644c *b, UnkVec_0202644c *axb)
{
    fx32 x, y, z;
    x = (a->y * b->z - a->z * b->y) >> 12;
    y = (a->z * b->x - a->x * b->z) >> 12;
    z = (a->x * b->y - a->y * b->x) >> 12;
    axb->x = x;
    axb->y = y;
    axb->z = z;
}

void func_0202644c(fx32 *m, fx32 frame, const UnkAnmInfo_0202644c *ai, const UnkRotAnm_0202644c *anm)
{
    const void *rot3;
    fx32 m0[9];
    const void *rot5;
    BOOL r;
    u32 idx;
    fx32 m1[9];
    fx32 w;
    u32 step;
    const u16 *data;
    u32 idx1;
    u32 info;

    rot3 = (const u8 *)anm + anm->ofsRot3;
    rot5 = (const u8 *)anm + anm->ofsRot5;
    data = (const u16 *)((const u8 *)anm + ai->offset);
    info = ai->info;
    idx = frame >> 12;

    if (idx == anm->numFrame - 1) {
        if (info & 0xc0000000) {
            if (info & 0x40000000)
                idx = (idx & 1) + (idx >> 1);
            else
                idx = (idx & 3) + (idx >> 2);
        }
        if (anm->flag & 2) {
            idx1 = 0;
            goto step1;
        }
        if (!func_020262ac(m, rot3, rot5, data[idx]))
            return;
        Cross_0202644c((UnkVec_0202644c *)&m[0], (UnkVec_0202644c *)&m[3], (UnkVec_0202644c *)&m[6]);
        return;
    }
    if (info & 0xc0000000) {
        u32 last = (info & 0x1fff0000) >> 16;
        if (info & 0x40000000) {
            if (idx >= last) {
                idx = last >> 1;
                idx1 = idx + 1;
                goto step1;
            }
            idx >>= 1;
            idx1 = idx + 1;
            w = frame & 0x1fff;
            step = 2;
        } else {
            if (idx >= last) {
                idx = (idx & 3) + (idx >> 2);
                idx1 = idx + 1;
                goto step1;
            }
            idx >>= 2;
            idx1 = idx + 1;
            w = frame & 0x3fff;
            step = 4;
        }
    } else {
        idx1 = idx + 1;
    step1:
        step = 1;
        w = frame & 0xfff;
    }
    r = 0;
    r |= func_020262ac(m0, rot3, rot5, data[idx]);
    r |= func_020262ac(m1, rot3, rot5, data[idx1]);
    m[0] = m0[0] * step + ((w * (m1[0] - m0[0])) >> 12);
    m[1] = m0[1] * step + ((w * (m1[1] - m0[1])) >> 12);
    m[2] = m0[2] * step + ((w * (m1[2] - m0[2])) >> 12);
    m[3] = m0[3] * step + ((w * (m1[3] - m0[3])) >> 12);
    m[4] = m0[4] * step + ((w * (m1[4] - m0[4])) >> 12);
    m[5] = m0[5] * step + ((w * (m1[5] - m0[5])) >> 12);
    func_020024b4(&m[0], &m[0]);
    func_020024b4(&m[3], &m[3]);
    if (!r) {
        m[6] = m0[6] * step + ((w * (m1[6] - m0[6])) >> 12);
        m[7] = m0[7] * step + ((w * (m1[7] - m0[7])) >> 12);
        m[8] = m0[8] * step + ((w * (m1[8] - m0[8])) >> 12);
        func_020024b4(&m[6], &m[6]);
    } else {
        Cross_0202644c((UnkVec_0202644c *)&m[0], (UnkVec_0202644c *)&m[3], (UnkVec_0202644c *)&m[6]);
    }
}

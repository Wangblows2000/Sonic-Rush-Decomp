#include <nitro/types.h>

typedef struct {
    fx32 m[4][3];
    fx32 n[3][3];
} UnkEvpMtx_02023274;

typedef struct {
    fx32 m[4][4];
    fx32 n[3][3];
} UnkEvpCache_02023274;

typedef struct {
    fx32 n[3][3];
    fx32 m[4][4];
} UnkEvpCacheN_02023274;

typedef struct {
    u8 unk0[0x10];
    u32 ofsEvp;
} UnkMdl_02023274;

typedef struct {
    u8 unk0[4];
    UnkMdl_02023274 *mdl;
} UnkObj_02023274;

typedef struct {
    u8 *c;
    UnkObj_02023274 *obj;
    u8 unk8[0xC4];
    u32 isEvpCached[8];
} UnkRS_02023274;

extern UnkEvpCache_02023274 data_020863a0[];
extern UnkEvpCacheN_02023274 data_020863e0[];

void MIi_CpuClearFast(u32 data, void *dst, u32 size);
void func_02025454(void);
void G3_MultMtx43(const void *m);
void G3_MultMtx33(const void *m);
void G3_LoadMtx43(const void *m);
BOOL func_020040e4(void *m);
BOOL func_0200409c(void *m);

static inline BOOL BitVecCheck(const u32 *vec, u32 idx) {
    return (BOOL)(vec[idx >> 5] & (1 << (idx & 31)));
}
static inline void BitVecSet(u32 *vec, u32 idx) {
    vec[idx >> 5] |= 1 << (idx & 31);
}

#define REG_MTX_MODE (*(vu32 *)0x04000440)
#define REG_MTX_STORE (*(vu32 *)0x0400044C)
#define REG_MTX_RESTORE (*(vu32 *)0x04000450)
#define REG_MTX_IDENTITY (*(vu32 *)0x04000454)

void func_02023274(UnkRS_02023274 *rs) {
    struct {
        fx32 m[4][3];
        fx32 n[3][3];
    } sum;
    UnkMdl_02023274 *mdl;
    UnkEvpMtx_02023274 *evp;
    u32 num;
    u32 cached;
    s64 w;
    u8 *p;
    u8 *c;
    u32 i;
    fx32 *pM;
    fx32 *pN;
    u32 idx;

    mdl = rs->obj->mdl;
    c = rs->c;
    evp = (UnkEvpMtx_02023274 *)((u8 *)mdl + mdl->ofsEvp);
    num = c[2];
    w = 0;
    p = c + 3;

    {
        vu32 z = 0;
        MIi_CpuClearFast(z, &sum, sizeof(sum));
    }
    func_02025454();
    REG_MTX_MODE = 0;
    REG_MTX_STORE = 1;
    REG_MTX_IDENTITY = 0;
    REG_MTX_MODE = 2;
    for (i = 0; i < num; i++) {
        idx = p[1];
        cached = BitVecCheck(rs->isEvpCached, idx);
        pM = &data_020863a0[idx].m[0][0];
        if (!cached) {
            BitVecSet(rs->isEvpCached, idx);
            REG_MTX_RESTORE = p[0];
            REG_MTX_MODE = 1;
            G3_MultMtx43(evp[idx].m);
        }
        if (i != 0) {
            sum.n[0][0] += (w * pN[0]) >> 12;
            sum.n[0][1] += (w * pN[1]) >> 12;
            sum.n[0][2] += (w * pN[2]) >> 12;
            sum.n[1][0] += (w * pN[3]) >> 12;
            sum.n[1][1] += (w * pN[4]) >> 12;
            sum.n[1][2] += (w * pN[5]) >> 12;
            sum.n[2][0] += (w * pN[6]) >> 12;
            sum.n[2][1] += (w * pN[7]) >> 12;
            sum.n[2][2] += (w * pN[8]) >> 12;
        }
        if (!cached) {
            while (func_020040e4(pM)) {
            }
            REG_MTX_MODE = 2;
            G3_MultMtx33(evp[idx].n);
        }
        pN = &data_020863e0[idx].n[0][0];
        w = p[2] << 4;
        sum.m[0][0] += (w * pM[0]) >> 12;
        sum.m[0][1] += (w * pM[1]) >> 12;
        sum.m[0][2] += (w * pM[2]) >> 12;
        sum.m[1][0] += (w * pM[4]) >> 12;
        sum.m[1][1] += (w * pM[5]) >> 12;
        sum.m[1][2] += (w * pM[6]) >> 12;
        sum.m[2][0] += (w * pM[8]) >> 12;
        sum.m[2][1] += (w * pM[9]) >> 12;
        sum.m[2][2] += (w * pM[10]) >> 12;
        sum.m[3][0] += (w * pM[12]) >> 12;
        sum.m[3][1] += (w * pM[13]) >> 12;
        sum.m[3][2] += (w * pM[14]) >> 12;
        p += 3;
        if (!cached) {
            while (func_0200409c(pN)) {
            }
        }
    }
    sum.n[0][0] += (w * pN[0]) >> 12;
    sum.n[0][1] += (w * pN[1]) >> 12;
    sum.n[0][2] += (w * pN[2]) >> 12;
    sum.n[1][0] += (w * pN[3]) >> 12;
    sum.n[1][1] += (w * pN[4]) >> 12;
    sum.n[1][2] += (w * pN[5]) >> 12;
    sum.n[2][0] += (w * pN[6]) >> 12;
    sum.n[2][1] += (w * pN[7]) >> 12;
    sum.n[2][2] += (w * pN[8]) >> 12;
    G3_LoadMtx43(sum.n);
    REG_MTX_MODE = 1;
    G3_LoadMtx43(sum.m);
    REG_MTX_MODE = 0;
    REG_MTX_RESTORE = 1;
    REG_MTX_MODE = 2;
    REG_MTX_STORE = rs->c[1];
    rs->c += (rs->c[2] + 1) * 3;
}

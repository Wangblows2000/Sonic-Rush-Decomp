#include <nitro/types.h>
#include <main_a2/g3drs.h>

typedef struct { u8 unk0[0x10]; u32 prmTexImage; u8 unk14[0x18]; u16 origW; u16 origH; } UnkMatAnm_02022e00;
typedef struct { u8 unk0[0x1E]; u16 flag; } UnkMatData_02022e00;
typedef struct { u8 unk0[4]; UnkResDict dict; } UnkResMat_02022e00;
typedef struct { u8 unk0[0xFC]; u32 flag; } UnkGlb_02022e00;

extern u32 data_02077b68[];
extern u32 data_02077b6c[];
extern UnkGlb_02022e00 data_02084d38;
extern u32 data_02084d84[];
extern u32 data_02084df4[];

void func_02025268(u32 op, const void *args, u32 n);
void func_02025a94(u32 a, void *m);

static inline void *GetData_02022e00(UnkResDict *dict, u32 idx) {
    UnkResDictEntry *e = (UnkResDictEntry *)((u8 *)dict + dict->ofsEntry);
    return e->data + e->sizeUnit * idx;
}

void func_02022e00(UnkRS *rs) {
    u32 a;
    u32 b;
    u32 c;
    u32 d;
    u32 e;
    fx32 m[9];
    fx32 v[3];
    int t;
    BOOL skip;
    u32 w;
    u32 h;
    if (!(rs->flag & 0x200) && (rs->flag & 1)) {
        if ((((UnkMatAnm_02022e00 *)rs->pMatAnm)->prmTexImage & 0xC0000000) != 0x80000000) {
            ((UnkMatAnm_02022e00 *)rs->pMatAnm)->prmTexImage &= ~0xC0000000;
            ((UnkMatAnm_02022e00 *)rs->pMatAnm)->prmTexImage |= 0x80000000;
            data_02077b68[1] = ((UnkMatAnm_02022e00 *)rs->pMatAnm)->prmTexImage;
            func_02025268(data_02077b68[0], data_02077b6c, 1);
        }
        a = 3;
        func_02025268(0x10, &a, 1);
        skip = UnkRS_CbA(rs, 0xC, &t);
        if (!skip) {
            h = ((UnkMatAnm_02022e00 *)rs->pMatAnm)->origH;
            w = ((UnkMatAnm_02022e00 *)rs->pMatAnm)->origW;
            v[0] = w << 15;
            v[1] = -h << 15;
            v[2] = 0x10000;
            func_02025268(0x1B, v, 3);
            b = (u16)(s16)(w << 3) | ((u16)(s16)(h << 3) << 16);
            func_02025268(0x22, &b, 1);
        }
        skip = UnkRS_CbB2(rs, 0xC, &t);
        if (!skip) {
            UnkResMat_02022e00 *res = rs->resMat;
            UnkMatData_02022e00 *mat = (UnkMatData_02022e00 *)((u8 *)res + *(u32 *)GetData_02022e00(&res->dict, rs->c[1]));
            if (mat->flag & 0x2000) {
                u8 *p = (u8 *)mat + 0x2C;
                if (!(mat->flag & 2)) p += 8;
                if (!(mat->flag & 4)) p += 4;
                if (!(mat->flag & 8)) p += 8;
                func_02025268(0x18, p, 0x10);
            }
        }
        if (t == 3) {
            rs->flag &= ~0x40;
            rs->cb[0xC](rs);
            skip = rs->flag & 0x40;
        } else {
            skip = FALSE;
        }
        if (!skip) {
            c = 2;
            func_02025268(0x10, &c, 1);
            func_02025a94(0, m);
            d = 3;
            func_02025268(0x10, &d, 1);
            if (data_02084d38.flag & 1) {
                func_02025268(0x1A, data_02084d84, 9);
                func_02025268(0x1A, data_02084df4, 9);
                func_02025268(0x1A, m, 9);
            } else if (data_02084d38.flag & 2) {
                func_02025268(0x1A, data_02084d84, 9);
                func_02025268(0x1A, m, 9);
            } else {
                func_02025268(0x1A, m, 9);
            }
        }
        e = 2;
        func_02025268(0x10, &e, 1);
    }
    rs->c += 3;
}

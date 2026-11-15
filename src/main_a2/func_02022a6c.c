#include <nitro/types.h>
#include <main_a2/g3drs.h>

typedef struct { u8 unk0[0x10]; u32 prmTexImage; u8 unk14[0x18]; u16 origW; u16 origH; } UnkMatAnm_02022a6c;
typedef struct { u8 unk0[0x1E]; u16 flag; } UnkMatData_02022a6c;
typedef struct { u8 unk0[4]; UnkResDict dict; } UnkResMat_02022a6c;
typedef struct { u8 unk0[0xFC]; u32 flag; } UnkGlb_02022a6c;

extern u32 data_02077b60[];
extern u32 data_02077b64[];
extern fx32 data_02077b90[];
extern UnkGlb_02022a6c data_02084d38;
extern u32 data_02084e18[];
extern u32 data_02084df4[];

void func_02025268(u32 op, const void *args, u32 n);
void func_02025a94(void *m, u32 a);
void *func_020221ac(void);
void func_02025454(void);
BOOL func_020040e4(void *m);

#define REG_MTX_MODE (*(vu32 *)0x04000440)
#define REG_MTX_PUSH (*(vu32 *)0x04000444)
#define REG_MTX_POP (*(vu32 *)0x04000448)
#define REG_MTX_IDENTITY (*(vu32 *)0x04000454)

static inline void *GetData_02022a6c(UnkResDict *dict, u32 idx) {
    UnkResDictEntry *e = (UnkResDictEntry *)((u8 *)dict + dict->ofsEntry);
    return e->data + e->sizeUnit * idx;
}

void func_02022a6c(UnkRS *rs) {
    u32 a;
    u32 b;
    u32 c;
    u32 d;
    fx32 m43[12];
    fx32 m44[16];
    int t;
    BOOL skip;
    u32 w;
    u32 h;
    if (!(rs->flag & 0x200) && (rs->flag & 1)) {
        func_02025a94(m43, 0);
        a = 0x1E;
        func_02025268(0x13, &a, 1);
        if ((((UnkMatAnm_02022a6c *)rs->pMatAnm)->prmTexImage & 0xC0000000) != 0xC0000000) {
            ((UnkMatAnm_02022a6c *)rs->pMatAnm)->prmTexImage &= ~0xC0000000;
            ((UnkMatAnm_02022a6c *)rs->pMatAnm)->prmTexImage |= 0xC0000000;
            data_02077b60[1] = ((UnkMatAnm_02022a6c *)rs->pMatAnm)->prmTexImage;
            func_02025268(data_02077b60[0], data_02077b64, 1);
        }
        skip = UnkRS_CbA(rs, 0xD, &t);
        if (!skip) {
            h = ((UnkMatAnm_02022a6c *)rs->pMatAnm)->origH;
            w = ((UnkMatAnm_02022a6c *)rs->pMatAnm)->origW;
            data_02077b90[0] = w << 15;
            data_02077b90[5] = -h << 15;
            data_02077b90[12] = w << 15;
            data_02077b90[13] = h << 15;
            func_02025268(0x16, data_02077b90, 0x10);
        }
        skip = UnkRS_CbB2(rs, 0xD, &t);
        if (!skip) {
            UnkResMat_02022a6c *res = rs->resMat;
            UnkMatData_02022a6c *mat = (UnkMatData_02022a6c *)((u8 *)res + *(u32 *)GetData_02022a6c(&res->dict, rs->c[1]));
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
            rs->cb[0xD](rs);
            skip = rs->flag & 0x40;
        } else {
            skip = FALSE;
        }
        if (!skip) {
            if (data_02084d38.flag & 1) {
                func_02025268(0x1C, data_02084e18, 3);
                func_02025268(0x1A, data_02084df4, 9);
                func_02025268(0x19, m43, 0xC);
            } else if (data_02084d38.flag & 2) {
                func_02025268(0x19, m43, 0xC);
            } else {
                func_02025268(0x19, func_020221ac(), 0xC);
                func_02025268(0x19, m43, 0xC);
            }
            func_02025454();
            REG_MTX_MODE = 0;
            REG_MTX_PUSH = 0;
            REG_MTX_IDENTITY = 0;
            while (func_020040e4(m44)) {
            }
            REG_MTX_POP = 1;
            REG_MTX_MODE = 3;
            func_02025268(0x16, m44, 0x10);
            {
                s32 s = m44[12] >> 4;
                s32 tt = m44[13] >> 4;
                b = (u16)(s16)(s >> 8) | ((u16)(s16)(tt >> 8) << 16);
            }
            func_02025268(0x22, &b, 1);
        }
        c = 2;
        func_02025268(0x10, &c, 1);
        d = 0x1E;
        func_02025268(0x14, &d, 1);
    }
    rs->c += 3;
}

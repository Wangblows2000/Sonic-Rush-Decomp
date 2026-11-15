#include <nitro/types.h>
#include <main_a2/g3drs.h>

typedef struct {
    u32 flag;
    fx32 scale[3];
    fx32 invScale[3];
    fx32 unk1C[3];
    fx32 rot[9];
    fx32 trans[3];
} UnkJntAnm_02023fb8;

typedef BOOL (*UnkJntCb_02023fb8)(UnkJntAnm_02023fb8 *res, void *arg, u32 idx);
typedef struct { u8 unk0[0x10]; void *cbArg; UnkJntCb_02023fb8 cb; u8 unk18[0x1C]; UnkJntAnm_02023fb8 *rec; } UnkObj_02023fb8;
typedef void (*UnkScaleFn_02023fb8)(UnkJntAnm_02023fb8 *res, const u8 *p, const u8 *c, u32 flag);
typedef void (*UnkSendFn_02023fb8)(void *res);

extern u8 data_02072b20[];
extern u8 data_02072b21[];
extern u8 data_02072b22[];
extern u8 data_02072b23[];

void func_02025268(u32 op, const void *args, u32 n);
void MI_Zero36B(void *p);

static inline void *GetData_02023fb8(UnkResDict *dict, u32 idx) {
    UnkResDictEntry *e = (UnkResDictEntry *)((u8 *)dict + dict->ofsEntry);
    return e->data + e->sizeUnit * idx;
}

void func_02023fb8(UnkRS *rs, u32 opt) {
    u32 len = 4;
    u32 idx;
    int t;
    BOOL skip;
    idx = rs->c[1];
    rs->curNodeDesc = idx;
    rs->flag |= 0x10;
    if (rs->flag & 0x400) {
        if (opt == 0x40 || opt == 0x60) len++;
        if (opt == 0x20 || opt == 0x60) {
            len++;
            if (!(rs->flag & 0x100)) {
                u32 a = rs->c[4];
                func_02025268(0x14, &a, 1);
            }
        }
        rs->c += len;
        return;
    }
    if (opt == 0x40 || opt == 0x60) {
        u32 a;
        len++;
        if (opt == 0x40) {
            a = rs->c[4];
        } else {
            a = rs->c[5];
        }
        if (!(rs->flag & 0x100)) func_02025268(0x14, &a, 1);
    }
    rs->pJntAnm = rs->tmpJntAnm;
    skip = UnkRS_CbA(rs, 6, &t);
    if (!skip) {
        UnkJntAnm_02023fb8 *res;
        BOOL useRec;
        UnkObj_02023fb8 *obj = rs->obj;
        if (obj->rec != NULL) {
            res = &obj->rec[idx];
            useRec = (rs->flag & 0x80) == 0;
        } else {
            res = (UnkJntAnm_02023fb8 *)rs->tmpJntAnm;
            useRec = FALSE;
        }
        if (!useRec) {
            res->flag = 0;
            if (!(((UnkObj_02023fb8 *)rs->obj)->cbArg != NULL &&
                  ((UnkObj_02023fb8 *)rs->obj)->cb(res, ((UnkObj_02023fb8 *)rs->obj)->cbArg, idx))) {
                const u8 *p;
                u16 *srt;
                UnkResDict *dict = rs->unkD4;
                srt = (u16 *)((u8 *)dict + *(u32 *)GetData_02023fb8(dict, idx));
                p = (const u8 *)srt + 4;
                if (srt[0] & 1) {
                    res->flag |= 4;
                } else {
                    res->trans[0] = ((const fx32 *)p)[0];
                    res->trans[1] = ((const fx32 *)p)[1];
                    res->trans[2] = ((const fx32 *)p)[2];
                    p += 12;
                }
                if (srt[0] & 2) {
                    res->flag |= 2;
                } else if (srt[0] & 8) {
                    s32 idxPivot = (srt[0] & 0xF0) >> 4;
                    fx32 A;
                    fx32 B;
                    A = ((const s16 *)p)[0];
                    B = ((const s16 *)p)[1];
                    MI_Zero36B(res->rot);
                    res->rot[idxPivot] = (srt[0] & 0x100) ? -0x1000 : 0x1000;
                    p += 4;
                    res->rot[data_02072b20[idxPivot * 4]] = A;
                    res->rot[data_02072b21[idxPivot * 4]] = B;
                    if (srt[0] & 0x200) B = -B;
                    res->rot[data_02072b22[idxPivot * 4]] = B;
                    if (srt[0] & 0x400) A = -A;
                    res->rot[data_02072b23[idxPivot * 4]] = A;
                } else {
                    res->rot[0] = (s16)srt[1];
                    res->rot[1] = ((const s16 *)p)[0];
                    res->rot[2] = ((const s16 *)p)[1];
                    res->rot[3] = ((const s16 *)p)[2];
                    res->rot[4] = ((const s16 *)p)[3];
                    res->rot[5] = ((const s16 *)p)[4];
                    res->rot[6] = ((const s16 *)p)[5];
                    res->rot[7] = ((const s16 *)p)[6];
                    res->rot[8] = ((const s16 *)p)[7];
                    p += 16;
                }
                ((UnkScaleFn_02023fb8)rs->unkE8)(res, p, rs->c, srt[0]);
            }
        }
        rs->pJntAnm = res;
    }
    skip = UnkRS_CbB2(rs, 6, &t);
    if (!skip && !(rs->flag & 0x100)) {
        ((UnkSendFn_02023fb8)rs->unkEC)(rs->pJntAnm);
    }
    rs->pJntAnm = NULL;
    if (t == 3) {
        rs->flag &= ~0x40;
        rs->cb[6](rs);
        skip = rs->flag & 0x40;
    } else {
        skip = FALSE;
    }
    if (opt == 0x20 || opt == 0x60) {
        len++;
        if (!skip && !(rs->flag & 0x100)) {
            u32 b = rs->c[4];
            func_02025268(0x13, &b, 1);
        }
    }
    rs->c += len;
}

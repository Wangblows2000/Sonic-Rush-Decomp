#include <nitro/types.h>

typedef struct { u8 revision; u8 numEntry; u16 sizeDictBlk; u16 dummy; u16 ofsEntry; } UnkDict_02025054;
typedef struct { u16 sizeUnit; u16 ofsName; u8 data[4]; } UnkDictEntry_02025054;
typedef struct {
    u8 unk0[8];
    u32 ofsMat;
    u32 ofsShp;
    u8 unk10[6];
    u8 texMtxMode;
    u8 unk17[5];
    fx32 posScale;
    fx32 invPosScale;
} UnkMdl_02025054;
typedef struct { u8 unk0[4]; UnkDict_02025054 dict; } UnkResMat_02025054;
typedef struct {
    u8 unk0[4];
    u32 diffAmb;
    u32 specEmi;
    u32 polyAttr;
    u32 unk10;
    u32 texImageParam;
    u32 unk18;
    u16 texPlttBase;
    u16 flag;
    u16 origW;
    u16 origH;
    fx32 magW;
    fx32 magH;
} UnkMatData_02025054;
typedef struct { u8 unk0[8]; u32 ofsDL; u32 sizeDL; } UnkShpData_02025054;
typedef struct {
    u32 flag;
    u8 unk4[0x14];
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
} UnkTexMtx_02025054;
typedef void (*UnkTexMtxFn_02025054)(UnkTexMtx_02025054 *tm);

extern UnkTexMtxFn_02025054 data_02077b50[];

void func_02025268(u32 op, const void *args, u32 n);
void func_02025364(const void *p, u32 size);

static inline void GeScale_02025054(fx32 x, fx32 y, fx32 z) {
    fx32 v[3];
    v[0] = x;
    v[1] = y;
    v[2] = z;
    func_02025268(0x1B, v, 3);
}

static inline void *GetData_02025054(UnkDict_02025054 *dict, u32 idx) {
    UnkDictEntry_02025054 *e = (UnkDictEntry_02025054 *)((u8 *)dict + dict->ofsEntry);
    return e->data + e->sizeUnit * idx;
}

void func_02025054(UnkMdl_02025054 *mdl, u32 matID, u32 shpID, BOOL sendMat) {
    struct {
        u32 cmd;
        u32 diffAmb;
        u32 specEmi;
        u32 polyAttr;
        u32 cmd2;
        u32 texImageParam;
        u32 texPlttBase;
    } buf;
    UnkTexMtx_02025054 tm;
    if (mdl->posScale != 0x1000) {
        GeScale_02025054(mdl->posScale, mdl->posScale, mdl->posScale);
    }
    if (sendMat) {
        UnkResMat_02025054 *res = (UnkResMat_02025054 *)((u8 *)mdl + mdl->ofsMat);
        UnkMatData_02025054 *mat = (UnkMatData_02025054 *)((u8 *)res + *(u32 *)GetData_02025054(&res->dict, matID));
        if (!(mat->polyAttr & 0x1F0000)) return;
        buf.cmd = 0x293130;
        buf.diffAmb = mat->diffAmb;
        buf.specEmi = mat->specEmi;
        buf.polyAttr = mat->polyAttr;
        if (mat->flag & 0x20) buf.polyAttr &= ~0x1F0000;
        buf.cmd2 = 0x2B2A;
        buf.texImageParam = mat->texImageParam;
        buf.texPlttBase = mat->texPlttBase;
        func_02025268(buf.cmd, &buf.diffAmb, 6);
        if (mat->flag & 1) {
            UnkTexMtxFn_02025054 fn;
            const u8 *p;
            fn = data_02077b50[mdl->texMtxMode];
            tm.flag = 8;
            p = (const u8 *)mat + 0x2C;
            tm.origW = mat->origW;
            tm.origH = mat->origH;
            tm.magW = mat->magW;
            tm.magH = mat->magH;
            if (!(mat->flag & 2)) {
                tm.scaleS = ((const fx32 *)p)[0];
                tm.scaleT = ((const fx32 *)p)[1];
                p += 8;
            } else {
                tm.flag |= 1;
            }
            if (!(mat->flag & 4)) {
                tm.sinR = ((const fx16 *)p)[0];
                tm.cosR = ((const fx16 *)p)[1];
                p += 4;
            } else {
                tm.flag |= 2;
            }
            if (!(mat->flag & 8)) {
                tm.transS = ((const fx32 *)p)[0];
                tm.transT = ((const fx32 *)p)[1];
            } else {
                tm.flag |= 4;
            }
            fn(&tm);
        }
    }
    {
        UnkDict_02025054 *shp = (UnkDict_02025054 *)((u8 *)mdl + mdl->ofsShp);
        UnkShpData_02025054 *sd = (UnkShpData_02025054 *)((u8 *)shp + *(u32 *)GetData_02025054(shp, shpID));
        func_02025364((u8 *)sd + sd->ofsDL, sd->sizeDL);
    }
    if (mdl->invPosScale != 0x1000) {
        GeScale_02025054(mdl->invPosScale, mdl->invPosScale, mdl->invPosScale);
    }
}

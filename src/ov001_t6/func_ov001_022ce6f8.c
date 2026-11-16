#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    u16 w;
    u16 h;
} UnkRes_022ce6f8;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkc;
} UnkFile_022ce6f8;

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x18];
} UnkSpr_022ce6f8;

typedef struct {
    u8 pad0[0x40];
} UnkBg_022ce6f8;

typedef struct {
    u8 pad0[0x78];
    UnkSpr_022ce6f8 sprites[11];
    s32 ids[11];
    UnkSpr_022ce6f8 spr3bc;
    s32 unk404;
    u8 pad408[0xc0c - 0x408];
    UnkSpr_022ce6f8 spr2[41];
    s32 ids2[41];
    u8 pad1838[0x1bc8 - 0x1838];
    UnkBg_022ce6f8 bg1bc8;
    UnkBg_022ce6f8 bg1c08;
    UnkBg_022ce6f8 bg1c48;
    UnkBg_022ce6f8 bg1c88;
    UnkBg_022ce6f8 bg1cc8;
    u8 pad1d08[0x1de8 - 0x1d08];
    s32 unk1de8[7];
    void *res1e04;
    void *res1e08;
    void *res1e0c;
    void *res1e10;
    void *res1e14;
    void *res1e18;
    void *res1e1c;
    void *res1e20;
    void *res1e24;
    void *res1e28;
    void *res1e2c;
    UnkRes_022ce6f8 *res1e30;
    UnkRes_022ce6f8 *res1e34;
    UnkRes_022ce6f8 *res1e38;
    UnkRes_022ce6f8 *res1e3c;
    void *res1e40;
    UnkFile_022ce6f8 *res1e44;
} UnkWork_022ce6f8;

extern u16 data_ov001_02315fdc[11];
extern u16 data_ov001_02315fbc[];
extern void *data_ov001_0231f048;
extern s32 data_ov001_0231f04c;

void func_0203473c(void *bg, UnkRes_022ce6f8 *res, s32 a, s32 b, s32 c, u16 w, u16 h);
void func_0203438c(void *bg);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_ov001_022ce670(UnkSpr_022ce6f8 *s, void *res, u16 anim);
void func_ov001_022ce6b4(UnkSpr_022ce6f8 *s, void *res, u16 anim);
s32 func_02039c18(s32 a, s32 b);

#pragma thumb on
void func_ov001_022ce6f8(UnkWork_022ce6f8 *p) {
    UnkRes_022ce6f8 *res;
    void *sres;
    s32 i;
    UnkSpr_022ce6f8 *spr;
    s32 *id;
    vu32 z0;
    vu32 z1;

    res = p->res1e30;
    func_0203473c(&p->bg1bc8, res, 0x38, 0, 3, res->w, res->h);
    func_0203438c(&p->bg1bc8);
    res = p->res1e34;
    func_0203473c(&p->bg1bc8, res, 0x38, 0, 2, res->w, res->h);
    func_0203438c(&p->bg1bc8);
    res = p->res1e30;
    func_0203473c(&p->bg1c08, res, 0x38, 1, 3, res->w, res->h);
    func_0203438c(&p->bg1c08);
    z0 = 0;
    MIi_CpuClear32(z0, (void *)0x6200800, 0x800);
    res = p->res1e38;
    func_0203473c(&p->bg1c88, res, 0x38, 1, 0, res->w, res->h);
    func_0203438c(&p->bg1c88);
    z1 = 0;
    MIi_CpuClear32(z1, (void *)0x6200800, 0x800);
    res = p->res1e3c;
    func_0203473c(&p->bg1cc8, res, 0x38, 1, 1, res->w, res->h);
    func_0203438c(&p->bg1cc8);
    for (i = 0; i < 11; i++) {
        if (i == 0) {
            sres = p->res1e0c;
        } else if (i == 2) {
            sres = p->res1e14;
        } else if (i == 10) {
            sres = p->res1e28;
        }
        func_ov001_022ce670(&p->sprites[i], sres, data_ov001_02315fdc[i]);
        p->sprites[i].flags &= 0x60000;
        if (i == 1 || i == 10) {
            p->sprites[i].flags |= 0x20000;
        } else {
            p->sprites[i].flags |= 0x60000;
        }
    }
    for (i = 0; i < 1; i++) {
        func_ov001_022ce6b4(&p->spr3bc + i, p->res1e14, data_ov001_02315fbc[i]);
        (&p->spr3bc + i)->flags &= 0x60000;
        (&p->spr3bc + i)->flags |= 0x20000;
    }
    for (i = 0; i < 11; i++) {
        p->ids[i] = -1;
    }
    p->unk404 = -1;
    for (i = 0; i < 7; i++) {
        p->unk1de8[i] = func_02039c18(1, 1);
    }
    spr = p->spr2;
    id = p->ids2;
    sres = p->res1e1c;
    for (i = 0; i < 41; i++) {
        func_ov001_022ce670(spr, sres, 0);
        spr->flags &= 0x60000;
        *id++ = 0;
        spr++;
    }
    {
        UnkFile_022ce6f8 *fl = p->res1e44;
        data_ov001_0231f048 = (u8 *)fl + fl->unk8;
        data_ov001_0231f04c = fl->unkc;
    }
}
#pragma thumb reset

#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x14];
    u16 unk44;
    u16 unk46;
} UnkSpr_022d2704;

typedef struct {
    UnkSpr_022d2704 spr;
    u8 pad48[0x2c];
    u16 unk74;
    u16 unk76;
    u16 unk78;
    u8 pad7a[2];
} UnkObj_022d2704;

typedef struct {
    s32 mode;
    u8 pad4[8];
    u8 unkc[0x414 - 0xc];
    void *res414;
    void *res418;
    void *res41c;
    UnkSpr_022d2704 sprites[18];
    UnkSpr_022d2704 spr930;
    UnkSpr_022d2704 spr978;
    UnkObj_022d2704 objs[4];
    u8 padbb0[0xbe4 - 0xbb0];
    UnkSpr_022d2704 spr3[3];
} UnkWork_022d2704;

extern u16 data_ov001_023160ec[];
extern u16 data_ov001_023160dc[];
extern u16 data_ov001_023160e4[];
extern s32 data_02250458;

void func_ov001_022d2650(void);
void func_ov001_022cfd34(UnkSpr_022d2704 *s, void *res, u16 a);
void func_ov001_022cfcb8(UnkSpr_022d2704 *s, void *res, u16 a);
void func_ov001_022cfcf8(UnkSpr_022d2704 *s, void *res, u16 a);
void func_ov001_022cfc60(UnkObj_022d2704 *o, void *res, u16 a);
void func_0203344c(UnkSpr_022d2704 *s, s32 a, s32 b);
void func_020332c4(UnkObj_022d2704 *o, s32 a, s32 b);
void func_020203bc(s32 a, s32 b);
void func_020201b0(void *p);

#pragma thumb on
void func_ov001_022d2704(UnkWork_022d2704 *w) {
    s32 i;
    UnkSpr_022d2704 *s;

    func_ov001_022d2650();
    i = 0;
    s = w->sprites;
    for (; i < 18; i++) {
        func_ov001_022cfd34(s, w->res414, i);
        func_0203344c(s, 0, 0);
        s->flags |= 0x18;
        s++;
    }
    s = &w->spr930;
    func_ov001_022cfcb8(s, w->res418, 0);
    s->unk46 = 2;
    func_0203344c(s, 0, 0);
    s = &w->spr978;
    func_ov001_022cfcb8(s, w->res414, 0x24);
    s->unk46 = 3;
    func_0203344c(s, 0, 0);
    if (w->mode != 2) {
        u16 *t1;
        u16 *t2;
        i = 0;
        s = w->spr3;
        t1 = data_ov001_023160ec;
        t2 = data_ov001_023160dc;
        for (; i < 3; i++) {
            void *res = w->res414;
            u16 a;
            if (w->mode == 0) {
                a = t1[0];
            } else {
                a = t1[3];
            }
            func_ov001_022cfcf8(s, res, a);
            s->unk44 = *t2;
            s->unk46 = 7;
            s->flags |= 0x60000;
            func_0203344c(s, 0, 0);
            s++;
            t1++;
            t2++;
        }
        if (w->mode == 0) {
            w->spr3[2].flags |= 0x80000;
        }
    }
    {
        UnkObj_022d2704 *o;
        u16 *t;
        i = 0;
        o = w->objs;
        t = data_ov001_023160e4;
        for (; i < 4; i++) {
            func_ov001_022cfc60(o, w->res41c, *t);
            o->unk74 = i;
            o->unk76 = i;
            o->spr.unk46 = 5;
            o->unk78 = 5;
            o->spr.flags |= 0x4000;
            func_020332c4(o, 0, 0);
            o++;
            t++;
        }
    }
    func_020203bc(10, data_02250458);
    func_020201b0(w->unkc);
}
#pragma thumb reset

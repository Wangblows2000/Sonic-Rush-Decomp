#include <nitro/types.h>

typedef struct UnkObj_0206c3d0 UnkObj_0206c3d0;

struct UnkObj_0206c3d0 {
    u8 pad0[6];
    s8 b6;
    s8 b7;
    u8 pad8[8];
    s32 x10;
    s32 x14;
    u8 pad18[0x1e];
    s16 h36;
    s16 h38;
    s16 h3a;
    u8 pad3c[0xe];
    s16 h4a;
    s16 h4c;
    s16 h4e;
    s16 h50;
    s16 h52;
    u32 flags54;
    u32 flags58;
    u16 h5c;
    u8 pad5e[4];
    u16 h62;
    u8 pad64[4];
    void (*fn68)(UnkObj_0206c3d0 *o);
    void (*fn6c)(void);
    void (*fn70)(void);
    u32 pad74;
    void (*fn78)(void);
    void (*fn7c)(void);
    s32 x80;
    s32 x84;
    UnkObj_0206c3d0 *x88;
    u8 pad8c[8];
    u8 x94[0x10];
    s32 xa4;
    u8 pada8[0x34];
    u8 xdc[0x48];
    s32 x124;
    s32 x128;
    s32 x12c;
};

typedef struct {
    u8 pad0[0x10];
    UnkObj_0206c3d0 *work;
} UnkTask_0206c3d0;

extern UnkTask_0206c3d0 *data_022b4574;
extern u32 data_022c4ef0;
extern s8 data_02077944[];

void func_020371c8(UnkTask_0206c3d0 *t);
s32 func_02068e58(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void func_0206919c(UnkObj_0206c3d0 *o);
BOOL func_0206c798(u32 v);
void func_020694b0(UnkObj_0206c3d0 *o);
void func_0206bd5c(UnkObj_0206c3d0 *o);
void func_0206c708(UnkObj_0206c3d0 *o);
void func_0206bd24(UnkObj_0206c3d0 *o, void *a, void *b);

void func_0206c3d0(void) {
    UnkTask_0206c3d0 *t = data_022b4574;
    UnkObj_0206c3d0 *o = t->work;
    u16 saved;

    if (o->flags54 & 4) {
        func_020371c8(t);
        return;
    }
    if (o->flags54 & 8)
        o->flags54 |= 4;
    if ((o->flags54 & 0x10) == 0) {
        if (func_02068e58(o->x10, o->x14, o->h4a, o->h4c, o->h4e, o->h50, o->h52) != 0)
            o->flags54 |= 4;
    }
    if (o->x88 != NULL && (o->x88->flags54 & 4)) {
        if ((o->flags54 & 0x200) == 0)
            o->flags54 |= 4;
        o->x88 = NULL;
    }
    func_0206919c(o);
    if ((!func_0206c798(o->flags54) || (o->flags54 & 0x40)) && o->fn6c != NULL)
        o->fn6c();
    if (!func_0206c798(o->flags54)) {
        if (o->h62 != 0) {
            if (o->h62 & 0xf)
                o->h62--;
            if (o->h62 & 0xf0)
                o->h62 -= 0x10;
        }
        if (!func_0206c798(o->flags54)) {
            if ((o->h62 & 0xf) == 0 && (o->flags54 & 0x80) == 0 && o->fn68 != NULL)
                o->fn68(o);
            if ((o->flags58 & 0x2000) == 0)
                func_020694b0(o);
            if ((data_022c4ef0 & 8) && (o->flags58 & 0x100) == 0) {
                o->x80 = 0;
                o->x84 = 0;
                func_0206bd5c(o);
            }
        }
    }
    if ((o->h62 & 0xf) || func_0206c798(o->flags54)) {
        saved = o->h5c;
        o->h5c = saved | 0x10;
    }
    if (o->h62 & 0xf0) {
        o->b6 += data_02077944[(o->h62 & 0xf0) >> 5];
        o->b7 += data_02077944[((o->h62 & 0xf0) >> 5) + 1];
    }
    if ((o->flags54 & 4) == 0) {
        if (o->fn70 != NULL)
            o->fn70();
        if (o->fn70 == NULL || (o->h5c & 0x40)) {
            if (o->x124 != 0 || o->x128 != 0 || o->x12c != 0)
                func_0206c708(o);
            else if (o->xa4 != 0)
                func_0206bd24(o, o->x94, o->xdc);
        }
    }
    if ((o->h62 & 0xf) || func_0206c798(o->flags54))
        o->h5c = saved;
    if (func_0206c798(o->flags54))
        return;
    if ((o->h62 & 0xf) == 0 && (data_022c4ef0 & 0x10) && o->fn78 != NULL)
        o->fn78();
    o->h36 = 0;
    o->h38 = 0;
    o->h3a = 0;
    o->b6 = 0;
    o->b7 = 0;
    if (o->fn7c != NULL)
        o->fn7c();
}

#include <nitro/types.h>

typedef struct {
    u16 eva : 5;
    u16 pad0 : 3;
    u16 evb : 5;
    u16 pad1 : 3;
} UnkAlpha_022d1494;

typedef struct {
    u8 pad0[0x22];
    UnkAlpha_022d1494 alpha;
} UnkGfx_022d1494;

typedef struct {
    u8 pad0[8];
    s16 bright;
} UnkBright_022d1494;

typedef struct {
    u8 pad0[0x40];
} UnkObj_022d1494;

typedef struct {
    u8 pad0[0x14];
    s32 timer;
    u8 pad18[0x54 - 0x18];
    UnkObj_022d1494 objs[(0xb34 - 0x54) / 0x40];
    u8 pad[0xb34 - 0x54 - ((0xb34 - 0x54) / 0x40) * 0x40];
    u8 unkb34[0xd0c - 0xb34];
    u8 unkd0c[0xf0c - 0xd0c];
    u8 unkf0c[0x110c - 0xf0c];
    u8 unk110c[0x130c - 0x110c];
    u8 unk130c[4];
} UnkStruct_022d1494;

extern UnkGfx_022d1494 gDisplayMain;
extern UnkAlpha_022d1494 data_0208924e;
extern UnkBright_022d1494 data_0208926c;

BOOL func_0202e5e4(float a, float b);
void func_ov001_022cfecc(void *a, s32 b, float f, void *c, void *d);
void func_ov001_022cfd7c(void *a, s32 b, float f, void *c, void *d);
void func_ov001_022d0050(s32 a);
void func_ov001_022d2234(UnkStruct_022d1494 *p, void (*fn)(UnkStruct_022d1494 *));
void func_ov001_022d1ce0(UnkStruct_022d1494 *p);

#pragma thumb on
void func_ov001_022d1494(UnkStruct_022d1494 *p) {
    float f;

    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1a00;
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0xa00;
    if (p->timer <= 0x78) {
        f = p->timer / 120.0f;
        if (func_0202e5e4(f, 1.0f)) f = 1.0f;
        f = f * f;
        func_ov001_022cfecc(&p->objs[2], 0x100, f, p->unkf0c, p->unkd0c);
        func_ov001_022cfd7c(p->unkb34, 0x100, f, p->unk130c, p->unk110c);
    }
    if (data_0208926c.bright < 0 || gDisplayMain.alpha.eva != 0xf) {
        if ((p->timer & 7) == 4) {
            func_ov001_022d0050((s16)(data_0208926c.bright + 1));
        } else if ((p->timer & 7) == 0) {
            data_0208924e.eva++;
            if (gDisplayMain.alpha.eva > 0xf) gDisplayMain.alpha.eva = 0xf;
            gDisplayMain.alpha.evb = 0xf - gDisplayMain.alpha.eva;
        }
    }
    if (p->timer > 0x1c6) {
        func_ov001_022d2234(p, func_ov001_022d1ce0);
    }
}
#pragma thumb reset

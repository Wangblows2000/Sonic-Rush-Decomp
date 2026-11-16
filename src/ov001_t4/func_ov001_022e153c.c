#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    u32 unk10;
    u8 pad14[4];
    u16 w;
    u16 h;
} UnkRes_022e153c;

typedef struct {
    u8 pad0[0x48];
} UnkSpr_022e153c;

typedef struct {
    u8 pad0[0xe0];
    u8 bg0[0x40];
    u8 bg1[0x40];
    u8 buf[0x200];
    UnkSpr_022e153c sprites[38];
    u8 padx[0xeac - 0xe10];
    UnkRes_022e153c *res0;
    UnkRes_022e153c *res1;
    UnkRes_022e153c *res2;
    UnkRes_022e153c *res3;
    UnkRes_022e153c *res4;
} UnkWork_022e153c;

void func_0203473c(void *bg, UnkRes_022e153c *res, s32 a, s32 b, s32 c, u16 w, u16 h);
void func_0203438c(void *bg);
s32 func_ov001_022e17bc(void *p);
u32 func_ov001_022e17b0(void *p);
void func_02008648(void *src, void *dst);
void func_02008720(void *src, void *dst);
void func_020087f8(void *src, void *dst);
void *func_02033a54(void *p);
void func_02033a68(void *src, void *dst, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_ov001_022e14dc(UnkSpr_022e153c *s, UnkRes_022e153c *res, u32 idx, u16 v, s32 mode);

#pragma thumb on
void func_ov001_022e153c(UnkWork_022e153c *w) {
    UnkRes_022e153c *res;
    s32 i;

    res = w->res0;
    func_0203473c(w->bg0, res, 0x38, 0, 1, res->w, res->h);
    func_0203438c(w->bg0);
    res = w->res1;
    func_0203473c(w->bg1, res, 0x38, 0, 2, res->w, res->h);
    func_0203438c(w->bg1);
    res = (UnkRes_022e153c *)((u8 *)res + res->unk10);
    switch (func_ov001_022e17bc(res)) {
    case 0x10:
        func_02008648(res, w->buf);
        break;
    case 0x20:
        func_02008720(res, w->buf);
        break;
    case 0x30:
        func_020087f8(res, w->buf);
        break;
    default:
        func_02033a68(func_02033a54(res), w->buf, func_ov001_022e17b0(res));
        break;
    }
    DC_StoreRange(w->buf, func_ov001_022e17b0(res));
    res = w->res2;
    func_ov001_022e14dc(&w->sprites[0], res, 0, 1, 1);
    func_ov001_022e14dc(&w->sprites[1], res, 1, 1, 1);
    func_ov001_022e14dc(&w->sprites[2], res, 2, 1, 1);
    func_ov001_022e14dc(&w->sprites[3], res, 3, 2, 1);
    func_ov001_022e14dc(&w->sprites[4], res, 4, 2, 1);
    func_ov001_022e14dc(&w->sprites[5], res, 5, 2, 1);
    func_ov001_022e14dc(&w->sprites[6], res, 6, 3, 1);
    func_ov001_022e14dc(&w->sprites[7], res, 7, 13, 0);
    func_ov001_022e14dc(&w->sprites[8], res, 8, 13, 0);
    func_ov001_022e14dc(&w->sprites[9], res, 15, 4, 0);
    func_ov001_022e14dc(&w->sprites[10], res, 18, 4, 0);
    func_ov001_022e14dc(&w->sprites[11], res, 21, 4, 0);
    func_ov001_022e14dc(&w->sprites[12], res, 25, 0, 0);
    func_ov001_022e14dc(&w->sprites[13], res, 26, 10, 0);
    func_ov001_022e14dc(&w->sprites[14], res, 26, 10, 0);
    func_ov001_022e14dc(&w->sprites[15], res, 26, 10, 0);
    func_ov001_022e14dc(&w->sprites[37], w->res4, 0, 0xe, 0);
    res = w->res3;
    for (i = 0x10; i <= 0x24; i++) {
        func_ov001_022e14dc(&w->sprites[i], res, 0, 7, 0);
    }
}
#pragma thumb reset

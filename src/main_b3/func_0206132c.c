#include <nitro/types.h>
#include <main_b3/w50.h>

typedef struct {
    u8 pad0[0x70];
    s32 unk70;
    u8 pad74[0x10];
    UnkSpr_W50 spr;  /* 0x84 */
    void *res;       /* 0xcc */
} UnkWork_0206132c;
extern char data_0207cc54[];
extern char *data_0207cb94;
extern u8 data_0207764c[];

void *func_02060464(s32 a);
void func_0202a2f0(char *buf, const char *fmt, ...);
void *func_02033fbc(void *a, u32 b);
void *func_02039c18(u32 a, u32 b);
void func_0203363c(UnkSpr_W50 *s, void *res, u32 idx, u32 c, u32 d, u32 e, void *f, u32 g, u32 h);
void func_0203344c(UnkSpr_W50 *s, u32 a, u32 b);

#pragma thumb on
void func_0206132c(UnkWork_0206132c *w) {
    char buf[0x30] = {0};
    UnkSpr_W50 *spr;

    func_0202a2f0(buf, data_0207cc54, data_0207cb94, func_02060464(w->unk70));
    w->res = func_02033fbc(buf, 0);
    spr = &w->spr;
    func_0203363c(spr, w->res, 0, 0x4404, 0, 0, func_02039c18(0, data_0207764c[w->unk70]), 0, 0x05000200);
    spr->unk44 = 0xf;
    spr->unk46 = 0;
    func_0203344c(spr, 0, 0);
    spr->unk2C |= 0x10;
}
#pragma thumb reset

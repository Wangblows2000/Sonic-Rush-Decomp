#include <nitro/types.h>
#include <main_b3/w50.h>

typedef struct {
    u8 pad0[0x14];
    UnkSpr_W50 spr[7]; /* 0x14 */
    u8 pad20C[0x4];
    void *res1;        /* 0x210 */
    void *res2;        /* 0x214 */
} UnkWork_0205788c;

extern u8 data_0207c5d4[];
extern u8 data_02072cb8;
extern void *data_0207c490[];

void *func_02033fbc(void *a, u32 b);
void *func_02039c18(u32 a, u32 b);
void func_0203363c(UnkSpr_W50 *s, void *res, u32 idx, u32 c, u32 d, u32 e, void *f, u32 g, u32 h);
void func_0203344c(UnkSpr_W50 *s, u32 a, u32 b);

#pragma thumb on
void func_0205788c(UnkWork_0205788c *w) {
    UnkSpr_W50 *spr;
    u32 idx;

    w->res1 = func_02033fbc(data_0207c5d4, 0);
    switch (data_02072cb8) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        idx = data_02072cb8;
        break;
    default:
        idx = 1;
        break;
    }
    w->res2 = func_02033fbc(data_0207c490[idx], 0);

    spr = &w->spr[0];
    func_0203363c(spr, w->res1, 0, 4, 1, 0, func_02039c18(1, 0x20), 0, 0x05000600);
    spr->unk44 = 3;
    spr->unk46 = 0;
    func_0203344c(spr, 0, 0);
    spr->unk2C |= 0x10;

    spr = &w->spr[1];
    func_0203363c(spr, w->res1, 1, 4, 1, 0, func_02039c18(1, 0x20), 0, 0x05000600);
    spr->unk44 = 3;
    spr->unk46 = 0;
    func_0203344c(spr, 0, 0);
    spr->unk2C |= 0x10;

    spr = &w->spr[2];
    func_0203363c(spr, w->res1, 2, 4, 1, 0, func_02039c18(1, 0x20), 0, 0x05000600);
    spr->unk2C |= 0x10;
    spr->unk44 = 0;
    spr->unk46 = 1;
    func_0203344c(spr, 0, 0);

    spr = &w->spr[3];
    func_0203363c(spr, w->res1, 3, 4, 1, 0, func_02039c18(1, 0x20), 0, 0x05000600);
    spr->unk2C |= 0x10;
    spr->unk44 = 0;
    spr->unk46 = 1;
    func_0203344c(spr, 0, 0);

    spr = &w->spr[4];
    func_0203363c(spr, w->res2, 0, 4, 1, 0, func_02039c18(1, 0x20), 0, 0x05000600);
    spr->unk44 = 2;
    spr->unk46 = 0;
    func_0203344c(spr, 0, 0);
    spr->unk2C |= 0x10;

    spr = &w->spr[5];
    func_0203363c(spr, w->res2, 1, 4, 1, 0, func_02039c18(1, 0x20), 0, 0x05000600);
    spr->unk44 = 0;
    spr->unk46 = 0;
    func_0203344c(spr, 0, 0);
    spr->unk2C |= 0x10;

    spr = &w->spr[6];
    func_0203363c(spr, w->res2, 2, 4, 1, 0, func_02039c18(1, 0x20), 0, 0x05000600);
    spr->unk44 = 0;
    spr->unk46 = 0;
    func_0203344c(spr, 0, 0);
    spr->unk2C |= 0x10;
}
#pragma thumb reset

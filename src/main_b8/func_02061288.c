#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u16 unk4;
    u16 unk6;
} UnkSprite_02061288;

typedef struct {
    u8 pad0[0x20];
    s32 unk20;
} UnkCfg_02061288;

typedef struct {
    u8 pad0[0x6c];
    UnkCfg_02061288 *cfg;
    u8 pad70[0x14];
    UnkSprite_02061288 spr;
} UnkWork_02061288;

void func_0203344c(UnkSprite_02061288 *s, s32 a, s32 b);
void func_02032d2c(UnkSprite_02061288 *s);

#pragma thumb on
void func_02061288(UnkWork_02061288 *w) {
    UnkSprite_02061288 *s = &w->spr;

    func_0203344c(s, 0, 0);
    if (w->cfg->unk20 == 1)
        s->unk4 = 0xd2;
    else
        s->unk4 = 0x7e;
    s->unk6 = 0x8a;
    func_02032d2c(s);
}
#pragma thumb reset

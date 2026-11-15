#include <nitro/types.h>
#include <main_b3/w50.h>

typedef struct { u8 pad0[4]; u32 flags; } UnkPlayer_020576dc;
typedef struct {
    u8 pad0[4];
    UnkPlayer_020576dc *player; /* 0x4 */
    u8 pad8[0xc];
    UnkSpr_W50 spr[7];          /* 0x14 */
    s16 unk20C;                 /* 0x20c */
} UnkWork_020576dc;

void func_0203344c(UnkSpr_W50 *s, u32 a, u32 b);
void func_02032d2c(UnkSpr_W50 *s);

#pragma thumb on
void func_020576dc(UnkWork_020576dc *w) {
    UnkSpr_W50 *spr;
    u32 f;

    spr = &w->spr[0];
    func_0203344c(spr, 0, 0);
    func_02032d2c(spr);
    spr = &w->spr[1];
    func_0203344c(spr, 0, 0);
    func_02032d2c(spr);
    spr = &w->spr[4];
    func_0203344c(spr, 0, 0);
    func_02032d2c(spr);
    f = w->player->flags;
    if (f & 0x100000) {
        if (f & 0x800000) {
            spr = &w->spr[3];
            func_0203344c(spr, 0, 0);
            func_02032d2c(spr);
            spr = &w->spr[6];
            func_0203344c(spr, 0, 0);
            func_02032d2c(spr);
        } else {
            spr = &w->spr[2];
            func_0203344c(spr, 0, 0);
            func_02032d2c(spr);
            spr = &w->spr[5];
            func_0203344c(spr, 0, 0);
            w->unk20C = (w->unk20C + 1) % 60;
            if (w->unk20C < 0x28)
                func_02032d2c(spr);
        }
    }
}
#pragma thumb reset

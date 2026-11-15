#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x3e];
    u16 unk46;
} UnkSpr_022d9b3c;

typedef struct {
    u32 unk0;
    s32 tiles[4];
    u8 pad14[0x20 - 0x14];
    s32 unk20;
    u8 pad24[4];
    s32 unk28;
    u32 tiles2[2];
} UnkEntry_022d9b3c;

typedef struct {
    u8 pad0[0x10];
    s32 sel;
    u8 pad14[4];
    s32 unk18;
    s32 unk1c;
    u8 pad20[0x70 - 0x20];
    UnkEntry_022d9b3c entries[6];
    u8 pad[0x210 - 0x70 - 6 * 0x34];
    UnkSpr_022d9b3c spr[32];
} UnkStruct_022d9b3c;

void func_0203344c(UnkSpr_022d9b3c *s, u32 a, u32 b);
void func_02032d2c(UnkSpr_022d9b3c *s);
void func_ov001_022d9864(UnkStruct_022d9b3c *p, s32 *src, u32 *dst, s32 x, s32 y, s32 flag);

#pragma thumb on
void func_ov001_022d9b3c(UnkStruct_022d9b3c *p, s32 i, s32 x, s32 y) {
    UnkSpr_022d9b3c *spr;
    s32 skip;
    UnkEntry_022d9b3c *e = &p->entries[i];

    if (p->sel == i) {
        if (p->unk1c) {
            spr = &p->spr[9];
            func_0203344c(spr, 0, 0);
        } else {
            spr = &p->spr[10];
        }
        skip = 0;
    } else if (p->entries[i].unk28 == 0) {
        spr = &p->spr[11];
        func_0203344c(spr, 0, 0);
        skip = 1;
    } else {
        spr = &p->spr[10];
        skip = 0;
    }
    spr->x = x;
    spr->y = y;
    spr->unk46 = 8;
    func_02032d2c(spr);
    if (skip == 1) return;

    spr = &p->spr[i + 13];
    spr->x = x;
    spr->y = y;
    spr->unk46 = 6;
    func_02032d2c(spr);
    {
        s32 *src = p->entries[i].tiles;
        u32 *dst = p->entries[i].tiles2;
        s32 tx = x + 0x16;
        s32 ty = y + 5;
        if (p->unk18) {
            func_ov001_022d9864(p, src, dst, tx, ty, 1);
        } else {
            func_ov001_022d9864(p, src, dst, tx, ty, 0);
        }
    }
    spr = &p->spr[e->unk28 + 0x15];
    spr->x = x + 0x58;
    spr->y = y;
    spr->unk46 = 6;
    func_02032d2c(spr);
    spr = &p->spr[24];
    spr->x = x + 0x60;
    spr->y = y;
    spr->unk46 = 6;
    func_02032d2c(spr);
    spr = &p->spr[23];
    spr->x = x + 0x68;
    spr->y = y;
    spr->unk46 = 6;
    func_02032d2c(spr);
    spr = &p->spr[e->unk20 + 0x19];
    spr->x = x + 0x78;
    spr->y = y;
    spr->unk46 = 6;
    func_02032d2c(spr);
}
#pragma thumb reset

#include <nitro/types.h>

typedef struct {
    u8 pad0[0xdac];
    u8 *buf;
} UnkWork_022d9864;

typedef struct {
    u8 pad0[4];
    s32 base;
} UnkVram_022d9864;

typedef struct {
    u8 pad0[0x14];
    u16 shift;
} UnkCfg_022d9864;

extern UnkVram_022d9864 gObjVramBaseTable;
extern UnkCfg_022d9864 data_02087d84;

void VRAMTransfer_QueueCopy(void *src, s32 size, s32 a, s32 dst);
u16 *Oam_AllocObj(s32 a, s32 b);

#pragma thumb on
void VsMenu_Unk_ov001_022d9864(UnkWork_022d9864 *w, s32 *tiles, s32 *addrs, s32 x, s32 y, s32 pal) {
    s32 base;
    s32 yb;
    s32 a2;
    s32 idx[4];
    s32 xx;
    s32 i;
    s32 *t;
    s32 *d;
    u32 ofs;
    u16 *oam;

    xx = x;
    i = 0;
    idx[3] = 0;
    idx[2] = 0;
    idx[1] = 0;
    idx[0] = 0;
    t = tiles;
    d = idx;
    for (; i < 4; i++) {
        if (xx >= 0 && xx < 0xf8) {
            *d = *t;
        }
        xx += 8;
        t++;
        d++;
    }
    VRAMTransfer_QueueCopy(w->buf + idx[0] * 0x20, 0x20, 0, addrs[0]);
    VRAMTransfer_QueueCopy(w->buf + idx[1] * 0x20, 0x20, 0, addrs[0] + 0x20);
    VRAMTransfer_QueueCopy(w->buf + idx[2] * 0x20, 0x20, 0, addrs[0] + 0x40);
    VRAMTransfer_QueueCopy(w->buf + idx[3] * 0x20, 0x20, 0, addrs[0] + 0x60);
    base = gObjVramBaseTable.base;
    ofs = (u32)(addrs[0] - base) >> (data_02087d84.shift + 5);
    oam = Oam_AllocObj(1, 0);
    yb = (u8)y;
    oam[0] = yb;
    oam[1] = (u8)x;
    oam[0] |= 0x4000;
    oam[1] |= 0x4000;
    a2 = pal << 10;
    a2 |= 0xc000;
    oam[2] = a2 | (ofs & 0x3ff);
    {
        s32 *d2;
        s32 xx2;
        s32 i2;
        xx2 = x + 0x20;
        i2 = 0;
        idx[2] = 0;
        idx[1] = 0;
        idx[0] = 0;
        d2 = idx;
        for (; i2 < 3; i2++) {
            if (xx2 >= 0 && xx2 < 0xf8) {
                *d2 = tiles[4];
            }
            xx2 += 8;
            tiles++;
            d2++;
        }
    }
    VRAMTransfer_QueueCopy(w->buf + idx[0] * 0x20, 0x20, 0, addrs[1]);
    VRAMTransfer_QueueCopy(w->buf + idx[1] * 0x20, 0x20, 0, addrs[1] + 0x20);
    VRAMTransfer_QueueCopy(w->buf + idx[2] * 0x20, 0x20, 0, addrs[1] + 0x40);
    VRAMTransfer_QueueCopy(w->buf, 0x20, 0, addrs[1] + 0x60);
    ofs = (u32)(addrs[1] - base) >> (data_02087d84.shift + 5);
    oam = Oam_AllocObj(1, 0);
    oam[0] = yb;
    x += 0x20;
    oam[1] = (u8)x;
    oam[0] |= 0x4000;
    oam[1] |= 0x4000;
    a2 |= ofs & 0x3ff;
    oam[2] = a2;
}
#pragma thumb reset

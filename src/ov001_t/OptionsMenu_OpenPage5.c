#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x18];
} UnkSpr_022cb134;

typedef struct {
    u8 pad0[0x1838];
    UnkSpr_022cb134 spr[12];
    u32 ids[12];
    u8 pad1bc8[0x1e08 - 0x1bc8];
    void *res1e08;
    u8 pad1e0c[8];
    void *res1e14;
    u8 pad1e18[8];
    void *res1e20;
    void *res1e24;
    void *res1e28;
} UnkWork_022cb134;

typedef struct {
    u8 pad0[0x3ac];
    UnkWork_022cb134 *work;
} UnkStruct_022cb134;

extern u16 data_ov001_02315ff4[];

void OptionsMenu_InitSpriteSub(UnkSpr_022cb134 *s, void *res, u16 id);

#pragma thumb on
void OptionsMenu_OpenPage5(UnkStruct_022cb134 *p) {
    void *res;
    u32 *ids;
    UnkSpr_022cb134 *spr = p->work->spr;
    s32 i;

    ids = p->work->ids;
    for (i = 0; i < 12; i++) {
        switch (i) {
        case 0:
            res = p->work->res1e14;
            break;
        case 1:
            res = p->work->res1e08;
            break;
        case 2:
            res = p->work->res1e28;
            break;
        case 3:
        case 4:
        case 5:
            res = p->work->res1e20;
            break;
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            res = p->work->res1e24;
            break;
        }
        OptionsMenu_InitSpriteSub(&spr[i], res, data_ov001_02315ff4[i]);
        spr[i].flags &= 0x60000;
        spr[i].flags |= 0x20000;
        ids[i] = data_ov001_02315ff4[i];
    }
}
#pragma thumb reset

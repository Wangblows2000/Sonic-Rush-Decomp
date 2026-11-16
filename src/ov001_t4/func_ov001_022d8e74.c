#include <nitro/types.h>

#define REG16(a) (*(vu16 *)(a))

typedef struct {
    u8 pad0[0x18];
    u16 w;
    u16 h;
} UnkRes_022d8e74;

typedef struct {
    u8 pad0[0x40];
} UnkBg_022d8e74;

typedef struct {
    u8 pad0[0x84];
} UnkObj_022d8e74;

typedef struct {
    u8 pad0[0x48];
} UnkSprite_022d8e74;

typedef struct {
    u8 pad0[0xc];
    UnkBg_022d8e74 bgc;
    UnkBg_022d8e74 bg4c;
    UnkBg_022d8e74 bg8c;
    UnkObj_022d8e74 objs[3];
    u8 pad258[4];
    UnkSprite_022d8e74 sprites[19];
    s32 ids[19];
    u8 pad800[0x808 - 0x800];
    UnkRes_022d8e74 *res808;
    UnkRes_022d8e74 *res80c;
    UnkRes_022d8e74 *res810;
    UnkRes_022d8e74 *res814;
    UnkRes_022d8e74 *res818;
    UnkRes_022d8e74 *res81c;
    UnkRes_022d8e74 *res820;
} UnkWork_022d8e74;

typedef struct {
    u8 pad0[4];
    s32 unk4;
} UnkCfg_022d8e74;

extern UnkCfg_022d8e74 data_022c4680;
extern u32 data_0208fe70;

void func_0203473c(void *bg, UnkRes_022d8e74 *res, s32 a, s32 b, s32 c, u16 w, u16 h);
void func_0203438c(void *bg);
void func_02034644(void *obj, UnkRes_022d8e74 *res, s32 a, u8 b, u8 c, u16 w, u16 h);
void func_020342b0(void *obj);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_ov001_022d8248(UnkSprite_022d8e74 *s, UnkRes_022d8e74 *res, s32 id);
void func_ov001_022d8294(UnkSprite_022d8e74 *s, UnkRes_022d8e74 *res, s32 id);

#pragma thumb on
void func_ov001_022d8e74(UnkWork_022d8e74 *w) {
    UnkRes_022d8e74 *res;
    vu32 z0;
    vu32 z1;
    s32 i;

    REG16(0x400000e) = (REG16(0x400000e) & 0x43) | 0x390;
    res = w->res818;
    func_0203473c(&w->bg4c, res, 0x38, 0, 3, res->w, res->h);
    func_0203438c(&w->bg4c);
    REG16(0x400000e) = (REG16(0x400000e) & 0x43) | 0x288;
    if (data_022c4680.unk4 == 0) {
        res = w->res810;
    } else {
        data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
        if ((u16)(data_0208fe70 >> 16) % 2 == 0) {
            res = w->res810;
        } else {
            res = w->res81c;
        }
    }
    func_0203473c(&w->bgc, res, 0x38, 0, 3, res->w, res->h);
    func_0203438c(&w->bgc);
    res = w->res814;
    func_0203473c(&w->bg8c, res, 0x38, 1, 3, res->w, res->h);
    func_0203438c(&w->bg8c);
    z0 = 0;
    MIi_CpuClear32(z0, (void *)0x6000800, 0x800);
    z1 = 0;
    MIi_CpuClear32(z1, (void *)0x6200800, 0x800);
    res = w->res820;
    for (i = 0; i < 3; i++) {
        func_02034644(&w->objs[i], res, 0x38, i, i, res->w, res->h);
        func_020342b0(&w->objs[i]);
    }
    res = w->res808;
    func_ov001_022d8248(&w->sprites[0], res, 0);
    w->ids[0] = 0;
    func_ov001_022d8248(&w->sprites[1], res, 3);
    w->ids[1] = 3;
    res = w->res80c;
    func_ov001_022d8294(&w->sprites[2], res, 0);
    w->ids[2] = 0;
    func_ov001_022d8294(&w->sprites[3], res, 1);
    w->ids[3] = 1;
    func_ov001_022d8294(&w->sprites[4], res, 2);
    w->ids[4] = 2;
    func_ov001_022d8294(&w->sprites[5], res, 3);
    w->ids[5] = 3;
    func_ov001_022d8294(&w->sprites[6], res, 4);
    w->ids[6] = 4;
    func_ov001_022d8294(&w->sprites[7], res, 5);
    w->ids[7] = 5;
    func_ov001_022d8294(&w->sprites[8], res, 6);
    w->ids[8] = 6;
    func_ov001_022d8248(&w->sprites[9], res, 7);
    w->ids[9] = 7;
    func_ov001_022d8248(&w->sprites[10], res, 10);
    w->ids[10] = 10;
    func_ov001_022d8248(&w->sprites[11], res, 13);
    w->ids[11] = 13;
    func_ov001_022d8248(&w->sprites[12], res, 16);
    w->ids[12] = 16;
    func_ov001_022d8248(&w->sprites[13], res, 19);
    w->ids[13] = 19;
    func_ov001_022d8248(&w->sprites[14], res, 22);
    w->ids[14] = 22;
    func_ov001_022d8248(&w->sprites[15], res, 25);
    w->ids[15] = 25;
    func_ov001_022d8248(&w->sprites[16], res, 28);
    w->ids[16] = 28;
    func_ov001_022d8248(&w->sprites[17], res, 31);
    w->ids[17] = 31;
    func_ov001_022d8248(&w->sprites[18], res, 34);
    w->ids[18] = 34;
}
#pragma thumb reset

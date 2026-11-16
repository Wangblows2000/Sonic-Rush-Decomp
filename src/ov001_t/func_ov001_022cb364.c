#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x18];
} UnkSpr_022cb364;

typedef struct {
    u8 pad0[0x408];
    UnkSpr_022cb364 spr[5];
    u32 ids[5];
    u8 padx[0x1e08 - 0x408 - 5 * 0x4c];
    void *res1e08;
    u8 pad1e0c[8];
    void *res1e14;
} UnkWork_022cb364;

typedef struct {
    u8 pad0[0x3ac];
    UnkWork_022cb364 *work;
} UnkStruct_022cb364;

extern u16 data_ov001_02315fc0[];

void func_ov001_022ce670(UnkSpr_022cb364 *s, void *res, u16 id);

#pragma thumb on
void func_ov001_022cb364(UnkStruct_022cb364 *p) {
    void *res;
    u32 *ids;
    UnkSpr_022cb364 *spr = p->work->spr;
    s32 i;

    ids = p->work->ids;
    for (i = 0; i < 5; i++) {
        switch (i) {
        case 1:
            res = p->work->res1e08;
            break;
        case 0:
        case 2:
        case 3:
        case 4:
            res = p->work->res1e14;
            break;
        }
        func_ov001_022ce670(&spr[i], res, data_ov001_02315fc0[i]);
        spr[i].flags &= 0x60000;
        spr[i].flags |= 0x20000;
        ids[i] = data_ov001_02315fc0[i];
    }
}
#pragma thumb reset

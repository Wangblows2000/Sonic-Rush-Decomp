#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x18];
} UnkSpr_022cb438;

typedef struct {
    u8 pad0[0x584];
    UnkSpr_022cb438 spr[8];
    u32 ids[8];
    u8 padx[0x1e08 - 0x584 - 8 * 0x4c];
    void *res1e08;
    u8 pad1e0c[8];
    void *res1e14;
} UnkWork_022cb438;

typedef struct {
    u8 pad0[0x3ac];
    UnkWork_022cb438 *work;
} UnkStruct_022cb438;

extern u16 data_ov001_02315fcc[];

void func_ov001_022ce670(UnkSpr_022cb438 *s, void *res, u16 id);

#pragma thumb on
void func_ov001_022cb438(UnkStruct_022cb438 *p) {
    void *res;
    u32 *ids;
    UnkSpr_022cb438 *spr = p->work->spr;
    s32 i;

    ids = p->work->ids;
    for (i = 0; i < 8; i++) {
        switch (i) {
        case 1:
        case 2:
            res = p->work->res1e08;
            break;
        case 0:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            res = p->work->res1e14;
            break;
        }
        func_ov001_022ce670(&spr[i], res, data_ov001_02315fcc[i]);
        spr[i].flags &= 0x60000;
        spr[i].flags |= 0x20000;
        ids[i] = data_ov001_02315fcc[i];
    }
}
#pragma thumb reset

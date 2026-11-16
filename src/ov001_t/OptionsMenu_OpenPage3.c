#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x18];
} UnkSpr_022cb260;

typedef struct {
    u8 pad0[0x7e4];
    UnkSpr_022cb260 spr[14];
    u32 ids[14];
    u8 padc0c[0x1e10 - 0xc0c];
    void *res1e10;
    void *res1e14;
    void *res1e18;
} UnkWork_022cb260;

typedef struct {
    u8 pad0[0x1e8];
    s32 unk1e8;
    u8 pad1ec[0x3ac - 0x1ec];
    UnkWork_022cb260 *work;
} UnkStruct_022cb260;

extern u16 data_ov001_0231600c[];

void OptionsMenu_Unk_ov001_022ce670(UnkSpr_022cb260 *s, void *res, u16 id);

#pragma thumb on
void OptionsMenu_OpenPage3(UnkStruct_022cb260 *p) {
    void *res;
    u32 *ids;
    UnkSpr_022cb260 *spr = p->work->spr;
    s32 i;

    ids = p->work->ids;
    for (i = 0; i < 14; i++) {
        switch (i) {
        case 0:
            res = p->work->res1e14;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
            res = p->work->res1e10;
            break;
        case 13:
            res = p->work->res1e18;
            break;
        }
        OptionsMenu_Unk_ov001_022ce670(&spr[i], res, data_ov001_0231600c[i]);
        spr[i].flags &= 0x60000;
        ids[i] = data_ov001_0231600c[i];
    }
    p->unk1e8 = 0;
}
#pragma thumb reset

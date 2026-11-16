#include <nitro/types.h>

typedef struct { s32 unk0; s32 unk4; } UnkWork_02051ca4;
typedef struct { s32 unk0; } UnkStruct_022c4560;
extern UnkWork_02051ca4 *data_022c465c;
extern UnkStruct_022c4560 data_022c4560;
extern void *data_02250458;

void func_02052000(void);
void func_0201e7cc(s32 id, void *p);

#pragma thumb on
void func_02051ca4(s32 stage) {
    s32 grp;
    s32 id;

    if (stage >= 0x17)
        return;
    if (stage <= 6)
        grp = 0;
    else if (stage <= 9)
        grp = 1;
    else if (stage <= 0x13)
        grp = 2;
    else if (stage <= 0x16)
        grp = 3;
    else
        return;
    if (grp == data_022c465c->unk0)
        return;
    func_02052000();
    data_022c465c->unk4 = 7;
    switch (grp) {
    case 0:
        id = 1;
        break;
    case 1:
        id = 0x15;
        break;
    case 2:
        if (data_022c4560.unk0 == 0)
            id = 2;
        else
            id = 3;
        break;
    case 3:
        id = 0xe;
        break;
    }
    func_0201e7cc(id, data_02250458);
    data_022c465c->unk0 = grp;
}
#pragma thumb reset

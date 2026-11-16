#include <nitro/types.h>

typedef struct {
    u8 data[0x48];
} UnkSpr_022cb400;

typedef struct {
    u8 pad0[0x584];
    UnkSpr_022cb400 spr[8];
    s32 ids[8];
} UnkWork_022cb400;

typedef struct {
    u8 pad0[0x3ac];
    UnkWork_022cb400 *work;
} UnkStruct_022cb400;

void func_ov001_022ce374(UnkSpr_022cb400 *s);

#pragma thumb on
void func_ov001_022cb400(UnkStruct_022cb400 *p) {
    UnkSpr_022cb400 *spr = p->work->spr;
    s32 *ids = p->work->ids;
    s32 i;

    for (i = 0; i < 8; i++) {
        func_ov001_022ce374(&spr[i]);
        ids[i] = -1;
    }
}
#pragma thumb reset

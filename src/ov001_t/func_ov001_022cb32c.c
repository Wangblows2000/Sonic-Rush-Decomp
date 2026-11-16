#include <nitro/types.h>

typedef struct {
    u8 data[0x48];
} UnkSpr_022cb32c;

typedef struct {
    u8 pad0[0x408];
    UnkSpr_022cb32c spr[5];
    s32 ids[5];
} UnkWork_022cb32c;

typedef struct {
    u8 pad0[0x3ac];
    UnkWork_022cb32c *work;
} UnkStruct_022cb32c;

void func_ov001_022ce374(UnkSpr_022cb32c *s);

#pragma thumb on
void func_ov001_022cb32c(UnkStruct_022cb32c *p) {
    UnkSpr_022cb32c *spr = p->work->spr;
    s32 *ids = p->work->ids;
    s32 i;

    for (i = 0; i < 5; i++) {
        func_ov001_022ce374(&spr[i]);
        ids[i] = -1;
    }
}
#pragma thumb reset

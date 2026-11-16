/* paramclone of func_ov001_022cb32c (src/ov001_t/func_ov001_022cb32c.c); immediates and relocations remapped. */
#include <nitro/types.h>

typedef struct {
    u8 data[0x48];
} UnkSpr_022cb228;

typedef struct {
    u8 pad0[0x7e4];
    UnkSpr_022cb228 spr[14];
    s32 ids[14];
} UnkWork_022cb228;

typedef struct {
    u8 pad0[0x3ac];
    UnkWork_022cb228 *work;
} UnkStruct_022cb228;

void func_ov001_022ce374(UnkSpr_022cb228 *s);

#pragma thumb on
void func_ov001_022cb228(UnkStruct_022cb228 *p) {
    UnkSpr_022cb228 *spr = p->work->spr;
    s32 *ids = p->work->ids;
    s32 i;

    for (i = 0; i < 14; i++) {
        func_ov001_022ce374(&spr[i]);
        ids[i] = -1;
    }
}
#pragma thumb reset

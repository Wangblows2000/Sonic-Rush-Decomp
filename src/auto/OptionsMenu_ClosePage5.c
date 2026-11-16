/* paramclone of OptionsMenu_ClosePage1 (src/ov001_t/OptionsMenu_ClosePage1.c); immediates and relocations remapped. */
#include <nitro/types.h>

typedef struct {
    u8 data[0x48];
} UnkSpr_022cb0fc;

typedef struct {
    u8 pad0[0x1838];
    UnkSpr_022cb0fc spr[12];
    s32 ids[12];
} UnkWork_022cb0fc;

typedef struct {
    u8 pad0[0x3ac];
    UnkWork_022cb0fc *work;
} UnkStruct_022cb0fc;

void Options_ReleaseSprite(UnkSpr_022cb0fc *s);

#pragma thumb on
void OptionsMenu_ClosePage5(UnkStruct_022cb0fc *p) {
    UnkSpr_022cb0fc *spr = p->work->spr;
    s32 *ids = p->work->ids;
    s32 i;

    for (i = 0; i < 12; i++) {
        Options_ReleaseSprite(&spr[i]);
        ids[i] = -1;
    }
}
#pragma thumb reset

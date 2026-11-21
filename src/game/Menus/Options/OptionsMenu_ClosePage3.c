/* paramclone of OptionsMenu_ClosePage1 (src/ov001_t/OptionsMenu_ClosePage1.c); immediates and relocations remapped. */
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

void Options_ReleaseSprite(UnkSpr_022cb228 *s);

#pragma thumb on
void OptionsMenu_ClosePage3(UnkStruct_022cb228 *p) {
    UnkSpr_022cb228 *spr = p->work->spr;
    s32 *ids = p->work->ids;
    s32 i;

    for (i = 0; i < 14; i++) {
        Options_ReleaseSprite(&spr[i]);
        ids[i] = -1;
    }
}
#pragma thumb reset

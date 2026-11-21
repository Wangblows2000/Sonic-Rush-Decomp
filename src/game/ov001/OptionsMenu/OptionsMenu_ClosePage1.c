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

void Options_ReleaseSprite(UnkSpr_022cb32c *s);

#pragma thumb on
void OptionsMenu_ClosePage1(UnkStruct_022cb32c *p) {
    UnkSpr_022cb32c *spr = p->work->spr;
    s32 *ids = p->work->ids;
    s32 i;

    for (i = 0; i < 5; i++) {
        Options_ReleaseSprite(&spr[i]);
        ids[i] = -1;
    }
}
#pragma thumb reset

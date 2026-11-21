#include <nitro/types.h>

typedef struct {
    u8 pad0[0x48];
} UnkStruct_TryAgainSprite;

typedef struct {
    u8 pad0[0xC];
    UnkStruct_TryAgainSprite sprites[6];
    u8 padBC[0x5E0 - 0xC - 6 * 0x48];
    void *unk5E0;
    void *unk5E4;
    u32 unk5E8[3];
    u32 unk5F4[4];
} UnkStruct_TryAgain;

void TryAgain_InitMainSprite(UnkStruct_TryAgainSprite *s, void *res, u32 idx, s32 mode, u16 a, u16 b);
void TryAgain_InitSubSprite(UnkStruct_TryAgainSprite *s, void *res, u16 idx, s32 prio, u16 x, u16 y);

#pragma thumb on
#pragma opt_propagation off
void TryAgain_InitSprites(UnkStruct_TryAgain *w)
{
    void *res;
    s32 i;
    s32 off;
    UnkStruct_TryAgainSprite *spr;
    UnkStruct_TryAgain *p;

    res = w->unk5E4;
    spr = &w->sprites[0];
    TryAgain_InitMainSprite(spr, res, 0, 0, 0, 0);
    TryAgain_InitSubSprite(&w->sprites[1], w->unk5E0, 0, 0, 0, 4);
    res = w->unk5E4;
    i = 0; off = 0; p = w;
    do {
        TryAgain_InitSubSprite((UnkStruct_TryAgainSprite *)((u8 *)w->sprites + (off + 0x90)), res, p->unk5F4[0], 0, 0, 8);
        off += 0x48;
        p = (UnkStruct_TryAgain *)((u8 *)p + 4);
        i++;
    } while (i < 4);
}
#pragma thumb reset

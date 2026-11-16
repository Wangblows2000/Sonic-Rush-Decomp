#include <nitro/types.h>

typedef struct {
    u8 pad[0x2c];
    u32 flags;
} UnkObj_022c98a4;

typedef struct {
    s32 unk0;
    s32 state;
} UnkSub_022c98a4;

typedef struct {
    u8 pad0[0x1ec];
    UnkSub_022c98a4 sub;
    u8 pad1f4[0x3ac - 0x1f4];
    u8 *work;
} UnkStruct_022c98a4;

void OptionsMenu_DrawSprite(UnkStruct_022c98a4 *p, s16 x, s16 y, s32 a, s32 b);

#pragma thumb on
void OptionsMenu_Unk_ov001_022c98a4(UnkStruct_022c98a4 *p, s16 x, s16 y) {
    UnkObj_022c98a4 *o = (UnkObj_022c98a4 *)(p->work + 0x1a30);

    switch (p->sub.state) {
    case 5:
        o->flags &= ~4;
        OptionsMenu_DrawSprite(p, x, y, 7, 1);
        break;
    case 6:
        o->flags |= 4;
        OptionsMenu_DrawSprite(p, x, y, 7, 2);
        break;
    case 7:
        o->flags &= ~4;
        OptionsMenu_DrawSprite(p, x, y, 7, 3);
        break;
    }
}
#pragma thumb reset

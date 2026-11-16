#include <nitro/types.h>

typedef struct {
    u8 pad[0x2c];
    u32 flags;
} UnkObj_022c9570;

typedef struct {
    s32 unk0;
    s32 state;
    s32 unk8;
    s32 mode;
} UnkSub_022c9570;

typedef struct {
    u8 pad0[0x14];
    u32 unk14;
    u8 pad18[0x1ec - 0x18];
    UnkSub_022c9570 sub;
    u8 pad1fc[0x3ac - 0x1fc];
    u8 *unk3ac;
} UnkStruct_022c9570;

void OptionsMenu_DrawSprite(UnkStruct_022c9570 *p, s16 x, s16 y, s32 a, s32 b);

#pragma thumb on
void OptionsMenu_Unk_ov001_022c970c(UnkStruct_022c9570 *p, s16 x, s16 y) {
    UnkSub_022c9570 *s = &p->sub;
    UnkObj_022c9570 *a = (UnkObj_022c9570 *)(p->unk3ac + 0x1a78);
    UnkObj_022c9570 *b = (UnkObj_022c9570 *)(p->unk3ac + 0x1880);

    switch (s->state) {
    case 0:
        if (s->mode == 0) {
            a->flags &= ~4;
            OptionsMenu_DrawSprite(p, x, y, 8, 8);
        } else {
            a->flags &= ~4;
            OptionsMenu_DrawSprite(p, x, y, 8, 7);
        }
        break;
    case 1:
        if (s->mode == 0) {
            b->flags &= ~4;
            if (p->unk14 < 4) {
                OptionsMenu_DrawSprite(p, x + 0x38, y + 0x77, 1, 4);
            } else {
                OptionsMenu_DrawSprite(p, x + 0x38, y + 0x77, 1, 3);
            }
            a->flags &= ~4;
            OptionsMenu_DrawSprite(p, x, y, 8, 8);
        } else {
            a->flags &= ~4;
            OptionsMenu_DrawSprite(p, x, y, 8, 7);
        }
        break;
    case 2:
        if (s->mode == 0) {
            b->flags &= ~4;
            OptionsMenu_DrawSprite(p, x + 0x38, y + 0x77, 1, 3);
            a->flags |= 4;
            OptionsMenu_DrawSprite(p, x, y, 8, 9);
        } else {
            a->flags &= ~4;
            OptionsMenu_DrawSprite(p, x, y, 8, 7);
        }
        break;
    case 3:
        if (s->mode == 0) {
            a->flags &= ~4;
            OptionsMenu_DrawSprite(p, x, y, 8, 8);
        } else {
            a->flags &= ~4;
            OptionsMenu_DrawSprite(p, x, y, 8, 7);
        }
        break;
    }
}
#pragma thumb reset

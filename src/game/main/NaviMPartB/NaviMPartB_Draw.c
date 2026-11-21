#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x40];
} UnkSpr_02063890;

typedef struct {
    u8 pad0[0x400];
    UnkSpr_02063890 cursor;
    UnkSpr_02063890 icons[7];
} UnkRes_02063890;

typedef struct {
    UnkRes_02063890 *res;
    u8 pad4[6];
    u16 flags;
    s32 sel;
} UnkWork_02063890;

typedef struct {
    s16 x;
    s16 y;
} UnkPos_02063890;

extern u16 gNaviMNewEmeraldMask;
extern UnkPos_02063890 data_02077894[];

void AnimatorSprite_Draw(UnkSpr_02063890 *s);

void NaviMPartB_Draw(UnkWork_02063890 *w) {
    u16 mask;
    UnkSpr_02063890 *icon;
    UnkSpr_02063890 *cur;
    s32 i;

    mask = w->flags & ~gNaviMNewEmeraldMask;
    cur = &w->res->cursor;
    icon = w->res->icons;
    for (i = 0; i < 7; i++) {
        if (mask & (1 << i)) {
            icon->x = data_02077894[i].x;
            icon->y = data_02077894[i].y;
            AnimatorSprite_Draw(icon);
        } else if (i != w->sel) {
            cur->x = data_02077894[i].x;
            cur->y = data_02077894[i].y;
            AnimatorSprite_Draw(cur);
        }
        icon++;
    }
}

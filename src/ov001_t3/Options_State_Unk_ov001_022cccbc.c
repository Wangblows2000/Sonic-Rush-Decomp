#include <nitro/types.h>

typedef struct {
    s32 id;
    s16 x;
    s16 y;
    u8 pad8[0x28];
} UnkItem_022cccbc;

typedef struct {
    s32 mode;
} UnkObj_022cccbc;

typedef struct UnkStruct_022cccbc UnkStruct_022cccbc;
typedef void (*UnkFn_022cccbc)(UnkStruct_022cccbc *p, s32 x, s32 y, s32 sel);

struct UnkStruct_022cccbc {
    u8 pad0[4];
    s32 cur;
    u8 pad8[0x14 - 8];
    u32 timer;
    u8 pad18[0x208 - 0x18];
    s16 ax;
    s16 ay;
    u8 pad20c[0x234 - 0x20c];
    s16 bx;
    s16 by;
    u8 pad238[0x260 - 0x238];
    s16 cx;
    s16 cy;
    u8 pad264[0x28c - 0x264];
    UnkItem_022cccbc items[6];
    UnkObj_022cccbc *obj;
};

extern UnkFn_022cccbc gOptionsMenuItemLabelDrawTable[];

void OptionsMenu_DrawItemSprite(UnkObj_022cccbc *o, s32 id, s32 x, s32 y, s32 sel);
void OptionsMenu_UpdatePage(UnkStruct_022cccbc *p, s32 x, s32 y);
void OptionsMenu_Unk_ov001_022cbe98(UnkObj_022cccbc *o, s32 x, s32 y);
void OptionsMenu_Unk_ov001_022cbe50(UnkObj_022cccbc *o, s32 x, s32 y);
void OptionsMenu_Unk_ov001_022cbdf0(UnkObj_022cccbc *o, s32 x, s32 y, s32 a);
void OptionsMenu_UpdateAnims(UnkStruct_022cccbc *p);
void Options_Unk_ov001_022cdcf4(void);

typedef struct {
    u8 pad0[0x10];
    UnkStruct_022cccbc *work;
} UnkTask_022cccbc;

extern UnkTask_022cccbc *gTaskCurrent;

#pragma thumb on
void Options_State_Unk_ov001_022cccbc(void) {
    UnkStruct_022cccbc *p;
    s16 y;
    s32 i;
    UnkItem_022cccbc *it;

    p = gTaskCurrent->work;
    OptionsMenu_UpdateAnims(p);
    y = 0x18;
    for (i = 0, it = p->items; i < 6; it++, i++) {
        if (it->id == 7) continue;
        if (i == p->cur) {
            UnkFn_022cccbc fn = gOptionsMenuItemLabelDrawTable[it->id];
            if (fn != NULL) fn(p, it->x, y, 1);
            OptionsMenu_DrawItemSprite(p->obj, it->id, it->x, y, 1);
        } else {
            UnkFn_022cccbc fn = gOptionsMenuItemLabelDrawTable[it->id];
            if (fn != NULL) fn(p, it->x, y, 0);
            OptionsMenu_DrawItemSprite(p->obj, it->id, it->x, y, 0);
        }
        y += 0x18;
    }
    OptionsMenu_UpdatePage(p, p->ax, p->ay);
    OptionsMenu_Unk_ov001_022cbe98(p->obj, p->ax, p->ay);
    OptionsMenu_Unk_ov001_022cbe50(p->obj, p->bx, p->by);
    OptionsMenu_Unk_ov001_022cbdf0(p->obj, p->cx, p->cy, 1);
    p->timer++;
    if (p->timer >= 10) {
        Options_Unk_ov001_022cdcf4();
    }
}
#pragma thumb reset

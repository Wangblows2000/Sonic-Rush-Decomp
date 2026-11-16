#include <nitro/types.h>

typedef struct { u8 pad0[0x10]; s32 pal; } UnkRes_0205df04;
typedef struct { u8 pad0[0x14]; s32 unk14; u8 pad18[4]; u32 unk1c; u8 pad20[0x20]; } UnkBg_0205df04;

typedef struct {
    u8 x1;
    u8 y1;
    u8 pad2[2];
    u8 x2;
    u8 y2;
    u8 pad6[2];
    u8 in;
    u8 pad9;
    u8 out;
    u8 padb;
    u32 enable;
} UnkWin_0205df04;

typedef struct {
    u8 pad0[0x10];
    UnkWin_0205df04 win;
} UnkDisp_0205df04;

typedef struct {
    u8 pad0[8];
    u16 unk8;
    u16 unka;
    u16 unkc;
    u16 unke;
} UnkBlend_0205df04;

typedef struct {
    u8 pad0[0x47c];
    s32 idx;
    u8 pad480[0x18];
    UnkRes_0205df04 *res498;
    UnkRes_0205df04 *res49c;
} UnkWork_0205df04;

extern u16 data_02076b48[];
extern UnkDisp_0205df04 *gDisplayTable[];
extern UnkBlend_0205df04 gDisplaySub;
extern UnkBlend_0205df04 gDisplayMain;

void Background_Init(UnkBg_0205df04 *bg, void *res, s32 a, s32 b, s32 c, s32 d, s32 e);
void Background_Draw(UnkBg_0205df04 *bg);
void PaletteTransfer_QueueCopy(void *p, s32 a, s32 b, u32 c);

void StageTitleCard_InitBgBoss(UnkWork_0205df04 *w) {
    u32 ofs;
    u8 i;
    UnkDisp_0205df04 *d;
    UnkWin_0205df04 *win;
    UnkBg_0205df04 bg;

    ofs = data_02076b48[w->idx];
    Background_Init(&bg, w->res49c, 1, 0, 2, 0x20, 0x20);
    Background_Draw(&bg);
    PaletteTransfer_QueueCopy((void *)((u32)w->res498 + w->res498->pal + 4 + ofs), 0x10, bg.unk14, 0x05000000);
    Background_Init(&bg, w->res498, 1, 1, 3, 0x20, 0x18);
    bg.unk1c += 0x1c0;
    Background_Draw(&bg);
    PaletteTransfer_QueueCopy((void *)((u32)w->res498 + w->res498->pal + 4 + ofs), 0x10, bg.unk14, 0x05000400);
    Background_Init(&bg, w->res49c, 1, 1, 2, 0x20, 0x20);
    Background_Draw(&bg);
    for (i = 0; i < 2; i++) {
        d = gDisplayTable[i];
        win = &d->win;
        win->y1 = 0;
        win->y2 = 0;
        win->x1 = 0x38;
        win->x2 = 0xc0;
        win->in = 0xff;
        win->out = 0xfb;
        win->enable = 1;
    }
    gDisplayMain.unk8 = 0;
    gDisplaySub.unk8 = 0;
    gDisplayMain.unka = 0;
    gDisplaySub.unka = 0;
    gDisplaySub.unkc = 0;
    gDisplaySub.unke = 0;
}

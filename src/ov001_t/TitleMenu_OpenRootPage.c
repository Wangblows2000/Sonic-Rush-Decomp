#include <nitro/types.h>

typedef struct {
    s32 id;
    s32 unk4;
    u8 anim[0x2c];
    s32 unk34;
    void (*cb)(void);
} UnkItem_022d7210;

typedef struct {
    u8 header[0x14];
    UnkItem_022d7210 items[6];
    s32 unk17c;
    s32 unk180;
    s32 unk184;
    s32 unk188;
    s32 unk18c;
    s32 unk190;
    u8 pad194[4];
} UnkMenu_022d7210;

typedef struct {
    u8 pad0[8];
    s32 unk8;
} UnkWork_022d7210;

void func_ov001_022d7ee8(UnkMenu_022d7210 *m, s32 a, s32 id0, s32 id1, s32 id2, s32 id3, UnkWork_022d7210 *res);
void func_ov001_022d8060(UnkMenu_022d7210 *m, s32 a, s32 id0, s32 id1, s32 id2, s32 id3, UnkWork_022d7210 *res);
void TitleMenu_CreatePageTask(UnkMenu_022d7210 *m, UnkWork_022d7210 *p, s32 b);
void TitleMenu_SelectGameStart(void);
void TitleMenu_OpenVsPage(void *p, BOOL flag, s32 b);
void TitleMenu_OpenPlayerSelectPage(void *p, BOOL flag, s32 b);
void TitleMenu_SelectOptions(void);
void TitleMenu_SelectTitleScreen(void);

#pragma thumb on
void TitleMenu_OpenRootPage(UnkWork_022d7210 *p, BOOL flag, s32 b) {
    UnkMenu_022d7210 menu;

    if (flag) {
        if (p->unk8) {
            func_ov001_022d7ee8(&menu, 2, 9, 10, 11, 12, p);
        } else {
            func_ov001_022d7ee8(&menu, 2, 9, 10, 12, 0x14, p);
        }
    } else {
        if (p->unk8) {
            func_ov001_022d8060(&menu, 2, 9, 10, 11, 12, p);
        } else {
            func_ov001_022d8060(&menu, 2, 9, 10, 12, 0x14, p);
        }
    }
    menu.items[0].cb = TitleMenu_SelectGameStart;
    menu.items[1].cb = (void (*)(void))TitleMenu_OpenVsPage;
    if (p->unk8) {
        menu.items[2].cb = (void (*)(void))TitleMenu_OpenPlayerSelectPage;
        menu.items[3].cb = TitleMenu_SelectOptions;
    } else {
        menu.items[2].cb = TitleMenu_SelectOptions;
        menu.items[3].cb = NULL;
    }
    menu.items[4].cb = TitleMenu_SelectTitleScreen;
    menu.items[0].unk4 = 1;
    menu.items[1].unk4 = 0;
    if (p->unk8) {
        menu.items[2].unk4 = 0;
        menu.items[3].unk4 = 2;
    } else {
        menu.items[2].unk4 = 2;
        menu.items[3].unk4 = 0;
    }
    menu.items[4].unk4 = 1;
    menu.items[0].unk34 = -1;
    menu.items[1].unk34 = -1;
    menu.items[2].unk34 = -1;
    menu.items[3].unk34 = -1;
    menu.items[4].unk34 = -1;
    menu.unk17c = 0;
    menu.unk180 = 8;
    menu.unk184 = 8;
    menu.unk188 = 1;
    if (p->unk8) {
        menu.unk18c = 2;
        menu.unk190 = 8;
    } else {
        menu.unk18c = 8;
        menu.unk190 = 8;
    }
    TitleMenu_CreatePageTask(&menu, p, b);
}
#pragma thumb reset

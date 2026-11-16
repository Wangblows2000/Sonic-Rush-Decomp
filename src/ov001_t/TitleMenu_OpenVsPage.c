#include <nitro/types.h>

typedef struct {
    s32 id;
    s32 unk4;
    u8 anim[0x2c];
    s32 unk34;
    void (*cb)(void);
} UnkItem_022d717c;

typedef struct {
    u8 header[0x14];
    UnkItem_022d717c items[6];
    s32 unk17c;
    s32 unk180;
    s32 unk184;
    s32 unk188;
    s32 unk18c;
    s32 unk190;
    u8 pad194[4];
} UnkMenu_022d717c;

void func_ov001_022d7ee8(UnkMenu_022d717c *m, s32 a, s32 id0, s32 id1, s32 id2, s32 id3, void *res);
void func_ov001_022d8060(UnkMenu_022d717c *m, s32 a, s32 id0, s32 id1, s32 id2, s32 id3, void *res);
void TitleMenu_CreatePageTask(UnkMenu_022d717c *m, void *p, s32 b);
void TitleMenu_OpenVsSubPage(void *p, BOOL flag, s32 b);
void TitleMenu_SelectVsMode0(void);
void TitleMenu_OpenRootPage(void);

#pragma thumb on
void TitleMenu_OpenVsPage(void *p, BOOL flag, s32 b) {
    UnkMenu_022d717c menu;

    if (flag) {
        func_ov001_022d7ee8(&menu, 3, 0xd, 0xe, 0x14, 0x14, p);
    } else {
        func_ov001_022d8060(&menu, 3, 0xd, 0xe, 0x14, 0x14, p);
    }
    menu.items[0].cb = (void (*)(void))TitleMenu_OpenVsSubPage;
    menu.items[1].cb = TitleMenu_SelectVsMode0;
    menu.items[2].cb = NULL;
    menu.items[3].cb = NULL;
    menu.items[4].cb = TitleMenu_OpenRootPage;
    menu.items[0].unk4 = 0;
    menu.items[1].unk4 = 4;
    menu.items[2].unk4 = 0;
    menu.items[3].unk4 = 0;
    menu.items[4].unk4 = 0;
    menu.items[0].unk34 = -1;
    menu.items[1].unk34 = -1;
    menu.items[2].unk34 = -1;
    menu.items[3].unk34 = -1;
    menu.items[4].unk34 = 1;
    menu.unk17c = 1;
    menu.unk180 = 0;
    menu.unk184 = 3;
    menu.unk188 = 6;
    menu.unk18c = 8;
    menu.unk190 = 8;
    TitleMenu_CreatePageTask(&menu, p, b);
}
#pragma thumb reset

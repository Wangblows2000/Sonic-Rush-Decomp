#include <nitro/types.h>

typedef struct {
    s32 id;
    s32 unk4;
    u8 anim[0x2c];
    s32 unk34;
    void (*cb)(void);
} UnkItem_022d7058;

typedef struct {
    u8 header[0x14];
    UnkItem_022d7058 items[6];
    s32 unk17c;
    s32 unk180;
    s32 unk184;
    s32 unk188;
    s32 unk18c;
    s32 unk190;
    u8 pad194[4];
} UnkMenu_022d7058;

void TitleMenu_Unk_ov001_022d7ee8(UnkMenu_022d7058 *m, s32 a, s32 id0, s32 id1, s32 id2, s32 id3, void *res);
void TitleMenu_Unk_ov001_022d8060(UnkMenu_022d7058 *m, s32 a, s32 id0, s32 id1, s32 id2, s32 id3, void *res);
void TitleMenu_CreatePageTask(UnkMenu_022d7058 *m, void *p, s32 b);
void TitleMenu_SelectVsMode1(void);
void TitleMenu_SelectVsMode2(void);
void TitleMenu_OpenVsPage(void);

#pragma thumb on
void TitleMenu_OpenVsSubPage(void *p, BOOL flag, s32 b) {
    UnkMenu_022d7058 menu;

    if (flag) {
        TitleMenu_Unk_ov001_022d7ee8(&menu, 5, 0x11, 0x12, 0x14, 0x14, p);
    } else {
        TitleMenu_Unk_ov001_022d8060(&menu, 5, 0x11, 0x12, 0x14, 0x14, p);
    }
    menu.items[0].cb = TitleMenu_SelectVsMode1;
    menu.items[1].cb = TitleMenu_SelectVsMode2;
    menu.items[2].cb = NULL;
    menu.items[3].cb = NULL;
    menu.items[4].cb = TitleMenu_OpenVsPage;
    menu.items[0].unk4 = 4;
    menu.items[1].unk4 = 4;
    menu.items[2].unk4 = 0;
    menu.items[3].unk4 = 0;
    menu.items[4].unk4 = 0;
    menu.items[0].unk34 = -1;
    menu.items[1].unk34 = -1;
    menu.items[2].unk34 = -1;
    menu.items[3].unk34 = -1;
    menu.items[4].unk34 = 0;
    menu.unk17c = 3;
    menu.unk180 = 1;
    menu.unk184 = 4;
    menu.unk188 = 5;
    menu.unk18c = 8;
    menu.unk190 = 8;
    TitleMenu_CreatePageTask(&menu, p, b);
}
#pragma thumb reset

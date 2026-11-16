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

void func_ov001_022d7ee8(UnkMenu_022d7058 *m, s32 a, s32 id0, s32 id1, s32 id2, s32 id3, void *res);
void func_ov001_022d8060(UnkMenu_022d7058 *m, s32 a, s32 id0, s32 id1, s32 id2, s32 id3, void *res);
void func_ov001_022d81c4(UnkMenu_022d7058 *m, void *p, s32 b);
void func_ov001_022d6fc0(void);
void func_ov001_022d6f74(void);
void func_ov001_022d717c(void);

#pragma thumb on
void func_ov001_022d7058(void *p, BOOL flag, s32 b) {
    UnkMenu_022d7058 menu;

    if (flag) {
        func_ov001_022d7ee8(&menu, 5, 0x11, 0x12, 0x14, 0x14, p);
    } else {
        func_ov001_022d8060(&menu, 5, 0x11, 0x12, 0x14, 0x14, p);
    }
    menu.items[0].cb = func_ov001_022d6fc0;
    menu.items[1].cb = func_ov001_022d6f74;
    menu.items[2].cb = NULL;
    menu.items[3].cb = NULL;
    menu.items[4].cb = func_ov001_022d717c;
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
    func_ov001_022d81c4(&menu, p, b);
}
#pragma thumb reset

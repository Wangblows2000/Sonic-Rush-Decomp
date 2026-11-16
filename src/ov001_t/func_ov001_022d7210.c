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
void func_ov001_022d81c4(UnkMenu_022d7210 *m, UnkWork_022d7210 *p, s32 b);
void func_ov001_022d6f64(void);
void func_ov001_022d717c(void *p, BOOL flag, s32 b);
void func_ov001_022d70ec(void *p, BOOL flag, s32 b);
void func_ov001_022d6f4c(void);
void func_ov001_022d6f6c(void);

#pragma thumb on
void func_ov001_022d7210(UnkWork_022d7210 *p, BOOL flag, s32 b) {
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
    menu.items[0].cb = func_ov001_022d6f64;
    menu.items[1].cb = (void (*)(void))func_ov001_022d717c;
    if (p->unk8) {
        menu.items[2].cb = (void (*)(void))func_ov001_022d70ec;
        menu.items[3].cb = func_ov001_022d6f4c;
    } else {
        menu.items[2].cb = func_ov001_022d6f4c;
        menu.items[3].cb = NULL;
    }
    menu.items[4].cb = func_ov001_022d6f6c;
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
    func_ov001_022d81c4(&menu, p, b);
}
#pragma thumb reset

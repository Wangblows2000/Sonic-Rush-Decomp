#include <nitro/types.h>

typedef struct {
    u16 bg0a : 1;
    u16 bg1a : 1;
    u16 bg2a : 1;
    u16 bg3a : 1;
    u16 obja : 1;
    u16 bda : 1;
    u16 mode : 2;
    u16 bg0b : 1;
    u16 bg1b : 1;
    u16 bg2b : 1;
    u16 bg3b : 1;
    u16 objb : 1;
    u16 bdb : 1;
    u16 pad : 2;
} UnkBlend_022d15e8;

typedef struct {
    u16 eva : 5;
    u16 pad0 : 3;
    u16 evb : 5;
    u16 pad1 : 3;
} UnkAlpha_022d15e8;

typedef struct {
    u8 pad0[0x20];
    UnkBlend_022d15e8 blend;
    UnkAlpha_022d15e8 alpha;
} UnkGfx_022d15e8;

typedef struct {
    u8 pad0[0x40];
} UnkObj_022d15e8;

typedef struct {
    u8 pad0[6];
    u16 unk6;
} UnkTbl_022d15e8;

typedef struct {
    u8 pad0[0x54];
    UnkObj_022d15e8 objs[(0x2d4 - 0x54) / 0x40];
    u8 unk2d4[0x41c - 0x2d4];
    s32 unk41c;
    u8 pad420[0xb34 - 0x420];
    u8 unkb34[0xd0c - 0xb34];
    u8 unkd0c[0xf0c - 0xd0c];
    u8 unkf0c[0x110c - 0xf0c];
    u8 unk110c[0x130c - 0x110c];
    u8 unk130c[4];
} UnkStruct_022d15e8;

extern UnkGfx_022d15e8 gDisplayMain;
extern UnkTbl_022d15e8 data_ov001_023160e4;

void func_ov001_022d0050(s32 a);
void func_ov001_022cff74(UnkObj_022d15e8 *o, s32 a, void *b, void *c);
void func_ov001_022d0158(s32 a);
void func_ov001_022cfe58(void *a, s32 b, u16 c, s32 d, void *e, void *f);
void Background_Draw(void *o);
void func_ov001_022d0128(s32 a);
void StaffRoll_SetState(UnkStruct_022d15e8 *p, void (*fn)(UnkStruct_022d15e8 *));
void func_ov001_022d1494(UnkStruct_022d15e8 *p);

#pragma thumb on
void func_ov001_022d15e8(UnkStruct_022d15e8 *p) {
    s32 i;

    func_ov001_022d0050(-0x10);
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0x800;
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1800;
    *(vu16 *)0x400000a = (*(vu16 *)0x400000a & 0x43) | 0x18c;
    *(vu16 *)0x400100a = (*(vu16 *)0x400100a & 0x43) | 0x18c;
    func_ov001_022cff74(&p->objs[2], 0x100, p->unkf0c, p->unkd0c);
    func_ov001_022d0158(3);
    func_ov001_022cfe58(p->unkb34, p->unk41c, data_ov001_023160e4.unk6, 0x100, p->unk130c, p->unk110c);
    gDisplayMain.blend.bg1a = 1;
    gDisplayMain.blend.mode = 1;
    gDisplayMain.alpha.eva = 0;
    gDisplayMain.alpha.evb = 15 - gDisplayMain.alpha.eva;
    Background_Draw(p->unk2d4);
    func_ov001_022d0128(2);
    for (i = 0; i < 4; i++) {
        func_ov001_022d0128(i);
    }
    StaffRoll_SetState(p, func_ov001_022d1494);
}
#pragma thumb reset

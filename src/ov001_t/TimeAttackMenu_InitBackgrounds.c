#include <nitro/types.h>

typedef struct {
    u8 pad[0x18];
    u16 unk18;
    u16 unk1a;
} UnkRes_022ebfd4;

typedef struct {
    u8 data[0x40];
} UnkBg_022ebfd4;

typedef struct {
    u32 unk0;
    u32 state;
    u8 pad8[0xe8 - 8];
    UnkBg_022ebfd4 bg[5];
    u8 pad228[0x106c - 0x228];
    UnkRes_022ebfd4 *res106c;
    UnkRes_022ebfd4 *res1070;
    UnkRes_022ebfd4 *res1074;
    UnkRes_022ebfd4 *res1078;
    UnkRes_022ebfd4 *res107c;
} UnkStruct_022ebfd4;

extern u16 gDisplayMain[];
extern u16 gDisplaySub[];

void Background_Init(void *buf, void *res, s32 a, s32 b, s32 c, s32 d, s32 e);
void Background_Draw(void *buf);
void TimeAttackMenu_Unk_ov001_022ea1b8(void *buf, s32 a, s32 b, s32 c);

#pragma thumb on
void TimeAttackMenu_InitBackgrounds(UnkStruct_022ebfd4 *p) {
    UnkRes_022ebfd4 *res;

    switch (p->state) {
    case 0:
    case 1:
        res = p->res107c;
        break;
    case 2:
    case 3:
        res = p->res1078;
        break;
    }
    Background_Init(&p->bg[4], res, 0x38, 1, 2, res->unk18, res->unk1a);
    Background_Draw(&p->bg[4]);
    res = p->res106c;
    Background_Init(&p->bg[0], res, 0x31, 0, 3, res->unk18, res->unk1a);
    Background_Init(&p->bg[1], res, 0x31, 1, 3, res->unk18, res->unk1a);
    Background_Draw(&p->bg[0]);
    Background_Draw(&p->bg[1]);
    if (p->state <= 1) {
        TimeAttackMenu_Unk_ov001_022ea1b8(&p->bg[0], 0, 0xf, 0);
        TimeAttackMenu_Unk_ov001_022ea1b8(&p->bg[1], 0, 0xf, 0);
    } else {
        TimeAttackMenu_Unk_ov001_022ea1b8(&p->bg[0], 0x10, 0x1f, 0);
        TimeAttackMenu_Unk_ov001_022ea1b8(&p->bg[1], 0x10, 0x1f, 0);
    }
    res = p->res1070;
    Background_Init(&p->bg[2], res, 0, 0, 1, res->unk18, res->unk1a);
    Background_Draw(&p->bg[2]);
    res = p->res1074;
    Background_Init(&p->bg[3], res, 0, 0, 2, res->unk18, res->unk1a);
    Background_Draw(&p->bg[3]);
    gDisplayMain[2] = 0x100;
    gDisplayMain[3] = 0;
    gDisplayMain[4] = 0;
    gDisplayMain[5] = 0;
    gDisplayMain[6] = 0;
    gDisplayMain[7] = 0;
    gDisplaySub[6] = 0;
    gDisplaySub[7] = 0;
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0x1e00;
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1c00;
}
#pragma thumb reset
